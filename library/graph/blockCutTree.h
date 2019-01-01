#pragma once

#include "bcc.h"

// https://en.wikipedia.org/wiki/Biconnected_component

// Undirected Graph
// Construct block-cut tree from biconnected components
struct BlockCutTree {
    int                 N;
    vector<vector<int>> edges;
    vector<bool>        cutV;
    vector<int>         graphToTreeID;

    void buildFromBCC(const BiconnectedComponents& bcc) {
        edges = vector<vector<int>>(bcc.N * 2);
        cutV = vector<bool>(bcc.N * 2);
        graphToTreeID = vector<int>(bcc.N * 2);

        N = 0;
        for (int u = 0; u < bcc.N; ++u) {
            if (bcc.cutV[u]) {
                graphToTreeID[u] = N++;
                cutV[graphToTreeID[u]] = true;
            }
        }
        for (auto& comp : bcc.comps) {
            int node = N++;
            for (int u : comp) {
                if (!bcc.cutV[u])
                    graphToTreeID[u] = node;
                else {
                    edges[node].push_back(graphToTreeID[u]);
                    edges[graphToTreeID[u]].push_back(node);
                }
            }
        }
    }

    void buildFromGraph(const vector<vector<int>>& edges, int N) {
        BiconnectedComponents bcc;
        bcc.findBCC(edges, N);
        buildFromBCC(bcc);
    }
};
