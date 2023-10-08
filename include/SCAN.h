#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>
#include <queue>

using namespace std;

class SCAN {
public:
    map<int, set<int>> homoGraph;

    // record the clusterID of each vertex. unclassified=0, non-memeber=-1, cluster>=1, hub=-2, outlier=-3.
    vector<int> cluster;

    // record the eps-Neighborhood for each vertex.
    map<int, set<int>> epsNbs;

    SCAN(const map<int, set<int>>& homoGraph);

    // get cluster, hub, outlier.
    void getCluster(double eps, int mu);

private:
    // check whether all vertices in Neighbor(v) belong to the same cluster or not.
    bool isSameCluster(int vertex);

    // get eps-Neighborhood.
    void getEpsNb(double eps);

    // check whether this vertex is a core or not.
    bool isCore(double eps, int mu, int vertex);
};