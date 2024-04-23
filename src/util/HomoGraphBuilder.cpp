/**
 * @file HomoGraphBuilder.cpp
 * @author Xiaotong Sun
 * @brief
 * @date 2023-10-07
 *
 */

#include "HomoGraphBuilder.h"

HomoGraphBuilder::HomoGraphBuilder(const vector<vector<int>>& graph, const vector<int>& vertexType,
    const vector<int>& edgeType, const MetaPath& queryMPath, const unordered_map<int, int>& edgeReverseMap) {
    this->graph = graph;
    this->vertexType = vertexType;
    this->edgeType = edgeType;
    this->queryMPath = queryMPath;
    this->edgeReverseMap = edgeReverseMap;

    this->totalVisitNodeNum = 0;
}

unordered_map<int, set<int>> HomoGraphBuilder::build() {
    // step1: collect vertices of the same type;
    int StartType = queryMPath.vertex[0];
    set<int> keepSet;
    for (int i = 0; i < vertexType.size(); i++) {
        if (vertexType[i] == StartType) {
            keepSet.insert(i);
        }
    }

    long count = 0; // count the total nb of vertices in keepSet.

    // step2: find neighbors
    unordered_map<int, set<int>> pnbMap;

    cout << "keepSet.size = " << keepSet.size() << endl;
    int fl = 0;

    for (int startID : keepSet) {
        ++fl;
        if (fl % 500 == 0) {
            cout << fl << endl;
        }

        vector<unordered_set<int>> visitList(queryMPath.pathLen + 1);
        set<int> nbSet = { startID };
        findAllNeighbors(startID, startID, 0, visitList, nbSet);
        pnbMap[startID] = nbSet;
        count += nbSet.size();
    }

    // cout << "Meta-Path : " << queryMPath.toString() << " nb : " << count << endl;

    return pnbMap;
}

void HomoGraphBuilder::findAllNeighbors(int startID, int curID, int index, vector<unordered_set<int>>& visitList, set<int>& pnbSet) {
    int targetVType = queryMPath.vertex[index + 1];
    int targetEType = queryMPath.edge[index];

    vector<int> nbArr = graph[curID];
    for (int i = 0; i < nbArr.size(); i += 2) {
        int nbVertexID = nbArr[i];
        int nbEdgeID = nbArr[i + 1];
        unordered_set<int>& visitSet = visitList[index + 1];
        // if (visitSet.contains(nbVertexID)) {
        //     cout << "true" << endl;
        // }
        if (targetVType == vertexType[nbVertexID] && targetEType == edgeType[nbEdgeID] && !visitSet.contains(nbVertexID)) {
            if (index + 1 < queryMPath.pathLen) {
                findAllNeighbors(startID, nbVertexID, index + 1, visitList, pnbSet);
                visitSet.insert(nbVertexID);
            } else {
                pnbSet.insert(nbVertexID);
                visitSet.insert(nbVertexID);
            }
        }
    }
}

unordered_map<int, set<int>> HomoGraphBuilder::build_optim1() {
    // step1: collect vertices of the same type with mid vertex in the meta-path.
    int mid = queryMPath.pathLen / 2;
    int MidType = queryMPath.vertex[mid];
    int StartType = queryMPath.vertex[0];

    set<int> keepSet;
    unordered_map<int, set<int>> pnbMap;

    for (int i = 0; i < vertexType.size(); i++) {
        if (vertexType[i] == MidType) {
            keepSet.insert(i);
        }
        if (vertexType[i] == StartType) {
            pnbMap[i].insert(i);
        }
    }

    // step2: find target vertices that connected to the mid vertex.
    cout << "keepSet.size = " << keepSet.size() << endl;
    int fl = 0;

    for (int startID : keepSet) {
        ++fl;
        if (fl % 500 == 0) {
            cout << fl << endl;
        }

        vector<unordered_set<int>> visitList(queryMPath.pathLen / 2 + 1);
        set<int> targetSet;
        findTargetFromMidType(startID, startID, mid, visitList, targetSet);

        // cout << startID << ":";
        // for (const int& elem : targetSet) {
        //     cout << elem << " ";
        // }
        // cout << endl;

        // step3: generate pnbMap by union the targetSet one by one.
        for (const int& elem : targetSet) {
            pnbMap[elem].insert(targetSet.begin(), targetSet.end());
        }
    }

    return pnbMap;
}

// I think this is the same to findAllNeighbors, the only difference is that it only search half of the path.
void HomoGraphBuilder::findTargetFromMidType(int startID, int curID, int index, vector<unordered_set<int>>& visitList, set<int>& targetSet) {
    int targetVType = queryMPath.vertex[index + 1];
    int targetEType = queryMPath.edge[index];
    int mid = queryMPath.pathLen / 2;

    vector<int> nbArr = graph[curID];
    for (int i = 0; i < nbArr.size(); i += 2) {
        int nbVertexID = nbArr[i];
        int nbEdgeID = nbArr[i + 1];
        unordered_set<int>& visitSet = visitList[index + 1 - mid];
        if (targetVType == vertexType[nbVertexID] && targetEType == edgeType[nbEdgeID] && !visitSet.contains(nbVertexID)) {
            if (index + 1 < queryMPath.pathLen) {
                findTargetFromMidType(startID, nbVertexID, index + 1, visitList, targetSet);
                visitSet.insert(nbVertexID);
            } else {
                targetSet.insert(nbVertexID);
                visitSet.insert(nbVertexID);
            }
        }
    }
}

