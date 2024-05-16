/**
 * @file MetaPath.h
 * @author Xiaotong Sun
 * @date 2023-10-07
 *
 */

#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <map>

using namespace std;

class DataReader {
private:
    string graphFile;
    string vertexFile;
    string edgeFile;
    string reverseMapFile;
    int vertexNum = 0;
    int edgeNum = 0;

public:
    DataReader(string graphFile, string vertexFile, string edgeFile, string reverseMapFile);

    map<int, vector<int>> readGraph(); // return the graph edge information

    vector<int> readVertexType(); // return the type of each vertex

    vector<int> readEdgeType(); // return the type of each edge

    unordered_map<int, int> readReverseMap(); // return the edge reverse map.
};