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

        UnionFind dsu(N);
        for (int i = 0; i < int(E.size()); i++) {
            T cost = E[i].first;
            int u = E[i].second.first;
            int v = E[i].second.second;
            if (dsu.find(u) == dsu.find(v))
                continue;
            dsu.merge(u, v);
            selected.emplace_back(u, v);
            res += cost;
        }

        return res; // total cost
    }

    // Sollin's (Boruvka's) Algorithm : O(E*logV)
    T sollin(vector<pair<int, int>>& selected) {
        T res = 0;

        selected.clear();

        UnionFind dsu(N);
        int treeCount = N;
        while (treeCount > 1) {
            vector<tuple<int,int,T>> cheapest(N, make_tuple(-1, -1, numeric_limits<T>::max()));
            for (int u = 0; u < N; u++) {
                int ug = dsu.find(u);
                for (auto& it : edges[u]) {
                    if (u >= it.first)
                        continue;   // skip one of bidirectional edges

                    int vg = dsu.find(it.first);
                    if (ug == vg)
                        continue;

                    if (get<2>(cheapest[ug]) > it.second)
                        cheapest[ug] = make_tuple(u, it.first, it.second);

                    if (get<2>(cheapest[vg]) > it.second)
                        cheapest[vg] = make_tuple(u, it.first, it.second);
                }
            }

            for (int i = 0; i < N; i++) {
                int u = get<0>(cheapest[i]);
                int v = get<1>(cheapest[i]);
                if (u == -1)
                    continue;
                int ug = dsu.find(u);
                int vg = dsu.find(v);
                if (ug == vg)
                    continue;
                dsu.merge(ug, vg);
                selected.emplace_back(u, v);
                res += get<2>(cheapest[i]);
                treeCount--;
            }
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

    //--- with edge list

    // Kruskal Algorithm : O(E*logE)
    static T kruskal(int N, const vector<tuple<int,int,T>>& edges, vector<int>& selected) {
        T res = 0;

        selected.clear();

        int edgeN = int(edges.size());
        vector<int> index(edgeN);
        iota(index.begin(), index.end(), 0);
        sort(index.begin(), index.end(), [&edges](int a, int b) {
            return get<2>(edges[a]) < get<2>(edges[b]);
        });

        UnionFind dsu(N);
        for (int i = 0; i < edgeN; i++) {
            int u = get<0>(edges[index[i]]);
            int v = get<1>(edges[index[i]]);
            if (dsu.find(u) == dsu.find(v))
                continue;
            dsu.merge(u, v);
            selected.push_back(index[i]);
            res += get<2>(edges[index[i]]);
        }

        return res; // total cost
    }

    // Sollin's (Boruvka's) Algorithm : O(E*logV)
    static T sollin(int N, const vector<tuple<int, int, T>>& edges, vector<int>& selected) {
        T res = 0;

        selected.clear();

        UnionFind dsu(N);
        int treeCount = N;
        while (treeCount > 1) {
            vector<int> cheapest(N, -1);
            for (int i = 0; i < int(edges.size()); i++) {
                int u = get<0>(edges[i]);
                int v = get<1>(edges[i]);
                T w = get<2>(edges[i]);
                int ug = dsu.find(u);
                int vg = dsu.find(v);
                if (ug == vg)
                    continue;

                if (cheapest[ug] == -1 || get<2>(edges[cheapest[ug]]) > w)
                    cheapest[ug] = i;

                if (cheapest[vg] == -1 || get<2>(edges[cheapest[vg]]) > w)
                    cheapest[vg] = i;
            }

            for (int i = 0; i < N; i++) {
                if (cheapest[i] == -1)
                    continue;
                int u = get<0>(edges[cheapest[i]]);
                int v = get<1>(edges[cheapest[i]]);
                int ug = dsu.find(u);
                int vg = dsu.find(v);
                if (ug == vg)
                    continue;
                dsu.merge(ug, vg);
                selected.push_back(cheapest[i]);
                res += get<2>(edges[cheapest[i]]);
                treeCount--;
            }
        }

        return res; // total cost
    }
};
