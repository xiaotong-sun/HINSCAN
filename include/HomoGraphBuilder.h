/**
 * @file HomoGraphBuilder.h
 * @author Xiaotong Sun
 * @date 2023-10-07
 *
 */

#pragma once
#include "MetaPath.h"
#include <unordered_map>
#include <set>
#include <iostream>
#include <unordered_set>
#include <Utility.h>
#include <map>
#include <omp.h>
#include <atomic>
#include <tbb/concurrent_hash_map.h>

using namespace std;

class HomoGraphBuilder {
private:
    const map<int, vector<int>> graph; // data graph, including vertex IDs, edge IDs, and their link relationships.
    const vector<int> vertexType; // vertex -> type
    const vector<int> edgeType; // edge -> type
    const MetaPath queryMPath; // the query meta-path
    const unordered_map<int, int> edgeReverseMap; // edge <-> edge
    int totalVisitNodeNum; // used to count the total number of visit node.

public:
    HomoGraphBuilder(const map<int, vector<int>>& graph, const vector<int>& vertexType,
        const vector<int>& edgeType, const MetaPath& queryMPath, const unordered_map<int, int>& edgeReverseMap);

    unordered_map<int, set<int>> build(); // Common build without optimize. Just do search for each vertex.
    unordered_map<int, set<int>> build_optim1(); // A-P-T-P-A, Search from T(mid type) for half of the metaPath.
    unordered_map<int, set<int>> build_optim2(); // A-P-T-P-A, Search from P for half of the metaPath.
    void build_forTest(int flagIndex, unordered_map<int, set<int>>& pnbMap); // A-P & P-T-P-A || A-P-T & T-P-A.
    void build_parallel(int flagIndex, tbb::concurrent_hash_map<int, set<int>>& pnbMap); // use OpenMP for parallel.

private:
    void findAllNeighbors(int startID, int curID, int index, vector<unordered_set<int>>& visitList, set<int>& pnbSet);
    void findTargetFromMidType(int startID, int curID, int index, vector<unordered_set<int>>& visitList, set<int>& targetSet);

    void findLeftTarget(int startID, int curID, int index, vector<unordered_set<int>>& visitList, set<int>& leftTargetSet, int flagIndex);
    void findRightTarget(int startID, int curID, int index, vector<unordered_set<int>>& visitList, set<int>& rightTargetSet, int flagIndex);
    void findRightTarget_test(int startID, int curID, int index, vector<unordered_set<int>>& visitList, set<int>& rightTargetSet, int flagIndex);
};