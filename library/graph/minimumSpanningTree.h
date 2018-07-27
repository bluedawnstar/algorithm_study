#pragma once

#include "../set/unionFind.h"

// undirected graph
template <typename T, const T INF = 0x3f3f3f3f>
struct MinimumSpanningTree {
    int N;
    vector<vector<pair<int, T>>> edges;  // (v, weight)

    MinimumSpanningTree() : N(0) {
    }

    explicit MinimumSpanningTree(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<pair<int, T>>>(N);
    }

    // add edges to undirected graph
    void addEdge(int u, int v, T w) {
        edges[u].emplace_back(v, w);
        edges[v].emplace_back(u, w);
    }

    // Kruskal Algorithm : O(E*logE)
    T kruskal(vector<pair<int, int>>& selected) {
        T res = 0;

        selected.clear();

        vector<pair<T, pair<int, int>>> E;  // (weight, (u, v))
        for (int u = 0; u < N; u++) {
            for (int i = 0; i < int(edges[u].size()); i++) {
                int v = edges[u][i].first;
                int cost = edges[u][i].second;
                E.push_back(make_pair(cost, make_pair(u, v)));
            }
        }
        sort(E.begin(), E.end());

        UnionFind sets(N);
        for (int i = 0; i < int(E.size()); i++) {
            T cost = E[i].first;
            int u = E[i].second.first;
            int v = E[i].second.second;
            if (sets.findSet(u) == sets.findSet(v))
                continue;
            sets.unionSet(u, v);
            selected.emplace_back(u, v);
            res += cost;
        }

        return res; // total cost
    }

    // Prim's Algorithm : O(E*logV)
    T prim(vector<int>& parent) {
        T res = 0;

        parent.assign(N, -1);

        vector<bool> visited(N);
        vector<T> cost(N, INF);

        priority_queue<pair<T, int>> Q;
        Q.emplace(T(0), 0);
        cost[0] = 0;
        while (!Q.empty()) {
            T w = -Q.top().first;
            int u = Q.top().second;

            Q.pop();

            if (visited[u])
                continue;
            visited[u] = true;

            res += w;
            for (auto& e : edges[u]) {
                int v = e.first;
                if (visited[v])
                    continue;

                T c = e.second;
                if (cost[v] > c) {
                    cost[v] = c;
                    parent[v] = u;
                    Q.emplace(-c, v);
                }
            }
        }

        return res; // total cost
    }
};
