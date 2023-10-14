#include "DataReader.h"
#include "MetaPath.h"
#include "HomoGraphBuilder.h"
#include "SCAN.h"

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
    map<int, set<int>> pnbMap = homoGraph.build();

    for (int i = 0; i < pnbMap.size(); i++) {
        cout << i + 1 << ": ";
        for (int j : pnbMap[i]) {
            cout << j + 1 << " ";
        }
        cout << endl;
    }

    SCAN myScan(pnbMap, graph, vertexType, edgeType, metaPath);
    myScan.getCluster(0.68, 3, 0);

    for (int i = 0; i < myScan.cluster.size(); i++) {
        cout << i << ":" << myScan.cluster[i] << endl;
    }
}