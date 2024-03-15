/**
 * @file Utility.h
 * @author Xiaotong Sun
 * @date 2023-11-09
 *
 */

#pragma once
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>
#include "MetaPath.h"

using namespace std;

// write the homoGraph to file.
void writeToFile(string filePath, const unordered_map<int, set<int>>& pnbMap);

// read the homoGraph from file.
unordered_map<int, set<int>> readFromFile(string filePath);

// roughly evaluation the number of total join.
int estimate(vector<vector<int>>& graph, vector<int>& vertexType, MetaPath& metaPath, int flagIndex);

// obtain vertices of each type.
unordered_map<int, set<int>> obtainVertexOfType(vector<int>& vertexType);

// estimate from left
unordered_map<int, int> estimateLeft(vector<vector<int>>& graph, vector<int>& vertexType, MetaPath& metaPath, unordered_map<int, set<int>>& vertexTypeMap, unordered_map<int, int> vertexCountMap, int flagIndex);

// estimate from right
unordered_map<int, int> estimateRight(vector<vector<int>>& graph, vector<int>& vertexType, MetaPath& metaPath, unordered_map<int, set<int>>& vertexTypeMap, unordered_map<int, int> vertexCountMap, int flagIndex);

void writeClusterResultToFile(string filePath, const unordered_map<int, int>& clusterMap);