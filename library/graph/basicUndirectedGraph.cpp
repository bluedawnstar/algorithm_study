#include <climits>
#include <numeric>
#include <queue>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;

#include "basicUndirectedGraph.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

static BasicUndirectedGraph buildUndirectedGraph(bool isCycle) {
    BasicUndirectedGraph graph(6);
    graph.addEdge(5, 2);
    graph.addEdge(4, 0);
    graph.addEdge(4, 1);
    graph.addEdge(2, 3);
    graph.addEdge(3, 1);
    if (isCycle)
        graph.addEdge(5, 0);
    return graph;
}

static BasicUndirectedGraph buildGraphForCutVertex1() {
    BasicUndirectedGraph graph(5);
    graph.addEdge(1, 0);
    graph.addEdge(0, 2);
    graph.addEdge(2, 1);
    graph.addEdge(0, 3);
    graph.addEdge(3, 4);
    return graph;
}

static BasicUndirectedGraph buildGraphForCutVertex2() {
    BasicUndirectedGraph graph(5);
    graph.addEdge(1, 0);
    graph.addEdge(2, 1);
    graph.addEdge(2, 3);
    return graph;
}

static BasicUndirectedGraph buildGraphForCutVertex3() {
    BasicUndirectedGraph graph(7);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 0);
    graph.addEdge(1, 3);
    graph.addEdge(1, 4);
    graph.addEdge(1, 6);
    graph.addEdge(3, 5);
    graph.addEdge(4, 5);
    return graph;
}

static bool check(vector<pair<int, int>>& L, vector<pair<int, int>>& R) {
    sort(L.begin(), L.end());
    sort(R.begin(), R.end());
    return L == R;
}

void testBasicUndirectedGraph() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Basic Undirected Graph ---------" << endl;
    cout << "* Shortest path" << endl;
    {
        BasicUndirectedGraph graph(7);

        graph.addEdge(0, 1);
        graph.addEdge(1, 2);
        graph.addEdge(1, 3);
        graph.addEdge(2, 3);
        graph.addEdge(3, 4);
        graph.addEdge(4, 6);
        graph.addEdge(5, 6);
        cout << graph.getShortestPath(0, 6) << endl;
    }
    cout << "* Cycle" << endl;
    {
        auto graph = buildUndirectedGraph(false);
        assert(graph.isCyclicGraphDFS() == false);
        assert(graph.isCyclicGraphWithUnionFind() == false);

        graph = buildUndirectedGraph(true);
        assert(graph.isCyclicGraphDFS() == true);
        assert(graph.isCyclicGraphWithUnionFind() == true);
    }
    cout << "* Articulation point" << endl;
    {
        auto graph1 = buildGraphForCutVertex1();
        vector<bool> gt1{ true, false, false, true, false };
        assert(graph1.findCutVertex() == gt1);

        auto graph2 = buildGraphForCutVertex2();
        vector<bool> gt2{ false, true, true, false, false };
        assert(graph2.findCutVertex() == gt2);

        auto graph3 = buildGraphForCutVertex3();
        vector<bool> gt3{ false, true, false, false, false, false, false };
        assert(graph3.findCutVertex() == gt3);
    }
    cout << "* Bridge" << endl;
    {
        auto graph1 = buildGraphForCutVertex1();
        vector<pair<int, int>> gt1{ {3, 4}, {0, 3} };
        assert(check(graph1.findBridge(), gt1));

        auto graph2 = buildGraphForCutVertex2();
        vector<pair<int, int>> gt2{ {2, 3}, {1, 2}, {0, 1} };
        assert(check(graph2.findBridge(), gt2));

        auto graph3 = buildGraphForCutVertex3();
        vector<pair<int, int>> gt3{ {1, 6} };
        assert(check(graph3.findBridge(), gt3));
    }
    cout << "OK" << endl;
}
