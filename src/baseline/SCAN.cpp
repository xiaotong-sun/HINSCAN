/**
 * @file SCAN.cpp
 * @author Xiaotong Sun
 * @date 2023-10-07
 *
 */

#include "SCAN.h"

SCAN::SCAN(const unordered_map<int, set<int>>& homoGraph, const map<int, vector<int>>& hinGraph, const vector<int> vertexType, const vector<int>& edgeType, const unordered_map<int, int>& edgeReverseMap, const MetaPath& metaPath) {
    // XXX: whether this data can be changed to const? or create another class?
    this->homoGraph = homoGraph;
    this->hinGraph = hinGraph;
    this->vertexType = vertexType;
    this->edgeType = edgeType;
    this->edgeReverseMap = edgeReverseMap;
    this->metaPath = metaPath;
    map<int, int> clusterMap; // set all vertex unclassified at the beginning.
    for (const auto& it : homoGraph) {
        clusterMap[it.first] = 0;
    }
    this->clusterMap = clusterMap;
}

void SCAN::getCommonEpsNbUsingPathsim(double eps) {
    int homoGraphVertex = 0;
    for (auto iter = homoGraph.begin(); iter != homoGraph.end(); iter++) {
        homoGraphVertex += iter->second.size() - 1;
    }
    cout << "--- total homoGraph neighbor num: " << homoGraphVertex << endl;
    cout << "--- average degree: " << (1.0 * homoGraphVertex / homoGraph.size()) << endl;

    // cout << "similarity of vertex" << endl;
    for (unordered_map<int, set<int>>::iterator iter = homoGraph.begin(); iter != homoGraph.end(); iter++) {
        int vertex = iter->first;
        set<int> neighbor_v = iter->second;

        for (int nb : neighbor_v) {
            if (nb < vertex) { // because these vertices have been considered in Line 51.
                continue;
            } else if (nb == vertex) {
                this->epsNbs[vertex].insert(nb);
                continue;
            }

            // calculate the basic p-structural similarity
            double similarity = ((double)psimMap.at(vertex).at(nb) + (double)psimMap.at(vertex).at(nb)) / ((double)psimMap.at(vertex).at(vertex) + (double)psimMap.at(nb).at(nb));
            // cout << vertex << "-" << nb << " : " << similarity << endl;
            if (similarity >= eps) {
                this->epsNbs[vertex].insert(nb);
                this->epsNbs[nb].insert(vertex);
            }
        }
    }

    // 统计common-eps neighbor会比homograph少掉多少点
    int epsNeighborVertex = 0;

    for (auto iter = epsNbs.begin(); iter != epsNbs.end(); iter++) {
        epsNeighborVertex += iter->second.size() - 1;
    }

    cout << "--- total epsNeighbor num: " << epsNeighborVertex << endl;
    cout << "--- pruned vertex number: " << homoGraphVertex - epsNeighborVertex << endl;
    cout << "--- target vertex num: " << homoGraph.size() << endl;
}

void SCAN::getCommonEpsNb(double eps) {
    int homoGraphVertex = 0;
    for (auto iter = homoGraph.begin(); iter != homoGraph.end(); iter++) {
        homoGraphVertex += iter->second.size() - 1;
    }
    cout << "--- total homoGraph neighbor num: " << homoGraphVertex << endl;
    cout << "--- average degree: " << (1.0 * homoGraphVertex / homoGraph.size()) << endl;

    // cout << "similarity of vertex" << endl;
    for (unordered_map<int, set<int>>::iterator iter = homoGraph.begin(); iter != homoGraph.end(); iter++) {
        int vertex = iter->first;
        set<int> neighbor_v = iter->second;

        for (int nb : neighbor_v) {
            if (nb < vertex) { // because these vertices have been considered in Line 51.
                continue;
            } else if (nb == vertex) {
                this->epsNbs[vertex].insert(nb);
                continue;
            }

            set<int> neighbor_w = homoGraph.at(nb);

            double eps_sq = pow(eps, 2);
            if (neighbor_v.size() < eps_sq * neighbor_w.size() || neighbor_w.size() < eps_sq * neighbor_v.size()) {
                continue;
            }

            // get the intersection
            set<int> commonNB;
            set_intersection(neighbor_v.begin(), neighbor_v.end(), neighbor_w.begin(),
                neighbor_w.end(), inserter(commonNB, commonNB.begin()));

            // calculate the basic p-structural similarity
            double similarity = commonNB.size() / sqrt(neighbor_v.size() * neighbor_w.size());
            // cout << vertex << "-" << nb << " : " << similarity << endl;
            if (similarity >= eps) {
                this->epsNbs[vertex].insert(nb);
                this->epsNbs[nb].insert(vertex);
            }
        }
    }

    // 统计common-eps neighbor会比homograph少掉多少点
    int epsNeighborVertex = 0;

    for (auto iter = epsNbs.begin(); iter != epsNbs.end(); iter++) {
        epsNeighborVertex += iter->second.size() - 1;
    }

    cout << "--- total epsNeighbor num: " << epsNeighborVertex << endl;
    cout << "--- pruned vertex number: " << homoGraphVertex - epsNeighborVertex << endl;
    cout << "--- target vertex num: " << homoGraph.size() << endl;
}

