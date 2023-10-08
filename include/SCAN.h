#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <set>

using namespace std;

class SCAN {
public:
    map<int, set<int>> homoGraph;
    vector<int> cluster; // record the clusterID of each vertex. unclassified=0, non-memeber=-1, member>=1

    SCAN(const map<int, set<int>>& homoGraph);

    // get eps-Neighborhood.
    void getEpsNb(double eps);

    // check whether this vertex is a core or not.
    bool isCore(double eps, int mu, int vertex);

    // get cluster, hub, outlier.
    void getCluster(double eps, int mu);
};