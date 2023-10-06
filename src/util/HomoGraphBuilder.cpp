#include "HomoGraphBuilder.h"

HomoGraphBuilder::HomoGraphBuilder(vector<vector<int>> graph, vector<int> vertexType, vector<int> edgeType, MetaPath queryMPath) {
    this->graph = graph;
    this->vertexType = vertexType;
    this->edgeType = edgeType;
    this->queryMPath = queryMPath;
}

map<int, vector<int>> HomoGraphBuilder::build() {
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
    map<int, vector<int>> pnbMap;
    for (int startID : keepSet) {
        vector<set<int>> visitList(queryMPath.pathLen + 1);
        set<int> nbSet;
        findAllNeighbors(startID, startID, 0, visitList, nbSet);
        vector<int> nbArr(nbSet.size());
        int i = 0;
        for (int nbId : nbSet) {
            nbArr[i] = nbId;
            i++;
        }
        pnbMap[startID] = nbArr;
        count += nbSet.size();
    }

    cout << "Meta-Path : " << queryMPath.toString() << " nb : " << count << endl;

    return pnbMap;
}

void HomoGraphBuilder::findAllNeighbors(int startID, int curID, int index, vector<set<int>> visitList, set<int> pnbSet) {
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
                if (nbVertexID != startID) {
                    pnbSet.insert(nbVertexID);
                    visitSet.insert(nbVertexID);
                }
            }
        }
    }
}