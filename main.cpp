#include "DataReader.h"
#include "MetaPath.h"
#include "HomoGraphBuilder.h"
#include "SCAN.h"
#include "Utility.h"
#include "Pscan.h"
#include "SmallGraph.h"
#include "EffecitveTest.h"

using namespace std;

static const auto io_sync_off = []() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return nullptr;
    }();

/*
    [0]:exe, [1]:data-dir, [2]:similarity-threshold, [3]:density-threshold
    [4]:cluster-mode, [5]:meta-path, [6]:output-file, [7]:flag-index, [8]:scalability.
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
    gettimeofday(&start, nullptr);

    string graphFile = Path + "/graph.txt";
    string vertexFile = Path + "/vertex.txt";
    string edgeFile = Path + "/edge.txt";
    string reverseMapFile = Path + "/edgeReverseMap.txt";
    string homoGraphFile = Path + outputFile;
    string clusterResultFile = Path + "/ClusterResult-" + argv[2] + "-" + argv[3] + "-" + argv[4] + ".txt";

    DataReader dr(graphFile, vertexFile, edgeFile, reverseMapFile);
    map<int, vector<int>> graph = dr.readGraph();
    vector<int> vertexType = dr.readVertexType();
    vector<int> edgeType = dr.readEdgeType();
    unordered_map<int, int> edgeReverseMap = dr.readReverseMap();

    string metaPathStr = argv[5];
    MetaPath metaPath(metaPathStr);
    // cout << metaPath.toString() << endl;

    // int count = estimate(graph, vertexType, metaPath, atoi(argv[7]));
    // cout << "count = " << count << endl;

    // getVertexNum(vertexType);


    /* === bellow is used for scalability test === */
    // SmallGraph sGraph(graph, vertexType, edgeType);
    // int sc = atoi(argv[8]);
    // if (sc >= 1 && sc <= 4) {
    //     cout << "Scalability = " << (sc / 5.0) << endl;
    //     sGraph.getSmallGraph(sc, 5);
    // } else {
    //     cout << "Scalability = 1" << endl;
    //     sGraph.setBigGraph();
    // }
    // getVertexNum(sGraph.smallGraphVertexType);

    unordered_map<int, set<int>> pnbMap;
    // HomoGraphBuilder homoGraph(sGraph.smallGraph, sGraph.smallGraphVertexType, sGraph.smallGraphEdgeType, metaPath, edgeReverseMap);
    HomoGraphBuilder homoGraph(graph, vertexType, edgeType, metaPath, edgeReverseMap);
    // long long mtime1 = getTime(start);
    // pnbMap = homoGraph.build();
    // map<int, set<int>> pnbMap = homoGraph.build_optim1();
    // map<int, set<int>> pnbMap = homoGraph.build_optim2();
    homoGraph.build_forTest(atoi(argv[7]), pnbMap);

    // long long mtime2 = getTime(start);
    // long long buildTime = mtime2 - mtime1;

    // cout << "Time of HomoGraph build without IO: " << buildTime << "(us)" << endl;

    // writeToFile(homoGraphFile, pnbMap);
    // pnbMap = readFromFile(homoGraphFile);
    // cout << "Finish Reading" << endl;

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

    // Pscan myPscan(pnbMap, graph, vertexType, edgeType, edgeReverseMap, metaPath, 0);
    // myPscan.get_graph();
    // cout << "Begin pSCAN" << endl;
    // long long mtime3 = getTime(start);
    // myPscan.pSCAN(argv[2], mu);
    // long long mtime4 = getTime(start);
    // long long useTime = mtime4 - mtime3;
    // cout << "Time of joint pScan Clustering: " << useTime << "(us)" << endl;
    // // myPscan.output(argv[2], argv[3], argv[1]);
    // unordered_map<int, set<int>> communities = myPscan.getCluster();

    // Pscan myPscan(pnbMap, graph, vertexType, edgeType, edgeReverseMap, metaPath, 0);
    // myPscan.get_graph();
    // cout << "Begin pSCAN" << endl;
    // long long mtime3 = getTime(start);
    // myPscan.pSCAN2(argv[2], mu);
    // long long mtime4 = getTime(start);
    // long long useTime = mtime4 - mtime3;
    // cout << "Time of disjoint pScan Clustering: " << useTime << "(us)" << endl;
    // // myPscan.output(argv[2], argv[3], argv[1]);
    // unordered_map<int, set<int>> communities = myPscan.getCluster();
    // myPscan.output("myPscan1", argv[3], argv[1]);
    // myPscan.showTime();
    // cout << "TOTAL TIME: " << useTime + buildTime << "(us)" << endl;
    // myPscan.showVerifyTimes();
    // myPscan.showGetNBTimes();
    // myPscan.showMessage();

    // ######## below is useless ########

    // Pscan myPscan2(pnbMap, graph, vertexType, edgeType, edgeReverseMap, metaPath, 1);
    // myPscan2.get_graph();
    // mtime3 = getTime(start);
    // myPscan2.pSCAN_disjoint(argv[2], mu, myPscan.getMinCN());
    // mtime4 = getTime(start);
    // cout << "Time of optimized disjoint pScan Clustering: " << useTime + (mtime4 - mtime3) << "(us)" << endl;
    // myPscan2.showTime();
    // myPscan2.output(argv[2], argv[3], argv[1]);
    // myPscan2.output("myPscan2", argv[3], argv[1]);
    // myPscan2.showVerifyTimes();
    // myPscan2.showGetNBTimes();
    // myPscan2.showMessage();

    // Pscan myPscan3(pnbMap, graph, vertexType, edgeType, edgeReverseMap, metaPath, 1);
    // myPscan3.get_graph();
    // mtime3 = getTime(start);
    // myPscan3.pSCAN(argv[2], mu);
    // mtime4 = getTime(start);
    // cout << "Time of no optimized disjoint pScan Clustering: " << (mtime4 - mtime3) << "(us)" << endl;
    // myPscan3.output(argv[2], argv[3], argv[1]);
    // myPscan3.output("myPscan3", argv[3], argv[1]);
    // myPscan3.showVerifyTimes();
    // myPscan3.showGetNBTimes();
    // myPscan3.showMessage();

    // ######## above is useless ########

    // mode = 0;
    SCAN myScan(pnbMap, graph, vertexType, edgeType, edgeReverseMap, metaPath);
    cout << "Begin Scan" << endl;
    // long long mtime5 = getTime(start);
    myScan.getCluster(eps, mu, mode);
    unordered_map<int, set<int>> communities = myScan.getCommunities();
    // long long mtime6 = getTime(start);
    // long long useTime = mtime6 - mtime5;

    // if (mode == 0) {
    //     cout << "Time of basic Scan Clustering: " << useTime << "(us)" << endl;
    // } else if (mode == 1) {
    //     cout << "Time of disjoint Scan Clustering: " << useTime << "(us)" << endl;
    //     // myScan.showVerifyTimes();
    // }
    // cout << "TOTAL TIME: " << useTime + buildTime << "(us)" << endl;


    cout << "=================" << endl;
    cout << "cluster result" << endl;
    for (const auto& it : myScan.clusterMap) {
        cout << it.first << ":" << it.second << endl;
    }
    // writeClusterResultToFile(clusterResultFile, myScan.clusterMap);

    /******* EffectiveTest *******/
    EffectiveTest efTest(graph, vertexType, edgeType, metaPath, communities);
    efTest.process();
}
