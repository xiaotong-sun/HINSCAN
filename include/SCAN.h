/**
 * @file SCAN.h
 * @author Xiaotong Sun
 * @date 2023-10-07
 *
 */

#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <queue>
#include "MetaPath.h"

using namespace std;

class SCAN {
public:
    struct MyTuple {
        int vertex1;
        int vertex2;
        MetaPath metaPath;
    };

    // store the homogeneous graph.
    map<int, set<int>> homoGraph;

    // store the HIN graph.
    vector<vector<int>> hinGraph;

    // store the type of each vertex.
    vector<int> vertexType;

    // store the type of each edge.
    vector<int> edgeType;

    // store the meta-path.
    MetaPath metaPath;

    // record the clusterID of each vertex. unclassified=0, non-memeber=-1, cluster>=1, hub=-2, outlier=-3.
    vector<int> cluster;

    // record the eps-Neighborhood for each vertex.
    map<int, set<int>> epsNbs;

    SCAN(const map<int, set<int>>& homoGraph, const vector<vector<int>>& hinGraph, const vector<int> vertexType, const vector<int>& edgeType, const MetaPath& metaPath);

    // get cluster, hub, outlier.
    void getCluster(double eps, int mu);

private:
    // check whether all vertices in Neighbor(v) belong to the same cluster or not.
    bool isSameCluster(int vertex);

    // get eps-Neighborhood (common p-neighbor).
    void getCommonEpsNb(double eps);

    // get eps-Neighborhood (disjoint common p-neighbor).
    void getDisjointEpsNb(double eps);

    // get disjoint common p-enighbor.
    set<int> disjoinNb(const set<int>& commonNB, int vertexV, int vertexW);

    // verify if there exist three instances of P connecting three P-pairs.
    bool verifyExistence(vector<MyTuple> lambda);

    // check whether this vertex is a core or not.
    bool isCore(double eps, int mu, int vertex);
};