/**
 * @file SCAN.h
 * @author Xiaotong Sun
 * @date 2023-10-07
 *
 */

#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <set>
#include <queue>
#include "MetaPath.h"
#include <map>
#include "EffecitveTest.h"

using namespace std;

class SCAN {
public:
    struct MyTuple {
        int vertex1;
        int vertex2;
        MetaPath metaPath;
    };

    // store the homogeneous graph.
    unordered_map<int, set<int>> homoGraph;

    // store the HIN graph.
    map<int, vector<int>> hinGraph;

    // store the type of each vertex.
    vector<int> vertexType;

    // store the type of each edge.
    vector<int> edgeType;

    // store the edge reverse map;
    unordered_map<int, int> edgeReverseMap;

    // store the meta-path.
    MetaPath metaPath;

    // store the pathsimMap
    unordered_map<int, unordered_map<int, int>> psimMap;

    // record the clusterID of each vertex. unclassified=0, non-memeber=-1, cluster>=1, hub=-2, outlier=-3.
    map<int, int> clusterMap;

    // record the eps-Neighborhood for each vertex.
    unordered_map<int, set<int>> epsNbs;

    // Two sets bellow are used to record the u, v, w that has been verified.
    set<set<int>> verifyTrueSet;
    set<set<int>> verifyFalseSet;

    int verifyTimes = 0;

    SCAN(const unordered_map<int, set<int>>& homoGraph, const map<int, vector<int>>& hinGraph, const vector<int> vertexType, const vector<int>& edgeType, const unordered_map<int, int>& edgeReverseMap, const MetaPath& metaPath);

    // get cluster, hub, outlier. CommonNb: mode=0; DisjointNb: mode=1;
    void getCluster(double eps, int mu, int mode);

    void showVerifyTimes();

    unordered_map<int, set<int>> getCommunities();

private:
    // check whether all vertices in Neighbor(v) belong to the same cluster or not.
    bool isSameCluster(int vertex);

    // get eps-Neighborhood (common p-neighbor).
    void getCommonEpsNb(double eps);

    void getCommonEpsNbUsingPathsim(double eps);

    // get eps-Neighborhood (disjoint common p-neighbor).
    void getDisjointEpsNb(double eps);

    // get disjoint common p-enighbor (used for disjoint common p-neighbor).
    set<int> disjoinNb(set<int>& commonNB, int vertexV, int vertexW, auto& verifyTrueSet, auto& verifyFalseSet, double lowerBound);

    // verify if there exist three instances of P connecting three P-pairs (used for disjoint common p-neighbor).
    bool verifyExistence(vector<MyTuple>& lambda);

    // get the common neighbor (Mx_i / My_i; used for disjoint common p-neighbor).
    void getNB(set<int>& M_i, set<int>& temp, MyTuple& tup, int index, bool fromRight);

    // check whether the vertex is repeat (used for disjoint common p-neighbor).
    bool hasSameValue(const vector<int>& arr, int vertex);

    // enumerate, to get different instance (used for disjoint common p-neighbor).
    bool enumeration(const vector<set<int>>& listOfComNb, int index, vector<int>& LArr, vector<MyTuple>& lambda);

    // check whether this vertex is a core or not.
    bool isCore(double eps, int mu, int vertex);
};