#include "Utility.h"

// write the homoGraph to file.
void writeToFile(string filePath, map<int, set<int>>& pnbMap) {
    ofstream of(filePath);
    for (const auto& iter : pnbMap) {
        of << iter.first;
        for (auto& elem : iter.second) {
            of << " " << elem;
        }
        of << endl;
    }
    cout << "Finish writing to file!" << endl;
}

// read the homoGraph from file.
map<int, set<int>> readFromFile(string filePath) {
    map<int, set<int>> myMap;

    ifstream fs(filePath);
    string line;
    while (getline(fs, line)) {
        istringstream iss(line);
        int key;
        iss >> key;
        int value;
        set<int> temp;
        while (iss >> value) {
            temp.insert(value);
        }
        myMap[key] = temp;
    }

    return myMap;
}

// roughly evaluation the number of total join.
int estimate(vector<vector<int>>& graph, vector<int>& vertexType, MetaPath& metaPath, int flagIndex) {
    map<int, set<int>> vertexTypeMap = obtainVertexOfType(vertexType);
    map<int, int> vertexCountMap;
    for (auto& elem : vertexTypeMap.at(metaPath.vertex[0])) {
        vertexCountMap[elem] = 1;
    }

    // estimate from left: A->P->T.
    map<int, int> leftMap = estimateLeft(graph, vertexType, metaPath, vertexTypeMap, vertexCountMap, flagIndex);


    // estimate from right: T<-P<-A.
    map<int, int> rightMap = estimateRight(graph, vertexType, metaPath, vertexTypeMap, vertexCountMap, flagIndex);

    // count total join
    int count = 0;
    for (auto& item : leftMap) {
        int key = item.first;
        count += rightMap.at(key) * item.second;
    }
    return count;
}

// Obtain vertices of each type.
map<int, set<int>> obtainVertexOfType(vector<int>& vertexType) {
    map<int, set<int>> vertexTypeMap;
    for (int i = 0; i < vertexType.size(); i++) {
        vertexTypeMap[vertexType[i]].insert(i);
    }
    return vertexTypeMap;
}

map<int, int> estimateLeft(vector<vector<int>>& graph, vector<int>& vertexType, MetaPath& metaPath, map<int, set<int>>& vertexTypeMap, map<int, int> vertexCountMap, int flagIndex) {
    for (int i = 1; i <= flagIndex; i++) {
        map<int, int> tempMap;
        int firstVertexType = metaPath.vertex[i - 1];
        int secondVertexType = metaPath.vertex[i];
        for (int elem : vertexTypeMap.at(secondVertexType)) {
            vector<int> nbArr = graph.at(elem);
            int count = 0;
            for (int i = 0; i < nbArr.size(); i += 2) {
                int nbVertexID = nbArr[i];
                if (firstVertexType == vertexType[nbVertexID]) {
                    count += vertexCountMap.at(nbVertexID);
                }
            }
            tempMap[elem] = count;
        }
        vertexCountMap = tempMap;
    }
    return vertexCountMap;
}


map<int, int> estimateRight(vector<vector<int>>& graph, vector<int>& vertexType, MetaPath& metaPath, map<int, set<int>>& vertexTypeMap, map<int, int> vertexCountMap, int flagIndex) {
    for (int i = metaPath.pathLen - 1; i >= flagIndex; i--) {
        map<int, int> tempMap;
        int firstVertexType = metaPath.vertex[i + 1];
        int secondVertexType = metaPath.vertex[i];
        for (int elem : vertexTypeMap.at(secondVertexType)) {
            vector<int> nbArr = graph.at(elem);
            int count = 0;
            for (int i = 0; i < nbArr.size(); i += 2) {
                int nbVertexID = nbArr[i];
                if (firstVertexType == vertexType[nbVertexID]) {
                    count += vertexCountMap.at(nbVertexID);
                }
            }
            tempMap[elem] = count;
        }
        vertexCountMap = tempMap;
    }
    return vertexCountMap;
}