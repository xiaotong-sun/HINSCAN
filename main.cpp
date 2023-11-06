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
    cout << "Finish writing to file!" << endl;
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

/*
    [0]:exe, [1]:data-dir, [2]:similarity-threshold, [3]:density-threshold
    [4]:cluster-mode, [5]:meta-path
*/
int main(int argc, char* argv[]) {
    string Path = argv[1];
    double eps = atof(argv[2]);
    int mu = atoi(argv[3]);
    int mode = atoi(argv[4]);

    // string graphFile = "data\\expHIN\\graph.txt";
    // string vertexFile = "data\\expHIN\\vertex.txt";
    // string edgeFile = "data\\expHIN\\edge.txt";
    // string reverseMapFile = "data\\expHIN\\edgeReverseMap.txt";
    // string homoGraphFile = "data\\expHIN\\homeGraph.txt";

    // string graphFile = "data\\expHIN_origin\\graph.txt";
    // string vertexFile = "data\\expHIN_origin\\vertex.txt";
    // string edgeFile = "data\\expHIN_origin\\edge.txt";
    // string reverseMapFile = "data\\expHIN_origin\\edgeReverseMap.txt";

    // string graphFile = "data\\smallimdb\\graph.txt";
    // string vertexFile = "data\\smallimdb\\vertex.txt";
    // string edgeFile = "data\\smallimdb\\edge.txt";
    // string reverseMapFile = "data\\smallimdb\\edgeReverseMap.txt";

    // string graphFile = "data\\CaseStudy2\\graph.txt";
    // string vertexFile = "data\\CaseStudy2\\vertex.txt";
    // string edgeFile = "data\\CaseStudy2\\edge.txt";
    // string reverseMapFile = "data\\CaseStudy2\\edgeReverseMap.txt";
    // string homoGraphFile = "data\\CaseStudy2\\homeGraph.txt";

    string graphFile = Path + "\\graph.txt";
    string vertexFile = Path + "\\vertex.txt";
    string edgeFile = Path + "\\edge.txt";
    string reverseMapFile = Path + "\\edgeReverseMap.txt";
    string homoGraphFile = Path + "\\homeGraph.txt";

    DataReader dr(graphFile, vertexFile, edgeFile, reverseMapFile);
    vector<vector<int>> graph = dr.readGraph();
    vector<int> vertexType = dr.readVertexType();
    vector<int> edgeType = dr.readEdgeType();
    unordered_map<int, int> edgeReverseMap = dr.readReverseMap();

    string metaPathStr = argv[5];
    cout << metaPathStr << endl;
    // string metaPathStr = "0 0 1 1 0"; // for expHIN_origin & expHIN & CaseStudy
    // string metaPathStr = "0 0 1 4 3 5 1 1 0"; // for expHIN_origin & expHIN
    // string metaPathStr = "3 5 0 0 1 1 0 4 3"; // for smallimdb
    MetaPath metaPath(metaPathStr);
    // cout << metaPath.toString() << endl;

    HomoGraphBuilder homoGraph(graph, vertexType, edgeType, metaPath);
    map<int, set<int>> pnbMap = homoGraph.build();
    writeToFile(homoGraphFile, pnbMap);
    // map<int, set<int>> pnbMap = readFromFile(homoGraphFile);

    cout << "=================" << endl;
    cout << "neighbor of each vertex (HomoGraph)" << endl;
    for (int i = 0; i < pnbMap.size(); i++) {
        cout << i << ": ";
        for (int j : pnbMap[i]) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << "=================" << endl;

    SCAN myScan(pnbMap, graph, vertexType, edgeType, edgeReverseMap, metaPath);
    // myScan.getCluster(0.68, 3, 1); // for expHIN_origin & expHIN
    // myScan.getCluster(0.64, 3, 0); // for case study
    myScan.getCluster(eps, mu, mode);

    cout << "cluster result" << endl;
    for (int i = 0; i < myScan.cluster.size(); i++) {
        cout << i << ":" << myScan.cluster[i] << endl;
    }
}

