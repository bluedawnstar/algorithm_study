#pragma once

#include "../set/unionFind.h"

// https://cp-algorithms.com/graph/lca_tarjan.html
// Lowest Common Ancestor - Tarjan's off-line algorithm

struct LcaTarjan {
    int N;
    UnionFind dsu;

    vector<int> ancestor;
    vector<bool> visited;

    LcaTarjan() : N(0) {
    }

    // O(N + M)
    // queries[u][i] = { v, lca }
    // CAUTION! : v should be visited before u
    void lca(const vector<vector<int>>& edges, int root, vector<vector<pair<int,int>>>& queries) {
        init(int(edges.size()));
        dfs(edges, root, queries);
    }

private:
    void init(int N) {
        this->N = N;
        dsu.init(N);

        ancestor.assign(N, 0);
        visited.assign(N, false);
    }

    void dfs(const vector<vector<int>>& edges, int u, vector<vector<pair<int, int>>>& queries) {
        visited[u] = true;
        ancestor[u] = u;

        for (int v : edges[u]) {
            if (!visited[v]) {
                dfs(edges, v, queries);
                dsu.unionSet(u, v);
                ancestor[dsu.findSet(u)] = u;
            }
        }

        for (auto& v : queries[u]) {
            if (visited[v.first])
                v.second = ancestor[dsu.findSet(v.first)];
        }
    }
};
