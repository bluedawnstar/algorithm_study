#include <cassert>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

#include "treeSqrtDecompositionPathQuery.h"

// Ref: https://www.geeksforgeeks.org/sqrt-square-root-decomposition-set-2-lca-tree-osqrth-time/

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include <queue>
#include "treeBasic.h"

struct MergeOp {
    int operator()(int a, int b) const {
        return a + b;
    }
};

static int pathQueryNaive(Tree& tree, int u, int v, vector<int>& values) {
    if (u == v)
        return values[u];

    if (tree.level[u] > tree.level[v])
        swap(u, v);

    int res = 0;
    while (tree.level[v] != tree.level[u]) {
        res += values[v];
        v = tree.P[0][v];
    }

    while (u != v) {
        res += values[u] + values[v];
        u = tree.P[0][u];
        v = tree.P[0][v];
    }

    return res + values[u];
}

static int climbKthNaive(Tree& tree, int u, int kth, vector<int>& values) {
    while (u >= 0 && kth > 0) {
        if (values[u]) {
            if (--kth == 0)
                break;
        }
        u = tree.P[0][u];
    }
    return u;
}

void testTreeSqrtDecompositionPathQuery() {
    return; //TODO: if you want to test a split function, make this line to a comment.

    cout << "--- Tree with Sqrt Decomposition and Path Query ---------------------" << endl;
    {
        int N = 10000;
        int T = 10000;

        Tree tree(N);
        auto treeSqrt = makeTreeSqrtDecompositionPathQuery(N, MergeOp(), 0);

        vector<int> values(N);

        values[0] = RandInt32::get() & 1;
        treeSqrt.setInitValue(0, values[0]);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
            treeSqrt.addEdge(u, v);

            values[v] = RandInt32::get() & 1;
            treeSqrt.setInitValue(v, values[v]);
        }

        tree.build(0);
        treeSqrt.build(0);

        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;
            assert(tree.findLCA(u, v) == treeSqrt.lca(u, v));
        }

        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;
            int d = RandInt32::get() % (tree.level[u] + 1);
            assert(tree.climbTree(u, d) == treeSqrt.climb(u, d));
        }

        for (int i = 0; i < T; i++) {
            int t = RandInt32::get() % N;
            values[t] = !values[t];
            treeSqrt.update(t, values[t]);

            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;
            auto gt = pathQueryNaive(tree, u, v, values);
            assert(gt == treeSqrt.query(u, v));
            assert(gt == treeSqrt.queryAccumulative(u, v));

            int uKth = RandInt32::get() % (treeSqrt.queryToRoot(u) + 1);
            int vKth = RandInt32::get() % (treeSqrt.queryToRoot(v) + 1);
            assert(climbKthNaive(tree, u, uKth, values) == treeSqrt.climbKth(u, uKth, [](int val) { return val; }));
            assert(climbKthNaive(tree, v, vKth, values) == treeSqrt.climbKth(v, vKth, [](int val) { return val; }));
        }
    }

    cout << "OK!" << endl;
}
