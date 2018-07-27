#pragma once

#include "shortestPathOneSource.h"

// Shortest paths of all pairs for directed graph
template <typename T>
struct ShortestAllPairs : public ShortestPath<T> {
    ShortestAllPairs() : ShortestPath<T>() {
    }

    ShortestAllPairs(int n) : ShortestPath<T>(n) {
    }

    // time complexity : O(V^2 * logV + VE)
    // return false if the graph has negative cycles
    bool findAllPathJohnson(vector<vector<T>>& D, vector<vector<int>>& parent, int N) {
        // Bellman-Ford
        vector<T> h(N);
        {
            bool updated = false;
            for (int i = 0; i < N; i++) {
                updated = false;
                for (int u = 0; u < N; u++) {
                    for (int j = 0; j < int(edges[u].size()); j++) {
                        int v = edges[u][j].first;
                        T w = edges[u][j].second;
                        if (h[v] > h[u] + w) {
                            h[v] = h[u] + w;
                            updated = true;
                        }
                    }
                }
                if (!updated)
                    break;
            }
            if (updated)
                return false;
        }

        D.resize(N);
        parent.resize(N);
        for (int u = 0; u < N; u++) {
            D[u].assign(N, INF);
            parent[u].assign(N, -1);

            D[u][u] = 0;
            for (auto& e : edges[u]) {
                auto v = e.first;
                if (u != v) {
                    D[u][v] = e.second + h[u] - h[v];
                    parent[u][v] = u;
                }
            }
        }

        // Dijkstra
        for (int start = 0; start < N; start++) {
            auto& d = D[start];
            auto& p = parent[start];

            fill(d.begin(), d.end(), INF);
            fill(p.begin(), p.end(), -1);

            priority_queue<pair<T, int>> pq;    // (-weight, vertex)

            pq.push(make_pair(0, start));
            d[start] = 0;
            p[start] = -1;
            while (!pq.empty()) {
                T w = -pq.top().first;          // weight
                int u = pq.top().second;        // vertex u

                pq.pop();
                if (d[u] < w)
                    continue;

                for (int i = 0; i < int(edges[u].size()); i++) {
                    int v = edges[u][i].first;
                    T vDist = w + edges[u][i].second + h[u] - h[v];
                    if (d[v] > vDist) {
                        pq.push(make_pair(-vDist, v));
                        d[v] = vDist;
                        p[v] = u;
                    }
                }
            }

            for (int v = 0; v < N; v++) {
                if (v != start && d[v] < INF)
                    d[v] += h[v] - h[start];
            }
        }

        return true;
    }

    // time complexity : normal - O(VE), worst - O(V^2 * E)
    // return false if the graph has negative cycles
    bool findAllPathSpfa(vector<vector<T>>& D, vector<vector<int>>& parent, int N) {
        D.resize(N);
        parent.resize(N);
        for (int u = 0; u < N; u++) {
            D[u].assign(N, INF);
            parent[u].assign(N, -1);

            D[u][u] = 0;
            for (auto& e : edges[u]) {
                auto v = e.first;
                if (u != v) {
                    D[u][v] = e.second;
                    parent[u][v] = u;
                }
            }
        }

        for (int start = 0; start < N; start++) {
            auto& d = D[start];
            auto& p = parent[start];

            fill(d.begin(), d.end(), INF);
            fill(p.begin(), p.end(), -1);

            vector<bool> inQ(N);
            vector<int> updated(N);

            d[start] = 0;
            p[start] = -1;

            queue<int> Q;
            Q.push(start);
            updated[start]++;
            inQ[start] = true;
            while (!Q.empty()) {
                int u = Q.front();
                Q.pop();

                for (auto v : edges[u]) {
                    if (d[v.first] > d[u] + v.second) {
                        d[v.first] = d[u] + v.second;
                        p[v.first] = u;
                        if (!inQ[v.first]) {
                            Q.push(v.first);
                            if (++updated[v.first] >= N)
                                return false;
                            inQ[v.first] = true;
                        }
                    }
                }

                inQ[u] = false;
            }
        }

        return true;
    }

    static vector<int> getShortestPath(vector<vector<int>>& parent, int u, int v) {
        vector<int> res;
        do {
            res.push_back(v);
            v = parent[u][v];
        } while (v >= 0);

        reverse(res.begin(), res.end());

        return res;
    }

    //------------------------------------------------------------------------------------

    // Floyd-Warshal algorithm : O(V^3)
    void doFloydWarshal(vector<vector<T>>& D, vector<vector<int>>& parent, int N) {
        D.resize(N);
        parent.resize(N);
        for (int u = 0; u < N; u++) {
            D[u].assign(N, INF);
            parent[u].assign(N, -1);

            D[u][u] = 0;
            for (auto& e : edges[u]) {
                auto v = e.first;
                if (u != v) {
                    D[u][v] = e.second;
                    parent[u][v] = u;
                }
            }
        }

        for (int k = 0; k < N; k++) {
            for (int i = 0; i < N; i++) {
                if (D[i][k] >= INF)
                    continue;
                for (int j = 0; j < N; j++) {
                    if (D[k][j] >= INF)
                        continue;
                    if (D[i][j] > D[i][k] + D[k][j]) {
                        D[i][j] = D[i][k] + D[k][j];
                        // D[i][j] = max(D[i][j], -INF);
                        parent[i][j] = parent[k][j];
                    }
                }
            }
        }
    }

    static vector<int> getPathFloydWarshal(const vector<vector<int>>& parent, int N, int u, int v) {
        vector<int> path;
        path.reserve(N);

        while (true) {
            path.push_back(v);
            if (u == v)
                break;
            v = parent[u][v];
        }
        reverse(path.begin(), path.end());

        return path;
    }
};
