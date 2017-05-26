#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

namespace MinCostMaxFlow {

#define MAXN    300

#define INF     1000000000

struct NodeT {
    int to;
    int flow;
    int cost;
    int revIndex;   // for (v -> u)
};
vector<NodeT> gE[MAXN];
bool gVisited[MAXN];
int gCost[MAXN];

// add edges to a directed graph
void addEdge(int u, int v, int flow, int cost) {
    int un = (int)gE[u].size();
    int vn = (int)gE[v].size();
    gE[u].push_back(NodeT{ v, flow, cost, vn });
    gE[v].push_back(NodeT{ u, 0, -cost, un });
}

// update cost
bool bfs(int S, int T) {
    fill(gCost, gCost + MAXN, INF);
    memset(gVisited, 0, sizeof(gVisited));

    gCost[S] = 0;

    queue<int> Q;
    Q.push(S);
    gVisited[S] = true; // inQ[S] = true
    while (!Q.empty()) {
        int u = Q.front();
        Q.pop();

        for (auto& v : gE[u]) {
            if (v.flow > 0 && gCost[v.to] > gCost[u] + v.cost) {
                gCost[v.to] = gCost[u] + v.cost;
                if (!gVisited[v.to]) {
                    Q.push(v.to);
                    gVisited[v.to] = true; // inQ[v.to] = true
                }
            }
        }

        gVisited[u] = false; // inQ[u] = false
    }

    return gCost[T] < 0; //### TODO: check stop condition!!!
}

// update flow and return (flow, cost)
pair<int, int> dfs(int u, int T, int flow) {
    if (u == T)
        return make_pair(flow, flow * gCost[u]);

    gVisited[u] = true;

    pair<int, int> res;
    for (auto& v : gE[u]) {
        if (!gVisited[v.to] && v.flow > 0 && gCost[v.to] == gCost[u] + v.cost) {
            auto r = dfs(v.to, T, min(flow - res.first, v.flow));

            res.first += r.first;
            res.second += r.second;

            v.flow -= r.first;
            gE[v.to][v.revIndex].flow += r.first;

            if (res.first == flow)
                break;
        }
    }
    return res;
}

// (flow, cost)
pair<int, int> minCostMaxFlow(int S, int T) {
    pair<int, int> res;

    while (bfs(S, T)) {
        auto r = dfs(S, T, INF);
        res.first += r.first;
        res.second += r.second;
    }

    return res;
}

} // namespace MinCostMaxFlow

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

using namespace MinCostMaxFlow;

// https://www.hackerrank.com/contests/w32/challenges/balls-and-boxes

#define NODE_BALL_S 0
#define NODE_BOX_S  100
#define NODE_S      200
#define NODE_T      201

void build(const vector<int>& A, const vector<int>& C, const vector<vector<int>>& B) {
    int N = (int)A.size();
    int M = (int)C.size();

    for (int i = 0; i < N; i++)
        addEdge(NODE_S, NODE_BALL_S + i, A[i], 0);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            addEdge(NODE_BALL_S + i, NODE_BOX_S + j, 1, -B[i][j]);
        }
    }

    for (int i = 0; i < M; i++) {
        addEdge(NODE_BOX_S + i, NODE_T, C[i], 0);
        for (int j = C[i]; j < N; j++)
            addEdge(NODE_BOX_S + i, NODE_T, 1, 2 * (j - C[i]) + 1);
    }
}

void testMinCostMaxFlow() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Min-Cost Max-Flow ---------" << endl;
    // https://www.hackerrank.com/contests/w32/challenges/balls-and-boxes
    build(
        vector<int>{ 1, 4, 8 },
        vector<int>{ 2, 5, 1, 1, 6, 1 },
        vector<vector<int>>{
            { 633, 587, 100, 887, 979, 892 },
            { 708, 871, 132, 28, 255, 825 },
            { 536, 139, 672, 280, 968, 417 }
        }
    );
    int minCost = -minCostMaxFlow(NODE_S, NODE_T).second;
    cout << "min cost = " << minCost << endl;
    assert(minCost == 6649);

    cout << "OK" << endl;
}
