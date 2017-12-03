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
    graph.addEdge(5, 2);
    graph.addEdge(5, 0);
    graph.addEdge(4, 0);
    graph.addEdge(4, 1);
    graph.addEdge(2, 3);
    graph.addEdge(3, 1);
    return graph;
}

void testDAG() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- TopologicalSort -------------" << endl;
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
    cout << "OK" << endl;
}
