/**
 * @file SCAN.cpp
 * @author Xiaotong Sun
 * @date 2023-10-07
 *
 */

#include "SCAN.h"

SCAN::SCAN(const map<int, set<int>>& homoGraph, const vector<vector<int>>& hinGraph, const vector<int> vertexType, const vector<int>& edgeType, const MetaPath& metaPath) {
    // HACK: whether this data can be changed to const? or create another class?
    this->homoGraph = homoGraph;
    this->hinGraph = hinGraph;
    this->vertexType = vertexType;
    this->edgeType = edgeType;
    this->metaPath = metaPath;
    vector<int> cluster(homoGraph.size(), 0); // set all vertex unclassified at the beginning.
    this->cluster = cluster;
}

void SCAN::getCommonEpsNb(double eps) {
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
            set<int> commonNB;
            set_intersection(neighbor_v.begin(), neighbor_v.end(), neighbor_w.begin(),
                neighbor_w.end(), back_inserter(commonNB));

            // calculate the basic p-structural similarity
            double similarity = commonNB.size() / sqrt(neighbor_v.size() * neighbor_w.size());
            if (similarity >= eps) {
                epsNb.insert(nb);
            }
        }

        this->epsNbs[vertex] = epsNb;
    }
}

// HACK: this code can be simplified.
void SCAN::getDisjointEpsNb(double eps) {
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

            // get the intersection.
            set<int> commonNB;
            set_intersection(neighbor_v.begin(), neighbor_v.end(), neighbor_w.begin(),
                neighbor_w.end(), back_inserter(commonNB));

            // get disjoint common p-neighbor.
            set<int> disjoinNB = disjoinNb(commonNB, vertex, nb);

            // calculate the basic p-structural similarity.
            double similarity = disjoinNB.size() / sqrt(neighbor_v.size() * neighbor_w.size());
            if (similarity >= eps) {
                epsNb.insert(nb);
            }
        }

        this->epsNbs[vertex] = epsNb;
    }
}

set<int> SCAN::disjoinNb(const set<int>& commonNB, int vertexV, int vertexW) {
    set<int> disjoinNB;
    for (int vertexU : commonNB) {
        if (vertexU == vertexV || vertexU == vertexW) {
            continue;
        }
        MyTuple tuple1 = { vertexV, vertexW, metaPath };
        MyTuple tuple2 = { vertexV, vertexU, metaPath };
        MyTuple tuple3 = { vertexW, vertexU, metaPath };
        vector<MyTuple> lambda = { tuple1, tuple2, tuple3 };
        if (verifyExistence(lambda)) {
            disjoinNB.insert(vertexU);
        }
    }

    // union {v, w}.
    disjoinNB.insert(vertexV);
    disjoinNB.insert(vertexW);

    return disjoinNB;
}

// TODO
bool SCAN::verifyExistence(vector<MyTuple> lambda) {
    for (MyTuple tup : lambda) {
        int midIndex = (tup.metaPath.vertex.size() - 1) >> 1;

        // get M(x_i)
        set<int> Mx_i = { tup.vertex1 };
        set<int> temp2;
        for (int i = 1; i <= midIndex; i++) {
            for (int vex : Mx_i) {
                int targetVType = tup.metaPath.vertex.at(i);
                int targetEType = tup.metaPath.edge.at(i - 1);
                vector<int> nbArr = hinGraph.at(tup.vertex1);

                for (int j = 0; j < nbArr.size(); j += 2) {
                    int nbVertexID = nbArr[i];
                    int nbEdgeID = nbArr[i + 1];
                    if (targetVType == vertexType[nbVertexID] && targetEType == edgeType[nbEdgeID]) {
                        temp2.insert(nbVertexID);
                    }
                }
            }

            Mx_i = temp2;
            temp2.clear();
        }

        // get M(y_i)
        set<int> My_i;
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
    getCommonEpsNb(eps);
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

