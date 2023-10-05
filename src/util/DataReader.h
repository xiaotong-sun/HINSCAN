#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

class DataReader {
private:
    string graphFile;
    string vertexFile;
    string edgeFile;
    int vertexNum = 0;
    int edgeNum = 0;

public:
    DataReader(string graphFile, string vertexFile, string edgeFile);

    vector<vector<int>> readGraph(); // return the graph edge information

    int* readVertexType(); // return the type of each vertex

    int* readEdgeType(); // return the type of each edge
};