void SCAN::getDisjointEpsNb(double eps) {
    // cout << "similarity of vertex" << endl;
    for (unordered_map<int, set<int>>::iterator iter = homoGraph.begin(); iter != homoGraph.end(); iter++) {
        int vertex = iter->first;
        set<int> neighbor_v = iter->second;

        for (int nb : neighbor_v) {
            if (nb < vertex) { // because these vertices have been considered in Line 85.
                continue;
            } else if (nb == vertex) {
                this->epsNbs[vertex].insert(nb);
                continue;
            }

            set<int> neighbor_w = homoGraph.at(nb);

            double eps_sq = pow(eps, 2);
            if (neighbor_v.size() < eps_sq * neighbor_w.size() || neighbor_w.size() < eps_sq * neighbor_v.size()) {
                continue;
            }

            // get the intersection.
            set<int> commonNB;
            set_intersection(neighbor_v.begin(), neighbor_v.end(), neighbor_w.begin(),
                neighbor_w.end(), inserter(commonNB, commonNB.begin()));

            // get disjoint common p-neighbor.
            double lowerBound = eps * sqrt(neighbor_v.size() * neighbor_w.size());
            set<int> disjoinNB = disjoinNb(commonNB, vertex, nb, verifyTrueSet, verifyFalseSet, lowerBound);

            if (disjoinNB.size() >= lowerBound) {
                this->epsNbs[vertex].insert(nb);
                this->epsNbs[nb].insert(vertex);
            }
        }
    }
}

set<int> SCAN::disjoinNb(set<int>& commonNB, int vertexV, int vertexW, auto& verifyTrueSet, auto& verifyFalseSet, double lowerBound) {
    commonNB.erase(vertexV);
    commonNB.erase(vertexW);

    set<int> disjoinNB;

    // union {v, w}.
    disjoinNB.insert(vertexV);
    disjoinNB.insert(vertexW);

    int count = -1;

    for (int vertexU : commonNB) {
        count++;
        if (disjoinNB.size() >= lowerBound || disjoinNB.size() + (commonNB.size() - count) < lowerBound) {
            return disjoinNB;
        }
        if (verifyFalseSet.contains({ vertexU, vertexV, vertexW })) {
            continue;
        }
        if (verifyTrueSet.contains({ vertexU, vertexV, vertexW })) {
            disjoinNB.insert(vertexU);
            continue;
        }
        MyTuple tuple1 = { vertexV, vertexW, metaPath };
        MyTuple tuple2 = { vertexV, vertexU, metaPath };
        MyTuple tuple3 = { vertexW, vertexU, metaPath };
        vector<MyTuple> lambda = { tuple1, tuple2, tuple3 };
        if (verifyExistence(lambda)) {
            disjoinNB.insert(vertexU);
            verifyTrueSet.insert({ vertexU, vertexV, vertexW });
        } else {
            verifyFalseSet.insert({ vertexU, vertexV, vertexW });
        }
    }

    return disjoinNB;
}

