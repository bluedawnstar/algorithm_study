#include <queue>
#include <climits>
#include <algorithm>
#include <vector>

using namespace std;

#include "shortestPathOneSource.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

static ShortestPath<int> buildGraph() {
    ShortestPath<int> graph(9);
    graph.addBiEdge(0, 1, 4);
    graph.addBiEdge(0, 7, 8);
    graph.addBiEdge(1, 2, 8);
    graph.addBiEdge(1, 7, 11);
    graph.addBiEdge(2, 3, 7);
    graph.addBiEdge(2, 5, 4);
    graph.addBiEdge(2, 8, 2);
    graph.addBiEdge(3, 4, 9);
    graph.addBiEdge(3, 5, 14);
    graph.addBiEdge(4, 5, 10);
    graph.addBiEdge(5, 6, 2);
    graph.addBiEdge(6, 7, 1);
    graph.addBiEdge(6, 8, 6);
    graph.addBiEdge(7, 8, 7);
    return graph;
}

void testShortestPath() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Shortest Path ---------" << endl;
    auto graph = buildGraph();

    graph.dijkstra(0);
    vector<int> resDijkstra(graph.dist);
    vector<int> resDijkstraPath = graph.getShortestPath(7);
    cout << resDijkstra << endl;

    graph.bellmanFord(0);
    vector<int> resBellmanFord(graph.dist);
    vector<int> resBellmanFordPath = graph.getShortestPath(7);
    cout << resBellmanFord << endl;

    assert(resDijkstra == resBellmanFord);
    assert(resDijkstraPath == resBellmanFordPath);

    graph.spfa(0);
    vector<int> resSpfa(graph.dist);
    vector<int> resSpfaPath = graph.getShortestPath(7);
    cout << resSpfa << endl;

    assert(resDijkstra == resSpfa);
    assert(resDijkstraPath == resSpfaPath);

    cout << "OK" << endl;
}
