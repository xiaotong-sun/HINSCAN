#include "SCAN.h"

SCAN::SCAN(const map<int, set<int>>& homoGraph) {
    this->homoGraph = homoGraph;
    vector<int> cluster(homoGraph.size(), 0); // set all vertex unclassified at the beginning.
    this->cluster = cluster;
}

void SCAN::getEpsNb(double eps) {

}

bool SCAN::isCore(double eps, int mu, int vertex) {
    set<int> neighbors = homoGraph.at(vertex);
    set<int> neighbor_v = homoGraph.at(vertex);

    for (int nb : neighbor_v) {
        if (nb == vertex) {
            continue;
        }
        set<int> neighbor_w = homoGraph.at(nb);

        // get the intersection
        sort(neighbor_v.begin(), neighbor_v.end());
        sort(neighbor_w.begin(), neighbor_w.end());
        vector<int> result;
        set_intersection(neighbor_v.begin(), neighbor_v.end(), neighbor_w.begin(),
            neighbor_w.end(), back_inserter(result));

        // calculate the basic p-structural similarity
        double similarity = result.size() / sqrt(neighbor_v.size() * neighbor_w.size());
        if (similarity <= eps) {
            neighbors.erase(nb);
        }
    }

    if (neighbors.size() >= mu) {
        return true;
    }

    return false;
}

void SCAN::getCluster(double eps, int mu) {
    int clusterID = 0;
    for (map<int, set<int>>::iterator iter = homoGraph.begin(); iter != homoGraph.end(); iter++) {
        int vertex = iter->first;
        if (cluster[vertex] != 0) {
            continue;
        }

        // Step1: check whether vertex is a core.
        if (isCore(eps, mu, vertex)) {
            // Step2.1: if vertex is a core, a new cluster is expanded.
            clusterID++;

        } else {
            // Step2.2: if vertex is not a core, it is labeled as non-member.
            cluster[vertex] = -1;
        }
    }

    // Step3: further classifies non-members


}