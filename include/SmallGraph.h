/**
 * @file SmallGraph.h
 * @author Xiaotong Sun
 * @date 2024-07-29
 *
 */

#pragma once
#include <map>
#include <vector>
#include <set>
#include <unordered_map>

using namespace std;

class SmallGraph {
public:
    map<int, int> newVidMap;
    map<int, int> newEidMap;
    map<int, int> oldVidMap;
    map<int, vector<int>> smallGraph;
    vector<int> smallGraphVertexType;
    vector<int> smallGraphEdgeType;

    SmallGraph(const map<int, vector<int>>& graph, const vector<int>& vertexType, const vector<int>& edgeType);

    void getSmallGraph(int part, int total);
    void setBigGraph();

private:
    map<int, vector<int>> graph;
    vector<int> vertexType;
    vector<int> edgeType;
    unordered_map<int, int> edgeReverseMap;

    void getNewVidMap(int part, int total);

    void getSmallGraphVertexType();

};