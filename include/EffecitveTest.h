/**
 * @file EffecitveTest.h
 * @author Xiaotong Sun
 * @date 2024-10-16
 *
 */

#pragma once
#include <unordered_map>
#include <map>
#include <set>
#include "MetaPath.h"
#include <vector>

using namespace std;

class EffectiveTest {
private:
    const map<int, vector<int>>& hinGraph;
    const vector<int>& vertexType;
    const vector<int>& edgeType;
    const MetaPath& metaPath;
    const unordered_map<int, set<int>> communities;

    unordered_map<int, unordered_map<int, int>> psimMap;

public:
    EffectiveTest(const map<int, vector<int>>& hinGraph, const vector<int>& vertexType, const vector<int>& edgeType, const MetaPath& metaPath);

    EffectiveTest(const map<int, vector<int>>& hinGraph, const vector<int>& vertexType, const vector<int>& edgeType, const MetaPath& metaPath, const unordered_map<int, set<int>>& communities);

    void process();

    unordered_map<int, unordered_map<int, int>> getpsimMap();

private:
    void pathsimTest();
    void pathcountTest();
    void densityTest();
    void pcrwTest();
    void avgVertexNumTest();
    unordered_map<int, unordered_map<int, int>> batchBuildForPathSim(set<int> keepSet);
    double avgPathSim(set<int> communitySet);
};