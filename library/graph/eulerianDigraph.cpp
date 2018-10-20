#include <climits>
#include <numeric>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "eulerianDigraph.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

static EulerianDigraph buildEulerian() {
    EulerianDigraph graph(5);
    graph.addEdge(1, 0);
    graph.addEdge(0, 2);
    graph.addEdge(2, 1);
    graph.addEdge(0, 3);
    graph.addEdge(3, 4);
    graph.addEdge(4, 0);
    return graph;
}

static EulerianDigraph buildEulerian2() {
    EulerianDigraph graph(5);
    graph.addEdge(4, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 3);
    graph.addEdge(1, 0);
    graph.addEdge(2, 4);
    graph.addEdge(3, 4);
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

void testEulerianDigraph() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Eulerian in Digraph ---------" << endl;
    {
        auto graph1 = buildEulerian();
        assert(graph1.existEulerPathNaive() == true);
        assert(graph1.existEulerCircuitNaive() == true);
        assert(graph1.existEulerPath() == true);
        assert(graph1.existEulerCircuit() == true);

        auto circuit = graph1.getEulerCircuit();
        assert(!circuit.empty());
    }
    {
        auto graph1 = buildEulerian();
        auto path = graph1.getEulerPath();
        assert(path.size() == 7 && path.front() == path.back() && count(path.begin(), path.end(), 0) == 3);

        auto circuit = graph1.getEulerCircuit();
        assert(!circuit.empty());
    }
    {
        auto graph2 = buildEulerian2();
        auto path = graph2.getEulerPath();
        assert(path.size() == 7 && (path.front() == 0 && path.back() == 4 || path.front() == 4 && path.back() == 0));

        auto circuit = graph2.getEulerCircuit();
        assert(circuit.empty());
    }
    cout << "OK" << endl;
}
