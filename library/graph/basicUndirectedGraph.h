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
