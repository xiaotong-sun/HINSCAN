#include "DataReader.h"
#include "MetaPath.h"
#include "HomoGraphBuilder.h"
using namespace std;

int main() {
    string graphFile = "data\\expHIN\\graph.txt";
    string vertexFile = "data\\expHIN\\vertex.txt";
    string edgeFile = "data\\expHIN\\edge.txt";

    DataReader dr(graphFile, vertexFile, edgeFile);
    vector<vector<int>> graph = dr.readGraph();
    vector<int> vertexType = dr.readVertexType();
    vector<int> edgeType = dr.readEdgeType();

    string metaPathStr = "0 0 1 1 0";
    MetaPath metaPath(metaPathStr);
    // cout << metaPath.toString() << endl;

    HomoGraphBuilder homoGraph(graph, vertexType, edgeType, metaPath);
    map<int, vector<int>> pnbMap = homoGraph.build();

    for (int i : pnbMap[0]) {
        cout << i << " ";
    }
    cout << endl;

    for (int i : pnbMap[4]) {
        cout << i << " ";
    }
    cout << endl;
}