unordered_map<int, set<int>> HomoGraphBuilder::build_optim2() { // step1: collect vertices of the same type with vertex in the meta-path.
    set<int> keepSet;
    unordered_map<int, set<int>> pnbMap;
    int flagIndex;
    int mid = queryMPath.pathLen / 2;

    if (queryMPath.pathLen == 2) {
        return build_optim1();
    } else if (queryMPath.pathLen == 4) {
        flagIndex = 1;
        int FlagType = queryMPath.vertex[flagIndex];
        int StartType = queryMPath.vertex[0];

        for (int i = 0; i < vertexType.size(); i++) {
            if (vertexType[i] == FlagType) {
                keepSet.insert(i);
            }
            if (vertexType[i] == StartType) {
                pnbMap[i].insert(i);
            }
        }
    } else if (queryMPath.pathLen >= 6) {
        int flagIndexL = 1;
        int flagIndexR = mid - 1;
        int FlagTypeL = queryMPath.vertex[flagIndexL];
        int FlagTypeR = queryMPath.vertex[flagIndexR];
        int StartType = queryMPath.vertex[0];
        set<int> keepSetL;
        set<int> keepSetR;

        for (int i = 0; i < vertexType.size(); i++) {
            if (vertexType[i] == FlagTypeL) {
                keepSetL.insert(i);
            } else if (vertexType[i] == FlagTypeR) {
                keepSetR.insert(i);
            } else if (vertexType[i] == StartType) {
                pnbMap[i].insert(i);
            }
        }

        if (keepSetL.size() <= keepSetR.size()) {
            keepSet = keepSetL;
            flagIndex = flagIndexL;
        } else {
            keepSet = keepSetR;
            flagIndex = flagIndexR;
        }
    }

    // step2: find target vertices that connected to the target vertex.
    cout << "keepSet.size = " << keepSet.size() << endl;
    int fl = 0;

    unordered_map<int, set<int>> tempMap;
    for (int startID : keepSet) {
        ++fl;
        if (fl % 500 == 0) {
            cout << fl << endl;
        }

        vector<unordered_set<int>> visitListForL(flagIndex + 1);
        vector<unordered_set<int>> visitListForR(mid - flagIndex + 1);
        set<int> leftTargetSet;
        set<int> rightTargetSet;

        findLeftTarget(startID, startID, flagIndex, visitListForL, leftTargetSet, flagIndex);
        findRightTarget(startID, startID, flagIndex, visitListForR, rightTargetSet, flagIndex);

        // step3: generate pnbMap by union the leftTargetSet and rightTargetSet one by one.
        for (auto& elem1 : rightTargetSet) {
            tempMap[elem1].insert(leftTargetSet.begin(), leftTargetSet.end());
        }
    }

    for (const auto& item : tempMap) {
        for (const int& elem : item.second) {
            pnbMap[elem].insert(item.second.begin(), item.second.end());
        }
    }

    return pnbMap;
}

// A-P-T-P-A: Choose P as FlagType, left target is A, left path is A-P.
void HomoGraphBuilder::findLeftTarget(int startID, int curID, int index, vector<unordered_set<int>>& visitList, set<int>& leftTargetSet, int flagIndex) {
    totalVisitNodeNum ++;
    if (flagIndex == 0) {
        leftTargetSet.insert(startID);
        return;
    }

    int targetVType = queryMPath.vertex[index - 1];
    int targetEType = queryMPath.edge[index - 1]; // note the edgetype should be reverse.
    targetEType = edgeReverseMap[targetEType];

    vector<int> nbArr = graph[curID];
    for (int i = 0; i < nbArr.size(); i += 2) {
        int nbVertexID = nbArr[i];
        int nbEdgeID = nbArr[i + 1];
        unordered_set<int>& visitSet = visitList[index - 1];
        if (!visitSet.contains(nbVertexID) && targetVType == vertexType[nbVertexID] && targetEType == edgeType[nbEdgeID]) {
            if (index - 1 > 0) {
                findLeftTarget(startID, nbVertexID, index - 1, visitList, leftTargetSet, flagIndex);
                visitSet.insert(nbVertexID);
            } else {
                leftTargetSet.insert(nbVertexID);
                visitSet.insert(nbVertexID);
            }
        }
    }

}

