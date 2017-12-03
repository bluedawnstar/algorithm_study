#include <climits>
#include <numeric>
#include <queue>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;

#include "minimumSpanningTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

static bool isEqual(vector<pair<int, int>>& v1, vector<pair<int, int>>& v2) {
    for (auto& it : v1) {
        if (it.first > it.second)
            swap(it.first, it.second);
    }
    for (auto& it : v2) {
        if (it.first > it.second)
            swap(it.first, it.second);
    }
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    return v1 == v2;
}

static vector<pair<int, int>> makeEdgeList(vector<int>& parent) {
    vector<pair<int, int>> res;
    for (int i = 0; i < (int)parent.size(); i++)
        if (parent[i] >= 0)
            res.emplace_back(i, parent[i]);
    return res;
}

void testMinimumSpanningTree() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Minimum Spanning Tree ---------" << endl;
    {
        MinimumSpanningTree<int> graph(5);
        graph.addEdge(0, 1, 2);
        graph.addEdge(0, 3, 6);
        graph.addEdge(1, 2, 3);
        graph.addEdge(2, 4, 7);
        graph.addEdge(3, 4, 9);

        vector<pair<int, int>> selected1;
        vector<int> parent;
        auto ans1 = graph.kruskal(selected1);
        auto ans2 = graph.prim(parent);
        auto selected2 = makeEdgeList(parent);
        cout << "cost = " << ans1 << ", selected edges = " << selected1 << endl;
        cout << "cost = " << ans2 << ", selected edges = " << selected2 << endl;
        assert(ans1 == 18);
        assert(ans2 == 18);
        assert(isEqual(selected1, selected2));
    }
    {
        MinimumSpanningTree<int> graph(4);
        graph.addEdge(0, 1, 10);
        graph.addEdge(0, 2,  6);
        graph.addEdge(0, 3,  5);
        graph.addEdge(1, 3, 15);
        graph.addEdge(2, 3,  4);

        vector<pair<int, int>> selected1;
        vector<int> parent;
        auto ans1 = graph.kruskal(selected1);
        auto ans2 = graph.prim(parent);
        auto selected2 = makeEdgeList(parent);
        cout << "cost = " << ans1 << ", selected edges = " << selected1 << endl;
        cout << "cost = " << ans2 << ", selected edges = " << selected2 << endl;
        assert(ans1 == 19);
        assert(ans2 == 19);
        assert(isEqual(selected1, selected2));
    }
    cout << "OK" << endl;
}
