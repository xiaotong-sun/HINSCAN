#include "MetaPath.h"
#include <map>
#include <set>

using namespace std;

class HomoGraphBuilder {
private:
    vector<vector<int>> graph; // data graph, including vertex IDs, edge IDs, and their link relationships.
    vector<int> vertexType; // vertex -> type
    vector<int> edgeType; // edge -> type
    MetaPath queryMPath; // the query meta-path

public:
    HomoGraphBuilder(vector<vector<int>> graph, vector<int> vertexType, vector<int> edgeType, MetaPath queryMPath);

    map<int, vector<int>> build();

    void findAllNeighbors(int startID, int curID, int index, vector<set<int>> visitList, set<int> pnbSet);
};