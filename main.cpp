#include "DataReader.h"
#include "MetaPath.h"
#include "HomoGraphBuilder.h"
#include "SCAN.h"
#include "Utility.h"
#include "Pscan.h"
#include <sys/time.h>

using namespace std;

/*
    [0]:exe, [1]:data-dir, [2]:similarity-threshold, [3]:density-threshold
    [4]:cluster-mode, [5]:meta-path
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
        outputFile = "/homeGraph.txt";
    }

    struct timeval start, end1, end2;
    gettimeofday(&start, NULL);

    string graphFile = Path + "/graph.txt";
    string vertexFile = Path + "/vertex.txt";
    string edgeFile = Path + "/edge.txt";
    string reverseMapFile = Path + "/edgeReverseMap.txt";
    string homoGraphFile = Path + outputFile;

    DataReader dr(graphFile, vertexFile, edgeFile, reverseMapFile);
    vector<vector<int>> graph = dr.readGraph();
    vector<int> vertexType = dr.readVertexType();
    vector<int> edgeType = dr.readEdgeType();
    unordered_map<int, int> edgeReverseMap = dr.readReverseMap();

    string metaPathStr = argv[5];
    // string metaPathStr = "0 0 1 1 0"; // for expHIN_origin & expHIN & CaseStudy
    // string metaPathStr = "0 0 1 4 3 5 1 1 0"; // for expHIN_origin & expHIN
    // string metaPathStr = "3 5 0 0 1 1 0 4 3"; // for smallimdb
    MetaPath metaPath(metaPathStr);
    // cout << metaPath.toString() << endl;

    gettimeofday(&end1, NULL);
    long long mtime1, seconds1, useconds1;
    seconds1 = end1.tv_sec - start.tv_sec;
    useconds1 = end1.tv_usec - start.tv_usec;
    mtime1 = seconds1 * 1000000 + useconds1;

    HomoGraphBuilder homoGraph(graph, vertexType, edgeType, metaPath);
    // map<int, set<int>> pnbMap = homoGraph.build();
    map<int, set<int>> pnbMap = homoGraph.build_optim1();

    gettimeofday(&end2, NULL);
    long long mtime2, seconds2, useconds2;
    seconds2 = end2.tv_sec - start.tv_sec;
    useconds2 = end2.tv_usec - start.tv_usec;
    mtime2 = seconds2 * 1000000 + useconds2;

    cout << "Time of HomoGraph build without IO: " << (mtime2 - mtime1) << "(us)" << endl;

    writeToFile(homoGraphFile, pnbMap);
    // map<int, set<int>> pnbMap = readFromFile(homoGraphFile);

    cout << "=================" << endl;
    cout << "neighbor of each vertex (HomoGraph)" << endl;
    for (const auto& iter : pnbMap) {
        cout << iter.first << ": ";
        for (auto& j : iter.second) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << "=================" << endl;

    // Pscan myPscan(pnbMap);
    // myPscan.get_graph();
    // myPscan.pSCAN(argv[2], mu);
    // myPscan.output(argv[2], argv[3], argv[1]);

    SCAN myScan(pnbMap, graph, vertexType, edgeType, edgeReverseMap, metaPath);
    // myScan.getCluster(0.68, 3, 1); // for expHIN_origin & expHIN
    // myScan.getCluster(0.64, 3, 0); // for case study
    myScan.getCluster(eps, mu, mode);

    cout << "cluster result" << endl;
    for (int i = 0; i < myScan.cluster.size(); i++) {
        cout << i << ":" << myScan.cluster[i] << endl;
    }
}