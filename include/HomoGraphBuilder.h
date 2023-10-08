#pragma once
#include "MetaPath.h"
#include <map>
#include <set>
#include <iostream>

using namespace std;

class HomoGraphBuilder {
private:
    vector<vector<int>> graph; // data graph, including vertex IDs, edge IDs, and their link relationships.
    vector<int> vertexType; // vertex -> type
    vector<int> edgeType; // edge -> type
    MetaPath queryMPath; // the query meta-path

public:
    HomoGraphBuilder(const vector<vector<int>>& graph, const vector<int>& vertexType,
        const vector<int>& edgeType, const MetaPath& queryMPath);

    map<int, set<int>> build();

    void findAllNeighbors(int startID, int curID, int index, vector<set<int>>& visitList, set<int>& pnbSet);
};