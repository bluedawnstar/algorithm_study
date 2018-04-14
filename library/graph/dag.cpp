#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

#include "dag.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

static DAG<int> buildGraph() {
    DAG<int> graph(6);
    graph.addEdge(5, 2, 0);
    graph.addEdge(5, 0, 0);
    graph.addEdge(4, 0, 0);
    graph.addEdge(4, 1, 0);
    graph.addEdge(2, 3, 0);
    graph.addEdge(3, 1, 0);
    return graph;
}

static DAG<int> buildGraphForShortestPath() {
    DAG<int> graph(6);
    graph.addEdge(0, 1, 5);
    graph.addEdge(0, 2, 3);
    graph.addEdge(1, 3, 6);
    graph.addEdge(1, 2, 2);
    graph.addEdge(2, 4, 4);
    graph.addEdge(2, 5, 2);
    graph.addEdge(2, 3, 7);
    graph.addEdge(3, 4, -1);
    graph.addEdge(4, 5, -2);
    return graph;
}

void testDAG() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Directed Acyclic Graph -------------" << endl;
    {
        auto graph = buildGraph();
        vector<int> res;

        graph.topologicalSortDFS(res);
        auto gtDFS = vector<int>{ 5, 4, 2, 3, 1, 0 };
        cout << "topological sort with DFS : " << res << endl;
        assert(res == gtDFS);

        graph.topologicalSortBFS(res);
        auto gtBFS = vector<int>{ 4, 5, 2, 0, 3, 1 };
        cout << "topological sort with BFS : " << res << endl;
        assert(res == gtBFS);
    }
    {
        auto graph = buildGraphForShortestPath();
        vector<int> res;

        auto gt = vector<int>{ DAG<int>::INF, 0, 2, 6, 5, 3 };
        auto ans = graph.findShortestPath(1);
        assert(ans.first == gt);
    }
    cout << "OK" << endl;
}
