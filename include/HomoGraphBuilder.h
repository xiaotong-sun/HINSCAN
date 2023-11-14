/**
 * @file HomoGraphBuilder.h
 * @author Xiaotong Sun
 * @date 2023-10-07
 *
 */

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
    unordered_map<int, int> edgeReverseMap; // edge <-> edge

public:
    HomoGraphBuilder(const vector<vector<int>>& graph, const vector<int>& vertexType,
        const vector<int>& edgeType, const MetaPath& queryMPath, const unordered_map<int, int>& edgeReverseMap);

    map<int, set<int>> build(); // Common build without optimize. Just do search for each vertex.
    map<int, set<int>> build_optim1(); // A-P-T-P-A, Search from T for the metaPath.
    map<int, set<int>> build_optim2(); // A-P-T-P-A, Search from P for half of the metaPath.

private:
    void findAllNeighbors(int startID, int curID, int index, vector<set<int>>& visitList, set<int>& pnbSet);
    void findTargetFromMidType(int startID, int curID, int index, vector<set<int>>& visitList, set<int>& targetSet);

    void findLeftTarget(int startID, int curID, int index, vector<set<int>>& visitList, set<int>& leftTargetSet);
    void findRightTarget(int startID, int curID, int index, vector<set<int>>& visitList, set<int>& rightTargetSet, int flagIndex);
};