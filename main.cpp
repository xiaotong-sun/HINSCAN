#include "DataReader.h"
#include "MetaPath.h"
#include "HomoGraphBuilder.h"
#include "SCAN.h"
#include "Utility.h"
#include "Pscan.h"

using namespace std;

/*
    [0]:exe, [1]:data-dir, [2]:similarity-threshold, [3]:density-threshold
    [4]:cluster-mode, [5]:meta-path, [6]:output-file, [7]:flag-index
*/
int main(int argc, char* argv[]) {
    string Path = argv[1];
    double eps = atof(argv[2]);
    int mu = atoi(argv[3]);
    int mode = atoi(argv[4]);
    string outputFile;
    if (argc >= 7) {
        outputFile = argv[6];
    } else {
        outputFile = "/homoGraph.txt";
    }

    struct timeval start;
    gettimeofday(&start, NULL);

    string graphFile = Path + "/graph.txt";
    string vertexFile = Path + "/vertex.txt";
    string edgeFile = Path + "/edge.txt";
    string reverseMapFile = Path + "/edgeReverseMap.txt";
    string homoGraphFile = Path + outputFile;
    string clusterResultFile = Path + "/ClusterResult-" + argv[2] + "-" + argv[3] + "-" + argv[4] + ".txt";

    DataReader dr(graphFile, vertexFile, edgeFile, reverseMapFile);
    vector<vector<int>> graph = dr.readGraph();
    vector<int> vertexType = dr.readVertexType();
    vector<int> edgeType = dr.readEdgeType();
    unordered_map<int, int> edgeReverseMap = dr.readReverseMap();

    string metaPathStr = argv[5];
    MetaPath metaPath(metaPathStr);
    // cout << metaPath.toString() << endl;

    long long mtime1 = getTime(start);

    // int count = estimate(graph, vertexType, metaPath, atoi(argv[7]));
    // cout << "count = " << count << endl;

    HomoGraphBuilder homoGraph(graph, vertexType, edgeType, metaPath, edgeReverseMap);
    // map<int, set<int>> pnbMap = homoGraph.build();
    // map<int, set<int>> pnbMap = homoGraph.build_optim1();
    // map<int, set<int>> pnbMap = homoGraph.build_optim2();
    unordered_map<int, set<int>> pnbMap = homoGraph.build_forTest(atoi(argv[7]));

    long long mtime2 = getTime(start);

    cout << "Time of HomoGraph build without IO: " << (mtime2 - mtime1) << "(us)" << endl;

    // writeToFile(homoGraphFile, pnbMap);
    // unordered_map<int, set<int>> pnbMap = readFromFile(homoGraphFile);

    // cout << "=================" << endl;
    // cout << "neighbor of each vertex (HomoGraph)" << endl;
    // for (const auto& iter : pnbMap) {
    //     cout << iter.first << ": ";
    //     for (auto& j : iter.second) {
    //         cout << j << " ";
    //     }
    //     cout << endl;
    // }
    // cout << "=================" << endl;


    // TODO: check whether the modify is correct!!
    Pscan myPscan(pnbMap);
    myPscan.get_graph();
    long long mtime3 = getTime(start);
    myPscan.pSCAN(argv[2], mu);
    long long mtime4 = getTime(start);
    myPscan.output(argv[2], argv[3], argv[1]);

    cout << "Time of pScan Clustering: " << (mtime4 - mtime3) << "(us)" << endl;

    SCAN myScan(pnbMap, graph, vertexType, edgeType, edgeReverseMap, metaPath);
    long long mtime5 = getTime(start);
    myScan.getCluster(eps, mu, mode);
    long long mtime6 = getTime(start);

    if (mode == 0) {
        cout << "Time of basic Scan Clustering: " << (mtime6 - mtime5) << "(us)" << endl;
    } else if (mode == 1) {
        cout << "Time of disjoint Scan Clustering: " << (mtime6 - mtime5) << "(us)" << endl;
    }

    // cout << "=================" << endl;
    // cout << "cluster result" << endl;
    // for (const auto& it : myScan.clusterMap) {
    //     cout << it.first << ":" << it.second << endl;
    // }
    writeClusterResultToFile(clusterResultFile, myScan.clusterMap);
}