bool SCAN::verifyExistence(vector<MyTuple>& lambda) {
    vector<set<int>> listOfComNb;
    verifyTimes++;

    for (MyTuple tup : lambda) {
        int pathVLen = tup.metaPath.vertex.size();
        int midIndex = (pathVLen - 1) >> 1;

        // get M(x_i)
        set<int> Mx_i = { tup.vertex1 };
        set<int> temp2;
        for (int i = 1; i <= midIndex; i++) {
            getNB(Mx_i, temp2, tup, i, false);
            Mx_i = temp2;
            temp2.clear();
        }

        // get M(y_i)
        set<int> My_i = { tup.vertex2 };
        for (int i = pathVLen - 2; i >= midIndex; i--) {
            getNB(My_i, temp2, tup, i, true);
            My_i = temp2;
            temp2.clear();
        }

        // get intersection
        set<int> intersection;
        set_intersection(Mx_i.begin(), Mx_i.end(), My_i.begin(), My_i.end(), inserter(intersection, intersection.begin()));

        // if (intersection.size() == 0) {

        //     cout << "Mx_i size: " << Mx_i.size() << endl;
        //     for (int i : Mx_i) {
        //         cout << i << " ";
        //     }
        //     cout << endl;

        //     cout << "My_i size: " << My_i.size() << endl;
        //     for (int i : My_i) {
        //         cout << i << " ";
        //     }
        //     cout << endl;

        //     cout << "v1: " << tup.vertex1 << "\tv2: " << tup.vertex2 << endl;
        // }

        listOfComNb.push_back(intersection);
    }

    // sort the vector<set<int>> in ascending order according to the size of each set.
    map<set<int>, int> listOfComNb2Lambda;
    for (int i = 0; i < listOfComNb.size(); i++) {
        listOfComNb2Lambda[listOfComNb[i]] = i;
    }

    auto compareSetSize = [](const set<int>& set1, const set<int>& set2) {
        return set1.size() < set2.size();
        };
    sort(listOfComNb.begin(), listOfComNb.end(), compareSetSize);

    vector<int> LArr; // record the different vertex. i.e. different instance.

    // enumeration and verify.
    vector<MyTuple> lambda2;
    for (int i = 0; i < listOfComNb.size(); i++) {
        lambda2.push_back(lambda[listOfComNb2Lambda.at(listOfComNb[i])]);
    }
    lambda = lambda2;
    return enumeration(listOfComNb, 0, LArr, lambda);
}

bool SCAN::hasSameValue(const vector<int>& arr, int vertex) {
    for (int val : arr) {
        if (val == vertex) {
            return true;
        }
    }
    return false;
}

bool SCAN::enumeration(const vector<set<int>>& listOfComNb, int index, vector<int>& LArr, vector<MyTuple>& lambda) {
    set<int> ComNb = listOfComNb.at(index);
    for (int vex : ComNb) {
        if (hasSameValue(LArr, vex)) {
            // cout << vex << " : ";
            // for (auto i : LArr) {
            //     cout << i << " ";
            // }
            // cout << endl;
            continue;
        }
        LArr.push_back(vex);
        if (index < listOfComNb.size() - 1) {
            if (enumeration(listOfComNb, index + 1, LArr, lambda)) {
                return true;
            } else {
                LArr.pop_back();
            }
        } else {
            vector<MyTuple> lambda2;
            for (int i = 0; i < lambda.size(); i++) {
                MyTuple element = lambda.at(i);
                int pathVLen = element.metaPath.vertex.size();
                int midIndex = (pathVLen - 1) >> 1;

                // generate l(Pj).
                vector<int> LVertex = { element.metaPath.vertex.at(0) };
                vector<int> LEdge;
                for (int i = 1; i <= midIndex; i++) {
                    LVertex.push_back(element.metaPath.vertex.at(i));
                    LEdge.push_back(element.metaPath.edge.at(i - 1));
                }
                MetaPath LMetaPath(LVertex, LEdge);
                if (LMetaPath.pathLen > 1) {
                    MyTuple LTup = { element.vertex1, LArr[i], LMetaPath };
                    // cout << element.vertex1 << "," << LArr[i] << " : " << LMetaPath.toString() << endl;
                    lambda2.push_back(LTup);
                }

                // generate r(Pj).
                vector<int> RVertex = { element.metaPath.vertex.at(pathVLen - 1) };
                vector<int> REdge;
                for (int i = pathVLen - 2; i >= midIndex; i--) {
                    RVertex.push_back(element.metaPath.vertex.at(i));
                    REdge.push_back(element.metaPath.edge.at(i));
                }
                MetaPath RMetaPath(RVertex, REdge);
                if (RMetaPath.pathLen > 1) {
                    MyTuple RTup = { element.vertex2, LArr[i], RMetaPath };
                    // cout << element.vertex2 << "," << LArr[i] << " : " << RMetaPath.toString() << endl;
                    lambda2.push_back(RTup);
                }
            }
            if (lambda2.empty()) {
                // cout << "find one" << endl;
                return true;
            } else {
                return verifyExistence(lambda2);
            }
        }
    }
    return false;
}

