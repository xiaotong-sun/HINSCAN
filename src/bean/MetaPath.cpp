#include "MetaPath.h"

MetaPath::MetaPath() {}

MetaPath::MetaPath(vector<int> vertex, vector<int> edge) {
    this->vertex = vertex;
    this->edge = edge;
    this->pathLen = edge.size(); // the number of relations in a metaPath

    if (vertex.size() != edge.size() + 1) {
        cout << "the meta-path is incorrect" << endl;
    }
}

MetaPath::MetaPath(string metaPathStr) {
    istringstream iss(metaPathStr);
    string st;
    vector<string> sArray;
    while (iss >> st) {
        sArray.push_back(st);
    }

    this->pathLen = sArray.size() / 2;
    vector<int> vertex(pathLen + 1);
    vector<int> edge(pathLen);

    for (int i = 0; i < sArray.size(); i++) {
        stringstream ss;
        int value;
        ss << sArray[i];
        ss >> value;
        if (i % 2 == 0) {
            vertex[i / 2] = value;
        } else {
            edge[i / 2] = value;
        }
    }

    this->vertex = vertex;
    this->edge = edge;
}

MetaPath::MetaPath(const MetaPath& mp) {
    vector<int> vertex(mp.vertex);
    vector<int> edge(mp.edge);
    this->vertex = vertex;
    this->edge = edge;
    this->pathLen = edge.size();
}

string MetaPath::toString() {
    string str;
    for (int i = 0; i < pathLen; i++) {
        str += to_string(vertex[i]) + "-" + to_string(edge[i]) + "-";
    }

    str += to_string(vertex[pathLen]);
    return str;
}

void MetaPath::symmetricPath(int halfLength, unordered_map<int, int> edgeTypeMap) {
    for (int i = 0; i < halfLength; i++) {
        this->edge[halfLength + i] = edgeTypeMap[this->edge[halfLength - i - 1]];
        this->vertex[halfLength + i + 1] = this->vertex[halfLength - i - 1];
    }

    vector<int> edge_(halfLength * 2);
    vector<int> vertex_(halfLength * 2 + 1);

    for (int i = 0; i < halfLength * 2; i++) {
        edge_[i] = this->edge[i];
    }

    for (int i = 0; i < halfLength * 2 + 1; i++) {
        vertex_[i] = this->vertex[i];
    }

    this->edge = edge_;
    this->vertex = vertex_;
    this->pathLen = this->edge.size();
}