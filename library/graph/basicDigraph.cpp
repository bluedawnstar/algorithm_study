#include <climits>
#include <numeric>
#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

#include "basicDigraph.h"
#include "cycleDigraph.h"

  /////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

static BasicDigraph buildDigraph(bool isCycle) {
    BasicDigraph graph(6);

    graph.addEdge(5, 2);
    graph.addEdge(5, 0);
    graph.addEdge(4, 0);
    graph.addEdge(4, 1);
    graph.addEdge(2, 3);
    graph.addEdge(3, 1);

    if (isCycle)
        graph.addEdge(1, 2);

    return graph;
}

static BasicDigraph buildSCC1() {
    BasicDigraph graph(5);
    graph.addEdge(1, 0);
    graph.addEdge(0, 2);
    graph.addEdge(2, 1);
    graph.addEdge(0, 3);
    graph.addEdge(3, 4);
    return graph;
}

static BasicDigraph buildSCC2() {
    BasicDigraph graph(4);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    return graph;
}

static BasicDigraph buildSCC3() {
    BasicDigraph graph(7);
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

static BasicDigraph buildSCGraph1() {
    BasicDigraph graph(5);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 0);
    graph.addEdge(2, 4);
    graph.addEdge(4, 2);
    return graph;
}

static BasicDigraph buildSCGraph2() {
    BasicDigraph graph(5);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    return graph;
}

static bool check(vector<vector<int>>& L, vector<vector<int>>& R) {
    for (auto& v : L)
        sort(v.begin(), v.end());
    for (auto& v : R)
        sort(v.begin(), v.end());
    sort(L.begin(), L.end());
    sort(R.begin(), R.end());
    return L == R;
}

void testBasicDigraph() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Basic Digraph ---------" << endl;
    cout << "* Shortest path" << endl;
    {
        BasicDigraph graph(7);

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
        auto graph = buildDigraph(false);
        assert(graph.isCyclicGraphDFS() == false);

        graph = buildDigraph(true);
        assert(graph.isCyclicGraphDFS() == true);
    }
    cout << "* Strongly contected components" << endl;
    {
        auto graph1 = buildSCC1();
        auto gt1 = vector<vector<int>>{ { 4 }, { 3 }, { 1, 2, 0 } };
        assert(check(graph1.findSCC(), gt1));

        auto graph2 = buildSCC2();
        auto gt2 = vector<vector<int>>{ { 3 },{ 2 },{ 1 }, { 0 } };
        assert(check(graph2.findSCC(), gt2));

        auto graph3 = buildSCC3();
        auto gt3 = vector<vector<int>>{ { 5 },{ 3 },{ 4 },{ 6 }, { 2, 1, 0 } };
        assert(check(graph3.findSCC(), gt3));
    }
    cout << "* Strongly contected graph" << endl;
    {
        auto graph1 = buildSCGraph1();
        assert(graph1.isSCGraph() == true);

        auto graph2 = buildSCGraph2();
        assert(graph2.isSCGraph() == false);
    }
    cout << "OK" << endl;
}
