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
void writeToFile(string filePath, map<int, set<int>>& pnbMap);

// read the homoGraph from file.
map<int, set<int>> readFromFile(string filePath);