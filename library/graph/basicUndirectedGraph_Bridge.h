#pragma once

// Tarjan's bridge-finding algorithm

struct UndirectedGraphBridge {
    int N;
    vector<vector<int>> edges;

    //--- for bridge
    vector<bool> visited;

    int discoverCount;
    vector<int> discover;
    vector<int> low;

    vector<pair<int,int>> bridge;

    UndirectedGraphBridge() : N(0) {
    }

    explicit UndirectedGraphBridge(int n) {
        init(n);
    }

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(N);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    //---

    // O(V + E), Tarjan's algorithm
    const vector<pair<int,int>>& findBridge() {
        visited = vector<bool>(N);

        discoverCount = 0;
        discover = vector<int>(N);
        low = vector<int>(N);

        bridge.clear();

        for (int u = 0; u < N; u++) {
            if (!visited[u])
                findBridge(u, -1);
        }

        return bridge;
    }

private:
    void findBridge(int u, int parent) {
        visited[u] = true;
        discover[u] = low[u] = discoverCount++;

        for (int v : edges[u]) {
            if (!visited[v]) {
                findBridge(v, u);
                if (low[v] > discover[u])
                    bridge.push_back(make_pair(u, v));
                low[u] = min(low[u], low[v]);
            } else if (v != parent) {
                low[u] = min(low[u], discover[v]);
            }
        }
    }
};
