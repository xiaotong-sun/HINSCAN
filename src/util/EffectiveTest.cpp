/**
 * @file EffectiveTest.cpp
 * @author Xiaotong Sun
 * @date 2024-10-16
 *
 */

#include "EffecitveTest.h"

EffectiveTest::EffectiveTest(const map<int, vector<int>>& hinGraph, const vector<int>& vertexType, const vector<int>& edgeType, const MetaPath& metaPath) : hinGraph(hinGraph), vertexType(vertexType), edgeType(edgeType), metaPath(metaPath) {
    set<int> keepSet;
    for (int i = 0; i < vertexType.size(); i++) {
        if (vertexType[i] == metaPath.vertex.at(0)) {
            keepSet.insert(i);
        }
    }

    psimMap = batchBuildForPathSim(keepSet);
}

EffectiveTest::EffectiveTest(const map<int, vector<int>>& hinGraph, const vector<int>& vertexType, const vector<int>& edgeType, const MetaPath& metaPath, const unordered_map<int, set<int>>& communities) : hinGraph(hinGraph), vertexType(vertexType), edgeType(edgeType), metaPath(metaPath), communities(communities) {
    set<int> keepSet;
    for (int i = 0; i < vertexType.size(); i++) {
        if (vertexType[i] == metaPath.vertex.at(0)) {
            keepSet.insert(i);
        }
    }

    psimMap = batchBuildForPathSim(keepSet);
}

void EffectiveTest::process() {
    pathcountTest();
    pathsimTest();
    pcrwTest();
    densityTest();
    avgVertexNumTest();
}

unordered_map<int, unordered_map<int, int>> EffectiveTest::getpsimMap() {
    return this->psimMap;
}

void EffectiveTest::pathsimTest() {
    int count = communities.size();
    double pathsimSum = 0;
    for (auto& iter : communities) {
        pathsimSum += avgPathSim(iter.second);
    }
    cout << "pathSim = " << pathsimSum / count << endl;
}

void EffectiveTest::pcrwTest() {
    int count = communities.size();
    double avgPCRW = 0;
    double pcrwSum = 0;
    for (auto& iter : communities) {
        set<int> communitySet = iter.second;
        for (int vid : communitySet) {
            unordered_map<int, int> nbMap = psimMap.at(vid);
            int allPath = 0;
            for (auto& iter : nbMap) {
                int nbVid = iter.first;
                allPath += psimMap.at(vid).at(nbVid);
            }

            for (auto& iter : nbMap) {
                int nbVid = iter.first;
                if (!communitySet.contains(nbVid)) continue;
                if (nbVid <= vid) continue;
                pcrwSum += (double)psimMap.at(vid).at(nbVid) / (allPath * communitySet.size());
            }
        }
    }

    avgPCRW = (double)pcrwSum / count;
    cout << "avgPCRW = " << avgPCRW << endl;
}

void EffectiveTest::avgVertexNumTest() {
    int count = communities.size();
    int totalVertex = 0;
    for (auto& iter : communities) {
        totalVertex += iter.second.size();
    }

    cout << "avgCommunitySize = " << (double)totalVertex / count << endl;
}

void EffectiveTest::pathcountTest() {
    int count = communities.size();
    double avgPathcount = 0;
    int pathcountSum = 0;
    for (auto& iter : communities) {
        set<int> communitySet = iter.second;
        for (int vid : communitySet) {
            unordered_map<int, int> nbMap = psimMap.at(vid);
            for (auto& iter : nbMap) {
                int nbVid = iter.first;
                if (!communitySet.contains(nbVid)) continue;
                if (nbVid <= vid) continue;
                pathcountSum += psimMap.at(vid).at(nbVid);
            }
        }
    }

    avgPathcount = (double)pathcountSum / count;
    cout << "community num = " << count << endl;
    cout << "avgPathCount = " << avgPathcount << " totalPathCount = " << pathcountSum << endl;
}

void EffectiveTest::densityTest() {
    int count = communities.size();
    double densitySum = 0;
    double avgDensity = 0;
    for (auto& iter : communities) {
        set<int> communitySet = iter.second;
        int pathNum = 0;
        for (int vid : communitySet) {
            unordered_map<int, int> nbMap = psimMap.at(vid);
            for (auto& iter : nbMap) {
                int nbVid = iter.first;
                if (!communitySet.contains(nbVid)) continue;
                if (nbVid <= vid) continue;
                pathNum += psimMap.at(vid).at(nbVid);
            }
        }
        densitySum += (double)pathNum / communitySet.size();
    }

    avgDensity = densitySum / count;

    cout << "density = " << avgDensity << endl;
}



unordered_map<int, unordered_map<int, int>> EffectiveTest::batchBuildForPathSim(set<int> keepSet)
{
    unordered_map<int, unordered_map<int, int>> vertexNbMap;
    for (int startId : keepSet) {
        unordered_map<int, int> anchorMap;
        anchorMap[startId] = 1;
        for (int layer = 0; layer < this->metaPath.pathLen; layer++) {
            int targetVType = metaPath.vertex.at(layer + 1);
            int targetEType = metaPath.edge.at(layer);
            unordered_map<int, int> nextAnchorMap;
            for (auto iter = anchorMap.begin(); iter != anchorMap.end(); iter++) {
                int anchorId = iter->first;
                int anchorPNum = iter->second;
                vector<int> nb = this->hinGraph.at(anchorId);
                for (int i = 0; i < nb.size(); i += 2) {
                    int nbVertexId = nb[i], nbEdgeId = nb[i + 1];
                    if (targetVType == vertexType[nbVertexId] && targetEType == edgeType[nbEdgeId]) {
                        if (layer < metaPath.pathLen - 1) {
                            if (!nextAnchorMap.contains(nbVertexId)) nextAnchorMap[nbVertexId] = 0;
                            int curPNum = nextAnchorMap.at(nbVertexId);
                            nextAnchorMap.at(nbVertexId) = anchorPNum + curPNum;
                        } else {
                            if (keepSet.contains(nbVertexId)) {
                                if (!nextAnchorMap.contains(nbVertexId)) nextAnchorMap[nbVertexId] = 0;
                                int curPNum = nextAnchorMap.at(nbVertexId);
                                nextAnchorMap.at(nbVertexId) = anchorPNum + curPNum;
                            }
                        }
                    }
                }
            }
            anchorMap = nextAnchorMap;
        }
        vertexNbMap[startId] = anchorMap;
    }
    return vertexNbMap;
}

double EffectiveTest::avgPathSim(set<int> communitySet)
{
    double eNum = 0;
    double cSim = 0;
    for (int vid : communitySet) {
        unordered_map<int, int> nbMap = psimMap.at(vid);
        for (auto& iter : nbMap) {
            int nbVid = iter.first;
            if (!communitySet.contains(nbVid)) continue;
            if (nbVid <= vid) continue;
            cSim += ((double)psimMap.at(vid).at(nbVid) + (double)psimMap.at(vid).at(nbVid)) / ((double)psimMap.at(vid).at(vid) + (double)psimMap.at(nbVid).at(nbVid));
        }

    }
    eNum = (double)communitySet.size() * (double)(communitySet.size() - 1) / 2;
    cSim = cSim / eNum;
    return cSim;
}