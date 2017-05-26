#include <queue>
#include <climits>
#include <algorithm>
#include <vector>

using namespace std;

namespace OneSourceShortestPath {

#define MAXN    100

int gN;
vector<pair<int, int>> gE[MAXN];     // (vertex, weight)
//vector<vector<pair<int, int>>> gE; // (vertex, weight)

int gDist[MAXN];
//vector<int> gDist;

int gParent[MAXN];
//vector<int> gParent;

// add edges to undirected graph
void addEdge(int u, int v, int w) {
    gE[u].emplace_back(v, w);
    gE[v].emplace_back(u, w);
}

// O(E logV)
void dijkstra(int start) {
    fill(gDist, gDist + gN, INT_MAX);
    //gDist = vector<int>(gN, INT_MAX);
    fill(gParent, gParent + gN, -1);
    //gParent = vector<int>(gN, -1);

    priority_queue<pair<int, int>> pq;   // (-weight, vertex)

    pq.push(make_pair(0, start));
    gDist[start] = 0;
    gParent[start] = -1;
    while (!pq.empty()) {
        int w = -pq.top().first;        // weight
        int u = pq.top().second;        // vertex u

        pq.pop();
        if (gDist[u] < w)
            continue;

        for (int i = 0; i < (int)gE[u].size(); i++) {
            int v = gE[u][i].first;
            int vDist = gE[u][i].second + w;
            if (gDist[v] > vDist) {
                pq.push(make_pair(-vDist, v));
                gDist[v] = vDist;
                gParent[v] = u;
            }
        }
    }
}

// O(VE)
bool bellmanFord(int start) {
    fill(gDist, gDist + gN, INT_MAX);
    //gDist = vector<int>(gN, INT_MAX);
    fill(gParent, gParent + gN, -1);
    //gParent = vector<int>(gN, -1);

    gDist[start] = 0;
    gParent[start] = -1;

    bool updated = false;
    for (int i = 0; i < gN; i++) {
        updated = false;
        for (int u = 0; u < gN; u++) {
            for (int j = 0; j < (int)gE[u].size(); j++) {
                int v = gE[u][j].first;
                int w = gE[u][j].second;
                if (gDist[v] > gDist[u] + w) {
                    gDist[v] = gDist[u] + w;
                    gParent[v] = u;
                    updated = true;
                }
            }
        }
        if (!updated)
            break;
    }
    return !updated;
}

// Shortest Path Faster Algorithm
// time complexity : normal - O(E), worst - O(VE)
void spfa(int start) {
    fill(gDist, gDist + gN, INT_MAX);
    //gDist = vector<int>(gN, INT_MAX);
    fill(gParent, gParent + gN, -1);
    //gParent = vector<int>(gN, -1);

    vector<bool> inQ(gN);

    gDist[start] = 0;
    gParent[start] = -1;

    queue<int> Q;
    Q.push(start);
    inQ[start] = true;
    while (!Q.empty()) {
        int u = Q.front();
        Q.pop();

        for (auto v : gE[u]) {
            if (gDist[v.first] > gDist[u] + v.second) {
                gDist[v.first] = gDist[u] + v.second;
                gParent[v.first] = u;
                if (!inQ[v.first]) {
                    Q.push(v.first);
                    inQ[v.first] = true;
                }
            }
        }

        inQ[u] = false;
    }
}


vector<int> getShortestPath(int v) {
    vector<int> res;
    do {
        res.push_back(v);
        v = gParent[v];
    } while (v >= 0);

    reverse(res.begin(), res.end());

    return res;
}

} // namespace OneSourceShortestPath

  /////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

using namespace OneSourceShortestPath;

void buildGraph() {
    gN = 9;
    addEdge(0, 1, 4);
    addEdge(0, 7, 8);
    addEdge(1, 2, 8);
    addEdge(1, 7, 11);
    addEdge(2, 3, 7);
    addEdge(2, 5, 4);
    addEdge(2, 8, 2);
    addEdge(3, 4, 9);
    addEdge(3, 5, 14);
    addEdge(4, 5, 10);
    addEdge(5, 6, 2);
    addEdge(6, 7, 1);
    addEdge(6, 8, 6);
    addEdge(7, 8, 7);
}

void testShortestPath() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Shortest Path ---------" << endl;
    buildGraph();

    dijkstra(0);
    vector<int> resDijkstra(gDist, gDist + gN);
    vector<int> resDijkstraPath = getShortestPath(7);
    cout << resDijkstra << endl;

    bellmanFord(0);
    vector<int> resBellmanFord(gDist, gDist + gN);
    vector<int> resBellmanFordPath = getShortestPath(7);
    cout << resBellmanFord << endl;

    assert(resDijkstra == resBellmanFord);
    assert(resDijkstraPath == resBellmanFordPath);

    spfa(0);
    vector<int> resSpfa(gDist, gDist + gN);
    vector<int> resSpfaPath = getShortestPath(7);
    cout << resSpfa << endl;

    assert(resDijkstra == resSpfa);
    assert(resDijkstraPath == resSpfaPath);

    cout << "OK" << endl;
}