void SCAN::getNB(set<int>& M_i, set<int>& temp, MyTuple& tup, int index, bool fromRight) {
    for (int vex : M_i) {
        int targetVType = tup.metaPath.vertex.at(index);
        int targetEType;
        int targetEType2; // for the reverse edge.

        if (!fromRight) {
            targetEType = tup.metaPath.edge.at(index - 1);
        } else {
            targetEType = tup.metaPath.edge.at(index);
        }
        targetEType2 = this->edgeReverseMap[targetEType];
        vector<int> nbArr = hinGraph.at(vex);

        for (int j = 0; j < nbArr.size(); j += 2) {
            int nbVertexID = nbArr[j];
            int nbEdgeID = nbArr[j + 1];
            if (targetVType == vertexType[nbVertexID] && (targetEType == edgeType[nbEdgeID] || targetEType2 == edgeType[nbEdgeID])) {
                temp.insert(nbVertexID);
            }
        }
    }
}

bool SCAN::isCore(double eps, int mu, int vertex) {
    set<int> nb = this->epsNbs.at(vertex);
    if (nb.size() >= mu) {
        return true;
    }

    return false;
}

void SCAN::getCluster(double eps, int mu, int mode) {
    EffectiveTest effTest(hinGraph, vertexType, edgeType, metaPath);
    this->psimMap = effTest.getpsimMap();

    if (mode == 0) {
        // getCommonEpsNb(eps);
        getCommonEpsNbUsingPathsim(eps);
    } else if (mode == 1) {
        getDisjointEpsNb(eps);
    }

    // cout << "=================" << endl;
    // cout << "eps neighbor of each vertex" << endl;
    // for (unordered_map<int, set<int>>::iterator iter = epsNbs.begin(); iter != epsNbs.end(); iter++) {
    //     cout << iter->first << ": " << ends;
    //     // for (int val : iter->second) {
    //     //     cout << val << " " << ends;
    //     // }
    //     cout << iter->second.size();
    //     cout << endl;
    // }
    // cout << "=================" << endl;

    int clusterID = 0;
    set<int> non_member;

    for (unordered_map<int, set<int>>::iterator iter = homoGraph.begin(); iter != homoGraph.end(); iter++) {
        int vertex = iter->first;
        if (clusterMap[vertex] != 0) {
            continue;
        }

        // Step1: check whether vertex is a core.
        if (isCore(eps, mu, vertex)) {
            // Step2.1: if vertex is a core, a new cluster is expanded.
            clusterID++;
            queue<int> Q;
            for (int epsNb : this->epsNbs.at(vertex)) {
                Q.push(epsNb);
            }

            while (!Q.empty()) {
                int y = Q.front(); // get the first vertex in Q.

                set<int> DirReach;
                if (isCore(eps, mu, y)) {
                    DirReach = this->epsNbs.at(y);
                }

                for (int x : DirReach) {
                    if (clusterMap[x] == 0) {
                        clusterMap[x] = clusterID;
                        Q.push(x);
                    } else if (clusterMap[x] == -1) {
                        clusterMap[x] = clusterID;
                        non_member.erase(x);
                    }
                }

                Q.pop(); // remove the first vertex y from Q.
            }

        } else {
            // Step2.2: if vertex is not a core, it is labeled as non-member.
            clusterMap[vertex] = -1;
            non_member.insert(vertex);
        }
    }

    // Step3: further classifies non-members
    for (int v : non_member) {
        if (!isSameCluster(v)) {
            clusterMap[v] = -2;
        } else {
            clusterMap[v] = -3;
        }
    }

}

bool SCAN::isSameCluster(int vertex) {
    set<int> temp;
    for (int nb : homoGraph.at(vertex)) {
        if (nb == vertex) {
            continue;
        }
        if (clusterMap[nb] > 0) { // Only consider the true clusters
            temp.insert(clusterMap[nb]);
        }
        if (temp.size() > 1) {
            return false;
        }
    }

    return true;
}

void SCAN::showVerifyTimes() {
    cout << "------------------------------" << endl;
    cout << "verifyTrueSet size: " << verifyTrueSet.size() << endl;
    cout << "verifyFalseSet size: " << verifyFalseSet.size() << endl;
    cout << "verifyTimes: " << verifyTrueSet.size() + verifyFalseSet.size() << endl;
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
}

unordered_map<int, set<int>> SCAN::getCommunities() {
    unordered_map<int, set<int>> clusters;
    for (auto& iter : clusterMap) {
        int vid = iter.first;
        int cid = iter.second;
        if (cid < 1) {
            continue;
        }
        if (!clusters.contains(cid)) {
            set<int> cluser;
            clusters[cid] = cluser;
        }
        clusters[cid].insert(vid);
    }

    return clusters;
}