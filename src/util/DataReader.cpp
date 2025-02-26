/**
 * @file DataReader.cpp
 * @author Xiaotong Sun
 * @date 2023-10-07
 *
 */

#include "DataReader.h"

DataReader::DataReader(string graphFile, string vertexFile, string edgeFile, string reverseMapFile) {
    this->graphFile = graphFile;
    this->vertexFile = vertexFile;
    this->edgeFile = edgeFile;
    this->reverseMapFile = reverseMapFile;

    ifstream fileStream(graphFile);
    if (!fileStream.is_open()) {
        cerr << "Error: could not read file " << graphFile << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(fileStream, line)) {
        vertexNum++; // count the number of lines, which is the number of nodes
    }

    fileStream.close();
}

map<int, vector<int>> DataReader::readGraph() {
    ifstream fileStream(graphFile);

    if (!fileStream.is_open()) {
        cerr << "Error: could not read file " << graphFile << endl;
        exit(EXIT_FAILURE);
    }

    map<int, vector<int>> graph;
    string line;

    while (getline(fileStream, line)) {
        istringstream iss(line);
        vector<int> nb;

        int vertexID;
        iss >> vertexID;

        int val;
        while (iss >> val) {
            nb.push_back(val);
        }
        edgeNum += nb.size() / 2;
        graph[vertexID] = nb;
    }

    fileStream.close();

    cout << graphFile << " |V|=" << vertexNum << " |E|=" << edgeNum / 2 << endl; // each edge is bidirectional

    return graph;
}

vector<int> DataReader::readVertexType() {
    ifstream fileStream(vertexFile);

    if (!fileStream.is_open()) {
        cerr << "Error: could not read file " << vertexFile << endl;
        exit(EXIT_FAILURE);
    }

    vector<int> vertexType(vertexNum);
    string line;

    while (getline(fileStream, line)) {
        istringstream iss(line);
        int id, type;
        iss >> id;
        iss >> type;
        vertexType[id] = type;
    }

    fileStream.close();

    return vertexType;
}

vector<int> DataReader::readEdgeType() {
    ifstream fileStream(edgeFile);

    if (!fileStream.is_open()) {
        cerr << "Error: could not read file " << edgeFile << endl;
        exit(EXIT_FAILURE);
    }

    vector<int> edgeType(edgeNum);
    string line;

    while (getline(fileStream, line)) {
        istringstream iss(line);
        int id, type;
        iss >> id;
        iss >> type;
        edgeType[id] = type;
    }

    fileStream.close();

    return edgeType;
}


unordered_map<int, int> DataReader::readReverseMap() {
    ifstream fileStream(reverseMapFile);

    if (!fileStream.is_open()) {
        cerr << "Error: could not read file " << reverseMapFile << endl;
        exit(EXIT_FAILURE);
    }

    unordered_map<int, int> edgeReverseMap;
    string line;

    while (getline(fileStream, line)) {
        istringstream iss(line);
        int key, value;
        iss >> key;
        iss >> value;
        edgeReverseMap[key] = value;
    }

    fileStream.close();

    return edgeReverseMap;
}