#include <cassert>
#include <limits>
#include <functional>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "rangeQuery-SparseTableOnTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

template <typename T, typename BinOp>
static T pathMinSlow(SparseTableOnTree<T, BinOp>& tree, int u, int v) {
    int lca = tree.findLCA(u, v);
    
    T res = tree.value[0][lca];
    while (u != lca) {
        res = min(res, tree.value[0][u]);
        u = tree.P[0][u];
    }
    while (v != lca) {
        res = min(res, tree.value[0][v]);
        v = tree.P[0][v];
    }

    return res;
}

template <typename T, typename BinOp>
static T pathMaxSlow(SparseTableOnTree<T, BinOp>& tree, int u, int v) {
    int lca = tree.findLCA(u, v);

    T res = tree.value[0][lca];
    while (u != lca) {
        res = max(res, tree.value[0][u]);
        u = tree.P[0][u];
    }
    while (v != lca) {
        res = max(res, tree.value[0][v]);
        v = tree.P[0][v];
    }

    return res;
}

template <typename T, typename BinOp>
static T pathSumSlow(SparseTableOnTree<T, BinOp>& tree, int u, int v) {
    int lca = tree.findLCA(u, v);

    T res = tree.value[0][lca];
    while (u != lca) {
        res += tree.value[0][u];
        u = tree.P[0][u];
    }
    while (v != lca) {
        res += tree.value[0][v];
        v = tree.P[0][v];
    }

    return res;
}

void testSparseTableOnTree() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Range Query - Sparse Table on Tree ----------------------------------" << endl;
    {
        int maxV = 1000000000;
        int N = 10000;
        int T = 10000;
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % maxV;

        auto tree = makeSparseTableOnTree(N, [](int a, int b) { return min(a, b); }, maxV);

        tree.setValue(0, in[0]);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
            tree.setValue(v, in[v]);
        }

        tree.build(0);
        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;

            int gt = pathMinSlow(tree, u, v);
            int ans = tree.query(u, v);
            assert(gt == ans);
        }
    }
    {
        int maxV = 1000000000;
        int N = 10000;
        int T = 10000;
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % maxV;

        auto tree = makeSparseTableOnTree(N, [](int a, int b) { return max(a, b); }, maxV);

        tree.setValue(0, in[0]);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
            tree.setValue(v, in[v]);
        }

        tree.build(0);
        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;

            int gt = pathMaxSlow(tree, u, v);
            int ans = tree.query(u, v);
            assert(gt == ans);
        }
    }
    {
        int maxV = 10000;
        int N = 10000;
        int T = 10000;
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % maxV;

        auto tree = makeSparseTableOnTree(N, [](int a, int b) { return a + b; }, 0);

        tree.setValue(0, in[0]);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
            tree.setValue(v, in[v]);
        }

        tree.build(0);
        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;

            int gt = pathSumSlow(tree, u, v);
            int ans = tree.queryNoOverlap(u, v);
            assert(gt == ans);
        }
    }

    cout << "OK!" << endl;
}
