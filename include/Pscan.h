/**
 * @file Pscan.h
 * @author Xiaotong Sun
 * @date 2023-11-08
 *
 */

#pragma once
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#include "MetaPath.h"

typedef unsigned int ui;

#define _LINUX_

#ifdef _LINUX_
#include <sys/time.h>
#endif

using namespace std;

class Pscan {
private:
    struct MyTuple {
        int vertex1;
        int vertex2;
        MetaPath metaPath;
    };

    unordered_map<int, set<int>>& homoGraph;
    vector<vector<int>>& hinGraph;
    vector<int>& vertexType;
    vector<int>& edgeType;
    unordered_map<int, int>& edgeReverseMap;
    MetaPath& metaPath;
    set<set<int>> verifyTrueSet;
    set<set<int>> verifyFlaseSet;
    const int mode;

    ui* index2id;
    unordered_map<int, int> id2index;

    ui n, m; //number of nodes and edges of the graph
    int eps_a2, eps_b2, miu; // eps_a2/eps_b2 = eps^2

    ui* pstart; //offset of neighbors of nodes
    int* edges; //adjacent ids of edges
    ui* reverse; //the position of reverse edge in edges
    int* min_cn; //minimum common neighbor: -2 means not similar; -1 means similar; 0 means not sure; > 0 means the minimum common neighbor

    int* pa;
    int* rank; //pa and rank are used for the disjoint-set data structure

    int* cid; //cluster id

    int* degree;
    int* similar_degree; //number of adjacent edges with similarity no less than epsilon
    int* effective_degree; //number of adjacent edges not pruned by similarity

    vector<pair<int, int> > noncore_cluster;

public:
    Pscan(unordered_map<int, set<int>>& homoGraph, vector<vector<int>>& hinGraph, vector<int>& vertexType, vector<int>& edgeType, unordered_map<int, int>& edgeReverseMap, MetaPath& metaPath, int mode);
    ~Pscan();

    void get_graph();
    void pSCAN(const char* eps_s, int miu);
    //eps_s and miu are the parameters (epsilon, miu) for the SCAN algorithm
    void cluster_noncore_vertices(int eps_a2, int eps_b2, int mu);
    void output(const char* eps_s, const char* miu, string dir);
    unordered_map<int, set<int>> getEpsNb();

private:
    FILE* open_file(const char* file_name, const char* mode);
    ui binary_search(const int* array, ui b, ui e, int val);
    //return the first pos, s.t. array[pos] >= val (may return e)
    // int naive_similar_check(int u, int v, int eps_a2, int eps_b2);
    // int similar_check(int u, int v, int eps_a2, int eps_b2);
    int similar_check_OP(int u, ui idx, int eps_a, int eps_b, int mode);
    int check_common_neighbor(int u, int v, int c);
    int check_disjoint_neighbor(int u, int v, int c);
    int compute_common_neighbor_lowerbound(int u, int v, int eps_a2, int eps_b2);
    void prune_and_cross_link(int eps_a2, int eps_b2, int miu, int* cores, int& cores_e);

    int find_root(int u);
    void my_union(int u, int v);

    void get_eps(const char* eps_s);
    int disjoinNb(set<int>& commonNB, int vertexU, int vertexV, auto& verifyTrueSet, auto& verifyFalseSet, int c);
    bool verifyExistence(vector<MyTuple>& lambda);
    bool hasSameValue(const vector<int>& arr, int vertex);
    bool enumeration(const vector<set<int>>& listOfComNb, int index, vector<int>& LArr, vector<MyTuple>& lambda);
    void getNB(set<int>& M_i, set<int>& temp, MyTuple& tup, int index, bool fromRight);
};
