#include "DataReader.h"
#include "MetaPath.h"
#include "HomoGraphBuilder.h"
#include "SCAN.h"

using namespace std;

int main() {
    string graphFile = "data\\expHIN\\graph.txt";
    string vertexFile = "data\\expHIN\\vertex.txt";
    string edgeFile = "data\\expHIN\\edge.txt";
    string reverseMapFile = "data\\expHIN\\edgeReverseMap.txt";

    // string graphFile = "data\\expHIN_origin\\graph.txt";
    // string vertexFile = "data\\expHIN_origin\\vertex.txt";
    // string edgeFile = "data\\expHIN_origin\\edge.txt";
    // string reverseMapFile = "data\\expHIN_origin\\edgeReverseMap.txt";

    DataReader dr(graphFile, vertexFile, edgeFile, reverseMapFile);
    vector<vector<int>> graph = dr.readGraph();
    vector<int> vertexType = dr.readVertexType();
    vector<int> edgeType = dr.readEdgeType();
    unordered_map<int, int> edgeReverseMap = dr.readReverseMap();

    // string metaPathStr = "0 0 1 1 0";
    string metaPathStr = "0 0 1 4 3 5 1 1 0";
    MetaPath metaPath(metaPathStr);
    // cout << metaPath.toString() << endl;

    HomoGraphBuilder homoGraph(graph, vertexType, edgeType, metaPath);
    map<int, set<int>> pnbMap = homoGraph.build();

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