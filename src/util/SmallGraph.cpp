#include "SmallGraph.h"

SmallGraph::SmallGraph(const map<int, vector<int>>& graph, const vector<int>& vertexType, const vector<int>& edgeType) {
    this->graph = graph;
    this->vertexType = vertexType;
    this->edgeType = edgeType;
}

void SmallGraph::getNewVidMap(int part, int total) {
    int smallGraphSize = (graph.size() * part) / total;
    int newVid = 0;

    for (auto iter = graph.begin(); iter != graph.end();) {
        for (int i = 0; i < part; i++) {
            int oldVid = iter->first;
            if (!newVidMap.contains(oldVid)) {
                newVidMap[oldVid] = newVid;
                oldVidMap[newVid] = oldVid;
                newVid++;
                if (newVidMap.size() >= smallGraphSize) {
                    return;
                }
            }
            iter++;
        }

        for (int i = 0; i < total - part && iter != graph.end(); i++) {
            iter++;
        }
    }
}

void SmallGraph::getSmallGraphVertexType() {
    smallGraphVertexType.resize(newVidMap.size());
    for (auto iter = newVidMap.begin(); iter != newVidMap.end(); iter++) {
        int oldVid = iter->first;
        int newVid = iter->second;
        smallGraphVertexType[newVid] = vertexType[oldVid];
    }
}

void SmallGraph::getSmallGraph(int part, int total) {
    getNewVidMap(part, total);
    getSmallGraphVertexType();
    int newEid = 0;
    for (auto iter = newVidMap.begin(); iter != newVidMap.end(); iter++) {
        int oldVid = iter->first;
        int newVid = iter->second;
        int numOfNeighbor = 0;
        for (int i = 0; i < graph[oldVid].size(); i = i + 2) {
            if (newVidMap.contains(graph[oldVid][i])) {
                numOfNeighbor++;
            }
        }

        smallGraph[newVid].resize(2 * numOfNeighbor);
        int location = 0;
        for (int i = 0; i < graph[oldVid].size(); i += 2) {
            if (newVidMap.contains(graph[oldVid][i])) {
                int neighborNid = newVidMap[graph[oldVid][i]];
                int neighborEid = graph[oldVid][i + 1];
                smallGraph[newVid][location] = neighborNid;
                smallGraph[newVid][location + 1] = newEid;
                newEidMap[neighborEid] = newEid;
                newEid++;
                location += 2;
            }
        }

        // for (int i = 0; i < graph[oldVid].size(); i += 2) {
        //     if (newVidMap.contains(graph[oldVid][i])) {
        //         int neighborNid = newVidMap[graph[oldVid][i]];
        //         int neighborEid = graph[oldVid][i + 1];
        //         smallGraph[newVid].push_back(neighborNid);
        //         smallGraph[newVid].push_back(newEid);
        //         newEidMap[neighborEid] = newEid;
        //         newEid++;
        //     }
        // }
    }
    smallGraphEdgeType.resize(newEid);
    for (int i = 0; i < edgeType.size(); i++) {
        int type = edgeType[i];
        if (newEidMap.contains(i)) {
            int newEdgeId = newEidMap[i];
            smallGraphEdgeType[newEdgeId] = type;
        }
    }
}

void SmallGraph::setBigGraph() {
    this->smallGraph = this->graph;
    this->smallGraphVertexType = this->vertexType;
    this->smallGraphEdgeType = this->edgeType;
}