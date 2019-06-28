#pragma once

#include "../set/bitSetSimple.h"

// Directed Acyclic Graph
template <typename T, const T INF = 0x3f3f3f3f>
struct DAG {
    int N;
    vector<vector<pair<int,T>>> edges;

    DAG() : N(0) {
    }

    DAG(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<pair<int, T>>>(N);
    }

    // add edges to undirected graph
    void addEdge(int u, int v, T w) {
        edges[u].emplace_back(v, w);
    }

    //--- topological sort with DFS

    // return if cycle detected
    bool topologicalSortDFS(int u, vector<int>& res) const {
        res.clear();
        res.reserve(N);

        vector<bool> visited(N);

        bool cycleDetected = topologicalSortDFS(u, visited, res);
        reverse(res.begin(), res.end());

        return cycleDetected;
    }

    // return if cycle detected
    bool topologicalSortDFS(vector<int>& res) const {
        res.clear();
        res.reserve(N);

        vector<bool> visited(N);

        bool cycleDetected = false;
        for (int i = 0; i < N; i++) {
            if (!visited[i]) {
                if (topologicalSortDFS(i, visited, res))
                    cycleDetected = true;
            }
        }
        reverse(res.begin(), res.end());

        return cycleDetected;
    }

    //--- topological sort with BFS (Kahn's algorithm)

    // return if cycle detected
    bool topologicalSortBFS(vector<int>& res) const {
        res.clear();
        res.reserve(N);

        vector<int> inDegree(N, 0);
        for (int u = 0; u < N; u++) {
            for (auto& e : edges[u])
                inDegree[e.first]++;
        }

        queue<int> Q;
        for (int i = 0; i < N; i++) {
            if (inDegree[i] == 0)
                Q.push(i);
        }
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            res.push_back(u);
            for (auto& e : edges[u]) {
                if (--inDegree[e.first] == 0)
                    Q.push(e.first);
            }
        }

        return res.size() != N;
    }

    //--- shortest path - one source

    // O(V + E)
    // return (distances, parents)
    pair<vector<int>, vector<int>> findShortestPath(int start) const {
        vector<int> order;
        topologicalSortDFS(order);

        vector<T> dist(N, INF);
        vector<int> parent(N, -1);

        dist[start] = 0;
        for (int u : order) {
            if (dist[u] == INF)
                continue;

            for (auto& e : edges[u]) {
                int v = e.first;
                if (dist[v] > dist[u] + e.second) {
                    dist[v] = dist[u] + e.second;
                    parent[v] = u;
                }
            }
        }

        return make_pair(move(dist), move(parent));
    }

    vector<int> getShortestPath(const vector<int>& parents, int v) const {
        vector<int> res;
        do {
            res.push_back(v);
            v = parents[v];
        } while (v >= 0);

        reverse(res.begin(), res.end());

        return res;
    }

    //--- Transitive Closure

    vector<BitSetSimple> getTransitiveClosure(const vector<int>& sorted) const {
        vector<BitSetSimple> res;
        for (int i = int(sorted.size()) - 1; i >= 0; i--) {
            int u = sorted[i];

            res[u].init(N);
            res[u].set(u);
            for (int v : edges[u])
                res[u] |= res[v];
        }

        return res;
    }

    vector<BitSetSimple> getTransitiveClosure() const {
        vector<int> sorted;
        topologicalSortDFS(sorted);

        return getTransitiveClosure(sorted);
    }


    //--- LCA

    vector<BitSetSimple> makeAncestorTable(const vector<int>& sorted) const {
        vector<BitSetSimple> res;

        for (int u = 0; u < N; u++)
            res[u].init(N);

        for (int u : sorted) {
            res[u].set(u);
            for (int v : edges[u])
                res[v] |= res[u];
        }

        return res;
    }

    vector<BitSetSimple> makeAncestorTable() const {
        vector<int> sorted;
        topologicalSortDFS(sorted);

        return makeAncestorTable(sorted);
    }

    // any one LCA
    int findLCA(const vector<BitSetSimple>& ancestorTable, int u, int v) const {
        for (int idx = (N - 1) >> BitSetSimple::INDEX_SHIFT; idx >= 0; idx--) {
            auto t = (ancestorTable[u].mV[idx] & ancestorTable[v].mV[idx]);
            if (t) {
                return idx * BitSetSimple::BIT_SIZE + (BIT_SIZE - 1) - clz(t);
            }
        }
        return -1;
    }

private:
    // return if cycle detected
    bool topologicalSortDFS(int u, vector<bool>& visited, vector<int>& res) const {
        visited[u] = true;
        for (auto& e : edges[u]) {
            if (!visited[e.first]) {
                if (topologicalSortDFS(e.first, visited, res))
                    return true;
            }
            //TODO: uncomment if cycle detection is necessary
            //else if (find(res.begin(), res.end(), e.first) == res.end())
            //    return true;
        }
        res.push_back(u);
        return false;
    }

    static int clz(unsigned x) {
#ifndef __GNUC__
        return int(__lzcnt(x));
#else
        return __builtin_clz(x);
#endif
    }
};
