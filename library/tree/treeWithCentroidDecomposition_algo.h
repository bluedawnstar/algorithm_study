#pragma once

#include "treeWithCentroidDecomposition.h"

// http://codeforces.com/contest/342/problem/E
struct MinDistanceToSpecialNode {
    vector<int> dist;       // minimum distance between a node and any node in the centroid-decomposition subtree

    MinDistanceToSpecialNode() {
    }

    explicit MinDistanceToSpecialNode(TreeWithCentroidDecomposition& tree) {
        init(tree);
    }

    void init(TreeWithCentroidDecomposition& tree) {
        dist.assign(tree.N, tree.N * 2);
    }


    void update(TreeWithCentroidDecomposition& tree, int u) {
        int v = u;
        while (v >= 0) {
            dist[v] = min(dist[v], tree.distance(u, v));
            v = tree.ctParent[v];
        }
    }

    int query(TreeWithCentroidDecomposition& tree, int u) {
        int res = dist[u];
        int v = u;
        while (v >= 0) {
            res = min(res, tree.distance(u, v) + dist[v]);
            v = tree.ctParent[v];
        }
        return res;
    }
};

// http://codeforces.com/contest/321/problem/C
