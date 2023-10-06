#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>

using namespace std;

class MetaPath {
public:
    vector<int> vertex;
    vector<int> edge;
    int pathLen = -1;

    MetaPath();

    MetaPath(vector<int> vertex, vector<int> edge); // create metaPath using array

    MetaPath(string metaPathStr); // create metaPath using string

    // use a MetaPath object's value to create a new one.
    MetaPath(const MetaPath& mp);

    string toString();

    vector<int> getVertex();

    vector<int> getEdge();

    // get symmetric meta path by given half meta path
    void symmetricPath(int halfLength, unordered_map<int, int> edgeTypeMap);
};