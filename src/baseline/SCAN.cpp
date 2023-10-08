#include "SCAN.h"

SCAN::SCAN(const map<int, set<int>>& homoGraph) {
    this->homoGraph = homoGraph;
    vector<int> cluster(homoGraph.size(), 0); // set all vertex unclassified at the beginning.
    this->cluster = cluster;
}

void SCAN::getEpsNb(double eps) {
    for (map<int, set<int>>::iterator iter = homoGraph.begin(); iter != homoGraph.end(); iter++) {
        int vertex = iter->first;
        set<int> neighbor_v = iter->second;
        set<int> epsNb;

        for (int nb : neighbor_v) {
            if (nb == vertex) {
                epsNb.insert(nb);
                continue;
            }

            set<int> neighbor_w = homoGraph.at(nb);

            // get the intersection
            vector<int> result;
            set_intersection(neighbor_v.begin(), neighbor_v.end(), neighbor_w.begin(),
                neighbor_w.end(), back_inserter(result));

            // calculate the basic p-structural similarity
            double similarity = result.size() / sqrt(neighbor_v.size() * neighbor_w.size());
            if (similarity >= eps) {
                epsNb.insert(nb);
            }
        }

        this->epsNbs[vertex] = epsNb;
    }
}

bool SCAN::isCore(double eps, int mu, int vertex) {
    set<int> nb = this->epsNbs.at(vertex);
    if (nb.size() >= mu) {
        return true;
    }

    return false;
}

void SCAN::getCluster(double eps, int mu) {
    getEpsNb(eps);
    int clusterID = 0;
    set<int> non_member;

    for (map<int, set<int>>::iterator iter = homoGraph.begin(); iter != homoGraph.end(); iter++) {
        int vertex = iter->first;
        if (cluster[vertex] != 0) {
            continue;
        }

        // Step1: check whether vertex is a core.
        if (isCore(eps, mu, vertex)) {
            // Step2.1: if vertex is a core, a new cluster is expanded.
            clusterID++;
            queue<int> Q;
            for (int epsNb : this->epsNbs.at(vertex)) {
                Q.push(epsNb);
            }

            while (!Q.empty()) {
                int y = Q.front(); // get the first vertex in Q.

                set<int> DirReach;
                if (isCore(eps, mu, y)) {
                    DirReach = this->epsNbs.at(y);
                }

                for (int x : DirReach) {
                    if (cluster[x] == 0) {
                        cluster[x] = clusterID;
                        Q.push(x);
                    } else if (cluster[x] == -1) {
                        cluster[x] = clusterID;
                        non_member.erase(x);
                    }
                }

                Q.pop(); // remove the first vertex y from Q.
            }

        } else {
            // Step2.2: if vertex is not a core, it is labeled as non-member.
            cluster[vertex] = -1;
            non_member.insert(vertex);
        }
    }

    // Step3: further classifies non-members
    for (int v : non_member) {
        if (!isSameCluster(v)) {
            cluster[v] = -2;
        } else {
            cluster[v] = -3;
        }
    }

}

bool SCAN::isSameCluster(int vertex) {
    set<int> temp;
    for (int nb : homoGraph.at(vertex)) {
        if (nb == vertex) {
            continue;
        }
        temp.insert(cluster[nb]);
        if (temp.size() > 1) {
            return false;
        }
    }

    return true;
}

