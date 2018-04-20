#pragma once

#include <vector>
using namespace std;

// Edmonds-Karp Algorithm (Ford-Fulkerson method)
template <typename T>
struct MinCutMaxFlow {
    static const T INF = T(0x3f3f3f3f);

    struct Edge {
        int to;                     // v
        int revIndex;               // for (v -> u)

        T   flow;
        T   capacity;
    };

    int N;                          // the number of vertices
    vector<vector<Edge>> edges;
    vector<pair<int, int>> parent;  // (u, edge index in u's edges)

    MinCutMaxFlow() : N(0) {
    }

    explicit MinCutMaxFlow(int n) : N(n), edges(N), parent(N, make_pair(-1, -1)) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<Edge>>(N);
        parent = vector<pair<int, int>>(N, make_pair(-1, -1));
    }

    // add edges to a directed graph
    void addEdge(int u, int v, T capacity, T capacityRev = 0) {
        int uN = (int)edges[u].size();
        int vN = (int)edges[v].size();
        edges[u].push_back(Edge{ v, vN, 0, capacity });
        edges[v].push_back(Edge{ u, uN, 0, capacityRev });
    }

    void clearFlow() {
        for (auto& vec : edges) {
            for (auto& e : vec)
                e.flow = 0;
        }
    }

    // O(V * E^2)
    T findMinCut(int s, int t, vector<pair<int, int>>& minCutEdges) {
        clearFlow();
        minCutEdges.clear();

        while (bfs(s, t)) {
            T amount = INF;
            for (int i = t; i != s; i = parent[i].first) {
                int u = parent[i].first;
                auto& e = edges[u][parent[i].second];
                amount = min(amount, e.capacity - e.flow);
            }

            for (int i = t; i != s; i = parent[i].first) {
                int u = parent[i].first;
                auto& e = edges[u][parent[i].second];
                e.flow += amount;
                edges[e.to][e.revIndex].flow -= amount;
            }
        }

        T res = 0;
        for (int u = 0; u < (int)N; u++) {
            for (int j = 0; j < (int)edges[u].size(); j++) {
                auto& e = edges[u][j];
                if (e.capacity > 0 && ((parent[u].first != -1) && (parent[e.to].first == -1))) {
                    res += e.flow;
                    minCutEdges.push_back(make_pair(u, e.to));
                }
            }
        }

        return res;
    }

private:
    bool bfs(int s, int t) {
        fill(parent.begin(), parent.end(), make_pair(-1, -1));
        //memset(&parent[0], -1, sizeof(parent[0]) * parent.size());

        queue<int> q;

        q.push(s);
        parent[s] = make_pair(s, -1);
        while (!q.empty() && parent[t].first == -1) {
            int u = q.front();
            q.pop();

            for (int i = 0; i < (int)edges[u].size(); i++) {
                auto& e = edges[u][i];
                if (parent[e.to].first == -1 && (e.capacity - e.flow) > 0) {
                    q.push(e.to);
                    parent[e.to] = make_pair(u, i);
                }
            }
        }

        return parent[t].first != -1;
    }
};
