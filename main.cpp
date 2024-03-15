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

    struct timeval start, end1, end2, end3, end4, end5, end6;
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

    // int count = estimate(graph, vertexType, metaPath, atoi(argv[7]));
    // cout << "count = " << count << endl;

    HomoGraphBuilder homoGraph(graph, vertexType, edgeType, metaPath, edgeReverseMap);
    // map<int, set<int>> pnbMap = homoGraph.build();
    // map<int, set<int>> pnbMap = homoGraph.build_optim1();
    // map<int, set<int>> pnbMap = homoGraph.build_optim2();
    unordered_map<int, set<int>> pnbMap = homoGraph.build_forTest(atoi(argv[7]));

    gettimeofday(&end2, NULL);
    long long mtime2, seconds2, useconds2;
    seconds2 = end2.tv_sec - start.tv_sec;
    useconds2 = end2.tv_usec - start.tv_usec;
    mtime2 = seconds2 * 1000000 + useconds2;

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

    gettimeofday(&end3, NULL);
    long long mtime3, seconds3, useconds3;
    seconds3 = end3.tv_sec - start.tv_sec;
    useconds3 = end3.tv_usec - start.tv_usec;
    mtime3 = seconds3 * 1000000 + useconds3;

    // Pscan myPscan(pnbMap);
    // myPscan.get_graph();
    // myPscan.pSCAN(argv[2], mu);
    // myPscan.output(argv[2], argv[3], argv[1]);

    gettimeofday(&end4, NULL);
    long long mtime4, seconds4, useconds4;
    seconds4 = end4.tv_sec - start.tv_sec;
    useconds4 = end4.tv_usec - start.tv_usec;
    mtime4 = seconds4 * 1000000 + useconds4;

    cout << "Time of pScan Clustering: " << (mtime4 - mtime3) << "(us)" << endl;

    gettimeofday(&end5, NULL);
    long long mtime5, seconds5, useconds5;
    seconds5 = end5.tv_sec - start.tv_sec;
    useconds5 = end5.tv_usec - start.tv_usec;
    mtime5 = seconds5 * 1000000 + useconds5;

    SCAN myScan(pnbMap, graph, vertexType, edgeType, edgeReverseMap, metaPath);
    // myScan.getCluster(0.68, 3, 1); // for expHIN_origin & expHIN
    // myScan.getCluster(0.64, 3, 0); // for case study
    myScan.getCluster(eps, mu, mode);

    gettimeofday(&end6, NULL);
    long long mtime6, seconds6, useconds6;
    seconds6 = end6.tv_sec - start.tv_sec;
    useconds6 = end6.tv_usec - start.tv_usec;
    mtime6 = seconds6 * 1000000 + useconds6;

    if (mode == 0) {
        cout << "Time of basic Scan Clustering: " << (mtime6 - mtime5) << "(us)" << endl;
    } else if (mode == 1) {
        cout << "Time of disjoint Scan Clustering: " << (mtime6 - mtime5) << "(us)" << endl;
    }

    cout << "=================" << endl;
    cout << "cluster result" << endl;
    for (const auto& it : myScan.clusterMap) {
        cout << it.first << ":" << it.second << endl;
    }
}
