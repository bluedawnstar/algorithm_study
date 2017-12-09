#pragma once

#include "../set/unionFind.h"

struct BasicUndirectedGraph {
    int N;
    vector<vector<int>> edges;

    BasicUndirectedGraph() : N(0) {
    }

    BasicUndirectedGraph(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(N);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    //--- shortest path without weight ---

    vector<int> searchShortestPathBFS(int start) {
        vector<int> dist(N, -1);
        vector<int> parent(N, -1);

        queue<int> Q;
        Q.push(start);
        dist[start] = 0;
        parent[start] = -1;
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (int v : edges[u]) {
                if (dist[v] < 0) {
                    Q.push(v);
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                }
            }
        }

        return move(parent);
    }

    vector<int> getShortestPath(int u, int v) {
        vector<int> parent = searchShortestPathBFS(u);

        vector<int> res;
        do {
            res.push_back(v);
            v = parent[v];
        } while (v >= 0);
        reverse(res.begin(), res.end());

        return res;
    }

    //--- cycle detection with DFS ---

    bool isCyclicGraphDFS(vector<bool>& visited, int u, int parent) {
        visited[u] = true;
        for (int v : edges[u]) {
            if (!visited[v]) {
                if (isCyclicGraphDFS(visited, v, u))
                    return true;
            } else if (v != parent)
                return true;
        }

        return false;
    }

    bool isCyclicGraphDFS() {
        vector<bool> visited(N);
        for (int u = 0; u < N; u++) {
            if (!visited[u] && isCyclicGraphDFS(visited, u, -1))
                return true;
        }

        return false;
    }

    //--- cycle detection with Union Find ---

    bool isCyclicGraphWithUnionFind() {
        UnionFind uf(N);

        for (int u = 0; u < N; u++) {
            for (int v : edges[u]) {
                if (u < v) {
                    int x = uf.findSet(u);
                    int y = uf.findSet(v);
                    if (x == y)
                        return true;
                    uf.unionSet(u, v);
                }
            }
        }

        return false;
    }

    //--- articulation point (cut vertex) ---
    struct CutVertexContext {
        vector<bool> cutVertex;

        vector<bool> visited;
        vector<int> parent;

        int discoverCount;
        vector<int> discover;
        vector<int> low;

        CutVertexContext(int n)
            : cutVertex(n), visited(n), parent(n, -1),
              discoverCount(0), discover(n), low(n) {
        }
    };
    void findCutVertex(CutVertexContext& ctx, int u) {
        ctx.visited[u] = true;
        ctx.discover[u] = ctx.low[u] = ctx.discoverCount++;

        int childCount = 0;
        for (int v : edges[u]) {
            if (!ctx.visited[v]) {
                ctx.parent[v] = u;
                findCutVertex(ctx, v);
                childCount++;
                if (ctx.low[v] >= ctx.discover[u])
                    ctx.cutVertex[u] = true;
                ctx.low[u] = min(ctx.low[u], ctx.low[v]);
            } else if (v != ctx.parent[u])
                ctx.low[u] = min(ctx.low[u], ctx.discover[v]);
        }
        if (ctx.parent[u] < 0)
            ctx.cutVertex[u] = (childCount > 1);
    }

    vector<bool> findCutVertex() {
        CutVertexContext ctx(N);
        for (int u = 0; u < N; u++) {
            if (!ctx.visited[u])
                findCutVertex(ctx, u);
        }
        return move(ctx.cutVertex);
    }

    //--- bridge ---
    struct BridgeContext {
        vector<pair<int,int>> bridge;

        vector<bool> visited;
        vector<int> parent;

        int discoverCount;
        vector<int> discover;
        vector<int> low;

        BridgeContext(int n)
            : visited(n), parent(n, -1), discoverCount(0), discover(n), low(n) {
        }
    };
    void findBridge(BridgeContext& ctx, int u) {
        ctx.visited[u] = true;
        ctx.discover[u] = ctx.low[u] = ctx.discoverCount++;

        for (int v : edges[u]) {
            if (!ctx.visited[v]) {
                ctx.parent[v] = u;
                findBridge(ctx, v);
                if (ctx.low[v] > ctx.discover[u])
                    ctx.bridge.push_back(pair<int, int>(u, v));
                ctx.low[u] = min(ctx.low[u], ctx.low[v]);
            } else if (v != ctx.parent[u])
                ctx.low[u] = min(ctx.low[u], ctx.discover[v]);
        }
    }

    vector<pair<int,int>> findBridge() {
        BridgeContext ctx(N);
        for (int u = 0; u < N; u++) {
            if (!ctx.visited[u])
                findBridge(ctx, u);
        }
        return move(ctx.bridge);
    }

protected:
    void dfs(int u, vector<bool>& visited) {
        //cout << "dfs(" << u << ")" << endl;

        visited[u] = true;
        for (int v : edges[u]) {
            if (!visited[v])
                dfs(v, visited);
        }
    }

    void dfsAll() {
        vector<bool> visited(N);
        for (int u = 0; u < N; u++) {
            if (!visited[u])
                dfs(u, visited);
        }
    }

    void bfs(int start, vector<bool>& visited) {
        queue<int> Q;
        Q.push(start);
        visited[start] = true;
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            //cout << "bfs(" << start << ") : " << u << endl;

            for (int v : edges[u]) {
                if (!visited[v]) {
                    Q.push(v);
                    visited[v] = true;
                }
            }
        }
    }

    void bfsAll() {
        vector<bool> visited(N);
        for (int u = 0; u < N; u++) {
            if (!visited[u])
                bfs(u, visited);
        }
    }
};
