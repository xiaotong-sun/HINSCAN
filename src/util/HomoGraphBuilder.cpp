#include "HomoGraphBuilder.h"

HomoGraphBuilder::HomoGraphBuilder(vector<vector<int>> graph, vector<int> vertexType, vector<int> edgeType, MetaPath queryMPath) {
    this->graph = graph;
    this->vertexType = vertexType;
    this->edgeType = edgeType;
    this->queryMPath = queryMPath;
}

map<int, vector<int>> HomoGraphBuilder::build() {
    // step1: collect vertices of the same type;
}

void HomoGraphBuilder::findAllNeighbors(int startID, int curID, int index, vector<set<int>> visitList, set<int> pnbSet) {

}