// A-P-T-P-A: Choose P as FlagType, right target is T, right path is P-T.
void HomoGraphBuilder::findRightTarget(int startID, int curID, int index, vector<unordered_set<int>>& visitList, set<int>& rightTargetSet, int flagIndex) {
    int targetVType = queryMPath.vertex[index + 1];
    int targetEType = queryMPath.edge[index];

    vector<int> nbArr = graph[curID];
    for (int i = 0; i < nbArr.size(); i += 2) {
        int nbVertexID = nbArr[i];
        int nbEdgeID = nbArr[i + 1];
        unordered_set<int>& visitSet = visitList[index - flagIndex + 1];
        if (targetVType == vertexType[nbVertexID] && targetEType == edgeType[nbEdgeID] && !visitSet.contains(nbVertexID)) {
            if (index + 1 < queryMPath.pathLen / 2) {
                findRightTarget(startID, nbVertexID, index + 1, visitList, rightTargetSet, flagIndex);
                visitSet.insert(nbVertexID);
            } else {
                rightTargetSet.insert(nbVertexID);
                visitSet.insert(nbVertexID);
            }
        }
    }
}

/*
    HACK: This code will count the num of join, so if you don't want the count (maybe run faster) please comment out corresponding lines.
*/
void HomoGraphBuilder::build_forTest(int flagIndex, unordered_map<int, set<int>>& pnbMap) {
    // DELETE: To evaluate the time.
    struct timeval start;
    gettimeofday(&start, nullptr);
    long long timeStart = getTime(start);
    long long getKeepsetTime = 0, totalJionTime = 0, totalSearchTime = 0;

    // step1: collect vertices of the same type with vertex of flagIndex.
    set<int> keepSet;
    // unordered_map<int, set<int>> pnbMap;

    int FlagType = queryMPath.vertex[flagIndex];
    int StartType = queryMPath.vertex[0];

    long long time1 = getTime(start);
    for (int i = 0; i < vertexType.size(); i++) {
        if (vertexType[i] == FlagType) {
            keepSet.insert(i);
        }
        if (vertexType[i] == StartType) {
            pnbMap[i].insert(i);
        }
    }
    long long time2 = getTime(start);
    getKeepsetTime = time2 - time1;

    // step2: find target vertices that connected to the target vertex.
    cout << "keepSet.size = " << keepSet.size() << endl;
    int fl = 0;

    int totalJoin = 0, usefulJoin = 0;

    for (int startID : keepSet) {
        ++fl;
        if (fl % 500 == 0) {
            cout << fl << endl;
        }

        vector<unordered_set<int>> visitListForL(flagIndex + 1);
        vector<unordered_set<int>> visitListForR(queryMPath.pathLen - flagIndex + 1);
        set<int> leftTargetSet;
        set<int> rightTargetSet;

        long long time3 = getTime(start);
        findLeftTarget(startID, startID, flagIndex, visitListForL, leftTargetSet, flagIndex);
        findRightTarget_test(startID, startID, flagIndex, visitListForR, rightTargetSet, flagIndex);
        long long time4 = getTime(start);
        totalSearchTime += time4 - time3;

        totalJoin += leftTargetSet.size() * rightTargetSet.size();

        // FIXME step3: generate pnbMap by union the leftTargetSet and rightTargetSet one by one.

        long long time5 = getTime(start);
        for (auto& elem1 : leftTargetSet) {
            int originSize = pnbMap[elem1].size();
            pnbMap[elem1].insert(rightTargetSet.begin(), rightTargetSet.end());
            usefulJoin += pnbMap[elem1].size() - originSize;
        }
        long long time6 = getTime(start);
        totalJionTime += time6 - time5;
    }

    long long timeEnd = getTime(start);

    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    cout << "total join:\t" << totalJoin << endl;
    cout << "useful join:\t" << usefulJoin << endl;
    cout << "useless join:\t" << totalJoin - usefulJoin << endl;
    cout << "getKeepset time:\t" << getKeepsetTime << "(us)" << endl;
    cout << "total search time:\t" << totalSearchTime << "(us)" << endl;
    cout << "total join time:\t" << totalJionTime << "(us)" << endl;
    cout << "total build time:\t" << timeEnd - timeStart << "(us)" << endl;
    cout << "total visit node num:\t" << totalVisitNodeNum << endl;
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
}

// A-P-T-P-A: Choose P as FlagType, right target(for test) is A, right path is P-T-P-A.
void HomoGraphBuilder::findRightTarget_test(int startID, int curID, int index, vector<unordered_set<int>>& visitList, set<int>& rightTargetSet, int flagIndex) {
    totalVisitNodeNum ++;
    if (flagIndex == queryMPath.pathLen) {
        rightTargetSet.insert(startID);
        return;
    }

    int targetVType = queryMPath.vertex[index + 1];
    int targetEType = queryMPath.edge[index];

    vector<int> nbArr = graph[curID];
    for (int i = 0; i < nbArr.size(); i += 2) {
        int nbVertexID = nbArr[i];
        int nbEdgeID = nbArr[i + 1];
        unordered_set<int>& visitSet = visitList[index - flagIndex + 1];

        if (!visitSet.contains(nbVertexID) && targetVType == vertexType[nbVertexID] && targetEType == edgeType[nbEdgeID]) {
            if (index + 1 < queryMPath.pathLen) {
                findRightTarget_test(startID, nbVertexID, index + 1, visitList, rightTargetSet, flagIndex);
                visitSet.insert(nbVertexID);
            } else {
                rightTargetSet.insert(nbVertexID);
                visitSet.insert(nbVertexID);
            }
        }
    }
}
