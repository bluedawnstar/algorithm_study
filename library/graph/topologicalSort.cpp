#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

namespace TopologicalSortDigraph {

#define MAXN    100

int gN;
vector<vector<int>> gEdge;
vector<bool> gVis;

//----------------------------------------------------
vector<int> gRes;

void dfsTS(int node) {
    gVis[node] = true;
    for (int i = 0; i < (int)gEdge[node].size(); i++) {
        int to = gEdge[node][i];
        if (!gVis[to])
            dfsTS(to);
        //else if (find(gRes.begin(), gRes.end(), to) == gRes.end())
        //    cout << "a cycle detected!" << endl;
    }
    gRes.push_back(node);
}

void dfsAllTS() {
    gVis = vector<bool>(gN, false);

    gRes.clear();
    for (int i = 0; i < gN; i++) {
        if (!gVis[i])
            dfsTS(i);
    }
    reverse(gRes.begin(), gRes.end());
}

// Kahn's algorithm
void bfsTS() {
    gRes.clear();

    vector<int> inDegree(gN, 0);
    for (int i = 0; i < gN; i++) {
        for (int j = 0; j < (int)gEdge[i].size(); j++)
            inDegree[gEdge[i][j]]++;
    }

    queue<int> q;
    for (int i = 0; i < gN; i++) {
        if (inDegree[i] == 0)
            q.push(i);
    }
    while (!q.empty()) {
        int node = q.front();
        q.pop();

        gRes.push_back(node);
        for (int i = 0; i < (int)gEdge[node].size(); i++) {
            int to = gEdge[node][i];
            if (--inDegree[to] == 0)
                q.push(to);
        }
    }

    //if (gRes.size() != gN)
    //    cout << "a cycle detected!" << endl;
}

} // namespace TopologicalSortDigraph

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

using namespace TopologicalSortDigraph;

static void buildGraph() {
    gN = 6;
    gEdge = vector<vector<int>>(gN);
    gEdge[5].push_back(2);
    gEdge[5].push_back(0);
    gEdge[4].push_back(0);
    gEdge[4].push_back(1);
    gEdge[2].push_back(3);
    gEdge[3].push_back(1);
}

void testTopologicalSort() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- TopologicalSort - DFS ---------" << endl;
    buildGraph();

    dfsAllTS();
    cout << "dfsAllTs() : " << gRes << endl;

    cout << "-- TopologicalSort - BFS ---------" << endl;
    buildGraph();

    bfsTS();
    cout << "bfsTs() : " << gRes << endl;

    cout << "OK" << endl;
}
