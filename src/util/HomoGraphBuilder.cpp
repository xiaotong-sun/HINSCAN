/**
 * @file HomoGraphBuilder.cpp
 * @author Xiaotong Sun
 * @brief
 * @date 2023-10-07
 *
 */

#include "HomoGraphBuilder.h"

HomoGraphBuilder::HomoGraphBuilder(const vector<vector<int>>& graph, const vector<int>& vertexType,
    const vector<int>& edgeType, const MetaPath& queryMPath) {
    this->graph = graph;
    this->vertexType = vertexType;
    this->edgeType = edgeType;
    this->queryMPath = queryMPath;
}

map<int, set<int>> HomoGraphBuilder::build() {
    // step1: collect vertices of the same type;
    int StarType = queryMPath.vertex[0];
    set<int> keepSet;
    for (int i = 0; i < vertexType.size(); i++) {
        if (vertexType[i] == StarType) {
            keepSet.insert(i);
        }
    }

    long count = 0; // count the total nb of vertices in keepSet.

    // step2: find neighbors
    map<int, set<int>> pnbMap;

    cout << "keesSet.size = " << keepSet.size() << endl;
    int fl = 0;

    for (int startID : keepSet) {
        ++fl;
        if (fl % 500 == 0) {
            cout << fl << endl;
        }

        vector<set<int>> visitList(queryMPath.pathLen + 1);
        set<int> nbSet = { startID };
        findAllNeighbors(startID, startID, 0, visitList, nbSet);
        pnbMap[startID] = nbSet;
        count += nbSet.size();
    }

    // cout << "Meta-Path : " << queryMPath.toString() << " nb : " << count << endl;

    return pnbMap;
}

void HomoGraphBuilder::findAllNeighbors(int startID, int curID, int index, vector<set<int>>& visitList, set<int>& pnbSet) {
    int targetVType = queryMPath.vertex[index + 1];
    int targetEType = queryMPath.edge[index];

    vector<int> nbArr = graph[curID];
    for (int i = 0; i < nbArr.size(); i += 2) {
        int nbVertexID = nbArr[i];
        int nbEdgeID = nbArr[i + 1];
        set<int> visitSet = visitList[index + 1];
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

map<int, set<int>> HomoGraphBuilder::build_optim1() {
    // step1: collect vertices of the same type with mid vertex in the meta-path.
    int mid = queryMPath.pathLen / 2;
    int MidType = queryMPath.vertex[mid];
}

void HomoGraphBuilder::findNeighborsFromMidType() {

}