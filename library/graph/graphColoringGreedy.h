#pragma once

#include "../set/bitSetVariable.h"

// undirected graph
struct GraphColoringGreedy {
    int N;
    vector<vector<int>> edges;

    GraphColoringGreedy() : N(0) {
    }

    GraphColoringGreedy(int n) : N(n), edges(N) {
    }

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(N);
    }

    // add edges to undirected graph
    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    // O(E*logV)
    vector<int> doVertexColoring() const {
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
};
