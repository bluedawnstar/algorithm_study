#include <climits>
#include <numeric>
#include <queue>
#include <stack>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "basicUndirectedGraph.h"
#include "basicUndirectedGraph_Bridge.h"
#include "basicUndirectedGraph_CutVertex.h"

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


static BasicUndirectedGraph buildGraphForEulerExist1() {
    BasicUndirectedGraph graph(5);
    graph.addEdge(1, 0);
    graph.addEdge(0, 2);
    graph.addEdge(2, 1);
    graph.addEdge(0, 3);
    graph.addEdge(3, 4);
    return graph;
}

static BasicUndirectedGraph buildGraphForEulerExist2() {
    BasicUndirectedGraph graph(5);
    graph.addEdge(1, 0);
    graph.addEdge(0, 2);
    graph.addEdge(2, 1);
    graph.addEdge(0, 3);
    graph.addEdge(3, 4);
    graph.addEdge(4, 0);
    return graph;
}

static BasicUndirectedGraph buildGraphForEulerExist3() {
    BasicUndirectedGraph graph(5);
    graph.addEdge(1, 0);
    graph.addEdge(0, 2);
    graph.addEdge(2, 1);
    graph.addEdge(0, 3);
    graph.addEdge(3, 4);
    graph.addEdge(1, 3);
    return graph;
}

static BasicUndirectedGraph buildGraphForEulerPath1() {
    BasicUndirectedGraph graph(4);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    return graph;
}

static BasicUndirectedGraph buildGraphForEulerPath2() {
    BasicUndirectedGraph graph(3);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 0);
    return graph;
}

static BasicUndirectedGraph buildGraphForEulerPath3() {
    BasicUndirectedGraph graph(5);
    graph.addEdge(1, 0);
    graph.addEdge(0, 2);
    graph.addEdge(2, 1);
    graph.addEdge(0, 3);
    graph.addEdge(3, 4);
    graph.addEdge(3, 2);
    graph.addEdge(3, 1);
    graph.addEdge(2, 4);
    return graph;
}

static bool check(vector<pair<int, int>> L, vector<pair<int, int>> R) {
    sort(L.begin(), L.end());
    sort(R.begin(), R.end());
    return L == R;
}

void testBasicUndirectedGraph() {
    //return; //TODO: if you want to test, make this line a comment.

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
        auto g1 = buildGraphForCutVertex1();
        vector<bool> gt1{ true, false, false, true, false };
        UndirectedGraphCutVertex graph1;
        graph1.N = g1.N;
        graph1.edges = g1.edges;
        assert(graph1.findCutVertex() == gt1);

        auto g2 = buildGraphForCutVertex2();
        vector<bool> gt2{ false, true, true, false, false };
        UndirectedGraphCutVertex graph2;
        graph2.N = g2.N;
        graph2.edges = g2.edges;
        assert(graph2.findCutVertex() == gt2);

        auto g3 = buildGraphForCutVertex3();
        vector<bool> gt3{ false, true, false, false, false, false, false };
        UndirectedGraphCutVertex graph3;
        graph3.N = g3.N;
        graph3.edges = g3.edges;
        assert(graph3.findCutVertex() == gt3);
    }
    cout << "* Bridge" << endl;
    {
        auto g1 = buildGraphForCutVertex1();
        vector<pair<int, int>> gt1{ {3, 4}, {0, 3} };
        UndirectedGraphBridge graph1;
        graph1.N = g1.N;
        graph1.edges = g1.edges;
        assert(check(graph1.findBridge(), gt1));

        auto g2 = buildGraphForCutVertex2();
        vector<pair<int, int>> gt2{ {2, 3}, {1, 2}, {0, 1} };
        UndirectedGraphBridge graph2;
        graph2.N = g2.N;
        graph2.edges = g2.edges;
        assert(check(graph2.findBridge(), gt2));

        auto g3 = buildGraphForCutVertex3();
        vector<pair<int, int>> gt3{ {1, 6} };
        UndirectedGraphBridge graph3;
        graph3.N = g3.N;
        graph3.edges = g3.edges;
        assert(check(graph3.findBridge(), gt3));
    }
    cout << "* Eulerian path & circuit (existence)" << endl;
    {
        auto graph1 = buildGraphForEulerExist1();
        assert(graph1.existEulerPath() == true && graph1.existEulerCircuit() == false);

        auto graph2 = buildGraphForEulerExist2();
        assert(graph2.existEulerPath() == true && graph2.existEulerCircuit() == true);

        auto graph3 = buildGraphForEulerExist3();
        assert(graph3.existEulerPath() == false && graph3.existEulerCircuit() == false);
    }
    cout << "* Eulerian path" << endl;
    {
        auto graph1 = buildGraphForEulerPath1();
        auto path1 = graph1.getEulerPath();
        assert(path1.size() == 5 && (path1.front() == 3 && path1.back() == 2 || path1.front() == 2 && path1.back() == 3));

        auto graph2 = buildGraphForEulerPath2();
        auto path2 = graph2.getEulerPath();
        assert(path2.size() == 4 && path2.front() == path2.back());

        auto graph3 = buildGraphForEulerPath3();
        auto path3 = graph3.getEulerPath();
        assert(path3.size() == 9 && (path3.front() == 0 && path3.back() == 1 || path3.front() == 1 && path3.back() == 0));
    }
    cout << "OK" << endl;
}
