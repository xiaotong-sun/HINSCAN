#include "Utility.h"

// write the homoGraph to file.
void writeToFile(string filePath, const unordered_map<int, set<int>>& pnbMap) {
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
unordered_map<int, set<int>> readFromFile(string filePath) {
    unordered_map<int, set<int>> myMap;

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
    unordered_map<int, set<int>> vertexTypeMap = obtainVertexOfType(vertexType);
    unordered_map<int, int> vertexCountMap;
    for (auto& elem : vertexTypeMap.at(metaPath.vertex[0])) {
        vertexCountMap[elem] = 1;
    }

    // estimate from left: A->P->T.
    unordered_map<int, int> leftMap = estimateLeft(graph, vertexType, metaPath, vertexTypeMap, vertexCountMap, flagIndex);


    // estimate from right: T<-P<-A.
    unordered_map<int, int> rightMap = estimateRight(graph, vertexType, metaPath, vertexTypeMap, vertexCountMap, flagIndex);

    // count total join
    int count = 0;
    for (auto& item : leftMap) {
        int key = item.first;
        count += rightMap.at(key) * item.second;
    }
    return count;
}

// Obtain vertices of each type.
unordered_map<int, set<int>> obtainVertexOfType(vector<int>& vertexType) {
    unordered_map<int, set<int>> vertexTypeMap;
    for (int i = 0; i < vertexType.size(); i++) {
        vertexTypeMap[vertexType[i]].insert(i);
    }
    return vertexTypeMap;
}

unordered_map<int, int> estimateLeft(vector<vector<int>>& graph, vector<int>& vertexType, MetaPath& metaPath, unordered_map<int, set<int>>& vertexTypeMap, unordered_map<int, int> vertexCountMap, int flagIndex) {
    for (int i = 1; i <= flagIndex; i++) {
        unordered_map<int, int> tempMap;
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


unordered_map<int, int> estimateRight(vector<vector<int>>& graph, vector<int>& vertexType, MetaPath& metaPath, unordered_map<int, set<int>>& vertexTypeMap, unordered_map<int, int> vertexCountMap, int flagIndex) {
    for (int i = metaPath.pathLen - 1; i >= flagIndex; i--) {
        unordered_map<int, int> tempMap;
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

void writeClusterResultToFile(string filePath, const unordered_map<int, int>& clusterMap) {
    ofstream of(filePath);
    of << "vertexID\tclusterID" << endl;
    for (const auto& iter : clusterMap) {
        of << iter.first << "\t" << iter.second << endl;
    }
    cout << "Finish writing Cluster-Result to " + filePath << endl;
}

long long getTime(struct timeval& start) {
    struct timeval curtime;
    gettimeofday(&curtime, nullptr);
    long long mtime, seconds, useconds;
    seconds = curtime.tv_sec - start.tv_sec;
    useconds = curtime.tv_usec - start.tv_usec;
    mtime = seconds * 1000000 + useconds;
    return mtime;
}

void getVertexNum(const vector<int>& vertexType) {
    unordered_map<int, int> vertexNum;
    for (int type : vertexType) {
        vertexNum[type]++;
    }

    cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
    for (auto& item : vertexNum) {
        cout << item.first << ": " << item.second << "\n";
    }
    cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
}