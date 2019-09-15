#pragma once

// Articulation point (Cut Vertex)
struct UndirectedGraphCutVertex {
    int N;
    vector<vector<int>> edges;

    //--- for cut-vertex
    vector<bool> visited;

    int discoverCount;
    vector<int> discover;
    vector<int> low;

    vector<bool> cutVertex;

    UndirectedGraphCutVertex() : N(0) {
    }

    explicit UndirectedGraphCutVertex(int n) {
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
    const vector<bool>& findCutVertex() {
        visited = vector<bool>(N);

        discoverCount = 0;
        discover = vector<int>(N);
        low = vector<int>(N);

        cutVertex = vector<bool>(N);

        for (int u = 0; u < N; u++) {
            if (!visited[u])
                findCutVertex(u, -1);
        }
        return cutVertex;
    }

private:
    void findCutVertex(int u, int parent) {
        visited[u] = true;
        discover[u] = low[u] = discoverCount++;

        int childCount = 0;
        for (int v : edges[u]) {
            if (!visited[v]) {
                findCutVertex(v, u);
                childCount++;
                if (low[v] >= discover[u])
                    cutVertex[u] = true;
                low[u] = min(low[u], low[v]);
            } else if (v != parent)
                low[u] = min(low[u], discover[v]);
        }
        if (parent < 0)
            cutVertex[u] = (childCount > 1);
    }
};
