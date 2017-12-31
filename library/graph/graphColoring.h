#pragma once

#include "../set/bitSetVariable.h"

// undirected graph
struct GraphColoring {
    int N;
    vector<vector<int>> edges;

    GraphColoring() : N(0) {
    }

    GraphColoring(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(N);
    }

    // add edges v undirected graph
    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    // O(E*logV)
    vector<int> doVertexColoringGreedy() const {
        vector<BitSetVariable> used(N);
        vector<int> colors(N);

        priority_queue<long long> Q;
        for (int u = 0; u < N; u++) {
            colors[u] = -1;
            Q.push(-u);
        }
        for (int i = 0; i < N; i++) {
            int u;
            while (true) {
                u = -(int)Q.top();
                Q.pop();

                if (colors[u] == -1)
                    break;
            }

            int c = used[u].firstClearBit();
            colors[u] = c;
            for (int v : edges[u]) {
                if (!used[v].test(c)) {
                    used[v].set(c);
                    if (colors[v] == -1)
                        Q.push(((long long)used[v].count() << 32) - v);
                }
            }
        }
        return colors;
    }

    pair<int, vector<int>> doVertexColoring() {
        minColors = 0;
        bestColoring.assign(N, 0);

        vector<vector<bool>> G(N, vector<bool>(N));
        for (int u = 0; u < N; u++) {
            for (int v : edges[u])
                G[u][v] = true;
        }

        vector<int> id(N + 1);
        vector<int> deg(N + 1);
        for (int i = 0; i <= N; i++)
            id[i] = i;

        int res = 1;
        vector<int> resColors;

        for (int u = 0, v = 1; v <= N; v++) {
            int best = v;
            for (int i = v; i < N; i++) {
                if (G[id[v - 1]][id[i]])
                    ++deg[id[i]];

                if (deg[id[best]] < deg[id[i]])
                    best = i;
            }
            swap(id[v], id[best]);

            if (deg[id[v]] == 0) {
                minColors = N + 1;

                vector<int> tmp(N);
                dfs(G, id, tmp, u, v, u, 0);

                u = v;
                if (res < minColors) {
                    res = minColors;
                    resColors = bestColoring;
                }
            }
        }

        return make_pair(res, move(resColors));
    }

private:
    int minColors = 0;
    vector<int> bestColoring;

    void dfs(vector<vector<bool>>& G, vector<int>& id, vector<int>& coloring, int u, int v, int cur, int usedColors) {
        if (usedColors >= minColors)
            return;

        if (cur == v) {
            for (int i = u; i < v; i++)
                bestColoring[id[i]] = coloring[i];
            minColors = usedColors;
            return;
        }

        vector<bool> used(usedColors + 1);
        for (int i = 0; i < cur; i++)
            if (G[id[cur]][id[i]])
                used[coloring[i]] = true;

        for (int i = 0; i <= usedColors; i++) {
            if (!used[i]) {
                int tmp = coloring[cur];
                coloring[cur] = i;
                dfs(G, id, coloring, u, v, cur + 1, max(usedColors, i + 1));
                coloring[cur] = tmp;
            }
        }
    }
};
