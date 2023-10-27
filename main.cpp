#include "DataReader.h"
#include "MetaPath.h"
#include "HomoGraphBuilder.h"
#include "SCAN.h"
#include <fstream>

using namespace std;

// write the homoGraph to file.
void writeToFile(string filePath, map<int, set<int>>& pnbMap) {
    ofstream of(filePath);
    for (const auto& iter : pnbMap) {
        of << iter.first;
        for (auto& elem : iter.second) {
            of << " " << elem;
        }
        of << endl;
    }
}

// read the homoGraph from file.
map<int, set<int>> readFromFile(string filePath) {
    map<int, set<int>> myMap;

    ifstream fs(filePath);
    string line;
    while (getline(fs, line)) {
        istringstream iss(line);
        int key;
        iss >> key;
        int value;
        set<int> temp;
        while (iss >> value) {
            temp.insert(value);
        }
        myMap[key] = temp;
    }

    return myMap;
}

int main() {
    string graphFile = "data\\expHIN\\graph.txt";
    string vertexFile = "data\\expHIN\\vertex.txt";
    string edgeFile = "data\\expHIN\\edge.txt";
    string reverseMapFile = "data\\expHIN\\edgeReverseMap.txt";
    string homoGraphFile = "data\\expHIN\\homeGraph.txt";

    // string graphFile = "data\\expHIN_origin\\graph.txt";
    // string vertexFile = "data\\expHIN_origin\\vertex.txt";
    // string edgeFile = "data\\expHIN_origin\\edge.txt";
    // string reverseMapFile = "data\\expHIN_origin\\edgeReverseMap.txt";

    // string graphFile = "data\\smallimdb\\graph.txt";
    // string vertexFile = "data\\smallimdb\\vertex.txt";
    // string edgeFile = "data\\smallimdb\\edge.txt";
    // string reverseMapFile = "data\\smallimdb\\edgeReverseMap.txt";

    DataReader dr(graphFile, vertexFile, edgeFile, reverseMapFile);
    vector<vector<int>> graph = dr.readGraph();
    vector<int> vertexType = dr.readVertexType();
    vector<int> edgeType = dr.readEdgeType();
    unordered_map<int, int> edgeReverseMap = dr.readReverseMap();

    // string metaPathStr = "0 0 1 1 0";
    string metaPathStr = "0 0 1 4 3 5 1 1 0";
    // string metaPathStr = "3 5 0 0 1 1 0 4 3";
    MetaPath metaPath(metaPathStr);
    // cout << metaPath.toString() << endl;

    HomoGraphBuilder homoGraph(graph, vertexType, edgeType, metaPath);
    map<int, set<int>> pnbMap = homoGraph.build();
    // writeToFile(homoGraphFile, pnbMap);
    // map<int, set<int>> pnbMap = readFromFile(homoGraphFile);

    cout << "=================" << endl;
    cout << "neighbor of each vertex" << endl;
    for (int i = 0; i < pnbMap.size(); i++) {
        cout << i + 1 << ": ";
        for (int j : pnbMap[i]) {
            cout << j + 1 << " ";
        }
        cout << endl;
    }
    cout << "=================" << endl;

    SCAN myScan(pnbMap, graph, vertexType, edgeType, edgeReverseMap, metaPath);
    myScan.getCluster(0.68, 3, 1);

    cout << "cluster result" << endl;
    for (int i = 0; i < myScan.cluster.size(); i++) {
        cout << i << ":" << myScan.cluster[i] << endl;
    }
}

