#include "DataReader.h"

DataReader::DataReader(string graphFile, string vertexFile, string edgeFile) {
    this->graphFile = graphFile;
    this->vertexFile = vertexFile;
    this->edgeFile = edgeFile;

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

vector<vector<int>> DataReader::readGraph() {
    ifstream fileStream(graphFile);

    if (!fileStream.is_open()) {
        cerr << "Error: could not read file " << graphFile << endl;
        exit(EXIT_FAILURE);
    }

    vector<vector<int>> graph(vertexNum);
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

int* DataReader::readVertexType() {
    ifstream fileStream(vertexFile);

    if (!fileStream.is_open()) {
        cerr << "Error: could not read file " << vertexFile << endl;
        exit(EXIT_FAILURE);
    }

    int* vertexType = new int[vertexNum];
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

int* DataReader::readEdgeType() {
    ifstream fileStream(edgeFile);

    if (!fileStream.is_open()) {
        cerr << "Error: could not read file " << edgeFile << endl;
        exit(EXIT_FAILURE);
    }

    int* edgeType = new int[edgeNum];
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