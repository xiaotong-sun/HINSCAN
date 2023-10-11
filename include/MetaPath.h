/**
 * @file MetaPath.h
 * @author Xiaotong Sun
 * @date 2023-10-07
 *
 */

#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>

using namespace std;

class MetaPath {
public:
    vector<int> vertex; // record the vertex type of a meta path.
    vector<int> edge; // record the edge type of a meta path.
    int pathLen = -1;

    MetaPath();

    // create metaPath using array.
    MetaPath(vector<int> vertex, vector<int> edge);

    // create metaPath using string.
    MetaPath(string metaPathStr);

    // use a MetaPath object's value to create a new one.
    MetaPath(const MetaPath& mp);

    // get the metaPath string.
    string toString();

    // get symmetric meta path by given half meta path.
    void symmetricPath(int halfLength, const unordered_map<int, int>& edgeTypeMap);
};