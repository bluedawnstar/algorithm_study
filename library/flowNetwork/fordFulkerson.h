#pragma once

#include <vector>
using namespace std;

// Edmonds-Karp Algorithm (Ford-Fulkerson method)
template <typename T>
struct MaxFlowFordFulkerson {
    static const T INF = T(0x3f3f3f3f);

    struct Node {
        int to;         // v
        int revIndex;   // for (v -> u)

        T   flow;
        T   capacity;
    };

    int N;
    vector<vector<Node>> edges;
    vector<pair<int, int>> parent;  // (u, edge index in u's edges)

    MaxFlowFordFulkerson() : N(0) {
    }

    MaxFlowFordFulkerson(int n) : N(n), edges(N + 1), parent(N + 1) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<Node>>(N + 1);
        parent = vector<int>(N + 1);
    }

    // add edges to a directed graph
    void addEdge(int u, int v, T capacity, T capacityRev = 0) {
        int uN = (int)edges[u].size();
        int vN = (int)edges[v].size();
        edges[u].push_back(Node{ v, vN, 0, capacity });
        edges[v].push_back(Node{ u, uN, 0, capacityRev });
    }

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
                auto& v = edges[u][i];
                if (parent[v.to].first == -1 && (v.capacity - v.flow) > 0) {
                    q.push(v.to);
                    parent[v.to] = make_pair(u, i);
                }
            }
        }

        return parent[t].first != -1;
    }

    T doFordFulkerson(int s, int t) {
        T res = 0;

        while (bfs(s, t)) {
            T amount = INF;
            for (int i = t; i != s; i = parent[i].first) {
                int u = parent[i].first;
                auto& v = edges[u][parent[i].second];
                amount = min(amount, v.capacity - v.flow);
            }

            for (int i = t; i != s; i = parent[i].first) {
                int u = parent[i].first;
                auto& v = edges[u][parent[i].second];
                v.flow += amount;
                edges[v.to][v.revIndex].flow -= amount;
            }
            res += amount;
        }

        return res;
    }
};
