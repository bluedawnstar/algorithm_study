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


void testBasicUndirectedGraph() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Basic Undirected Graph ---------" << endl;
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
    {
        auto graph = buildUndirectedGraph(false);
        assert(graph.isCyclicGraphDFS() == false);
        assert(graph.isCyclicGraphWithUnionFind() == false);

        graph = buildUndirectedGraph(true);
        assert(graph.isCyclicGraphDFS() == true);
        assert(graph.isCyclicGraphWithUnionFind() == true);
    }
    cout << "OK" << endl;
}
