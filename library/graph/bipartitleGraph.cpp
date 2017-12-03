#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

#include "bipartiteGraph.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

static BipartiteGraphArray<int> buildGraphArray() {
    BipartiteGraphArray<int> graph(6, 6);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 0);
    graph.addEdge(1, 3);
    graph.addEdge(2, 2);
    graph.addEdge(3, 2);
    graph.addEdge(3, 3);
    graph.addEdge(5, 5);
    return graph;
}

static BipartiteGraph<int> buildGraph() {
    BipartiteGraph<int> graph(6, 6);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 0);
    graph.addEdge(1, 3);
    graph.addEdge(2, 2);
    graph.addEdge(3, 2);
    graph.addEdge(3, 3);
    graph.addEdge(5, 5);
    return graph;
}

void testBipartiteMaching() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Bipartite Maching -------------" << endl;
    {
        auto graph = buildGraphArray();

        int ans = graph.bipartiteMatching();
        cout << "bipartite matchin : " << ans << endl;
        assert(ans == 5);

        cout << graph.getLastBipartiteMatchingEdges() << endl;

    }
    {
        auto graph = buildGraph();

        int ans = graph.bipartiteMatching();
        cout << "bipartite matchin : " << ans << endl;
        assert(ans == 5);

        cout << graph.getLastBipartiteMatchingEdges() << endl;

    }
    cout << "OK" << endl;
}
