/**
 * @file Utility.h
 * @author Xiaotong Sun
 * @date 2023-11-09
 *
 */

#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
using namespace std;

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