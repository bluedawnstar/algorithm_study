#include <cassert>
#include <vector>
#include <set>
#include <functional>
#include <algorithm>

using namespace std;

#include "treeBlockTreeSubtreeUpdateDynamic.h"

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

static int pathQueryNaive(Tree& tree, vector<int>& values, int u, int v) {
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

static int climbKthNaive(Tree& tree, vector<int>& values, int u, int kth) {
    while (u >= 0 && kth > 0) {
        if (values[u]) {
            if (--kth == 0)
                break;
        }
        u = tree.P[0][u];
    }
    return u;
}

static void updateSubtreeNaive(Tree& tree, vector<int>& values, int u, int parent, int val) {
    values[u] = val;
    for (auto v : tree.edges[u]) {
        if (v != parent)
            updateSubtreeNaive(tree, values, v, u, val);
    }
}

static void addSubtreeNaive(Tree& tree, vector<int>& values, int u, int parent, int val) {
    values[u] += val;
    for (auto v : tree.edges[u]) {
        if (v != parent)
            addSubtreeNaive(tree, values, v, u, val);
    }
}

static int querySubtreeNaive(Tree& tree, vector<int>& values, int u, int parent) {
    int res = values[u];
    for (auto v : tree.edges[u]) {
        if (v != parent)
            res += querySubtreeNaive(tree, values, v, u);
    }
    return res;
}

void testBlockTreeSubtreeUpdateDynamic() {
    //return; //TODO: if you want to test a split function, make this line to a comment.

    cout << "--- Dynamic Block Tree for Path & Subtree Query ---------------------" << endl;
    {
        int N = 10000;
        int T = 10000;

        Tree tree(N);
        auto treeBlock = makeDynamicBlockTreeSubtreeUpdate(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        vector<int> values(N);

        values[0] = RandInt32::get() & 1;
        treeBlock.setInitValue(0, values[0]);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
            treeBlock.addEdge(u, v);

            values[v] = RandInt32::get() & 1;
            treeBlock.setInitValue(v, values[v]);
        }

        tree.build(0);
        treeBlock.build(0);

        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;
            assert(tree.findLCA(u, v) == treeBlock.lca(u, v));
        }

        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;
            int d = RandInt32::get() % (tree.level[u] + 1);
            assert(tree.climbTree(u, d) == treeBlock.climb(u, d));
        }

        for (int i = 0; i < T; i++) {
            int t = RandInt32::get() % N;
            values[t] = !values[t];
            treeBlock.update(t, values[t]);

            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;
            auto gt = pathQueryNaive(tree, values, u, v);
            assert(gt == treeBlock.query(u, v));
            assert(gt == treeBlock.queryAccumulative(u, v));

            int uKth = RandInt32::get() % (treeBlock.queryToRoot(u) + 1);
            int vKth = RandInt32::get() % (treeBlock.queryToRoot(v) + 1);
            assert(climbKthNaive(tree, values, u, uKth) == treeBlock.climbKth(u, uKth, [](int val) { return val; }));
            assert(climbKthNaive(tree, values, v, vKth) == treeBlock.climbKth(v, vKth, [](int val) { return val; }));
        }
    }
    {
        int N = 10000;
        int T = 10000;

        Tree tree(N);
        auto treeBlock = makeDynamicBlockTreeSubtreeUpdate(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        vector<int> values(N);

        values[0] = RandInt32::get() & 1;
        treeBlock.setInitValue(0, values[0]);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
            treeBlock.addEdge(u, v);

            values[v] = RandInt32::get() % 100;
            treeBlock.setInitValue(v, values[v]);
        }

        tree.build(0);
        treeBlock.build(0);

        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;
            assert(tree.findLCA(u, v) == treeBlock.lca(u, v));
        }

        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;
            int d = RandInt32::get() % (tree.level[u] + 1);
            assert(tree.climbTree(u, d) == treeBlock.climb(u, d));
        }

        for (int i = 0; i < T; i++) {
            //if ((i + 1) % treeBlock.sqrtN == 0)
            //    treeBlock.rebuild(0);

            int u = RandInt32::get() % N;
            int val = RandInt32::get() % 100;
            updateSubtreeNaive(tree, values, u, tree.P[0][u], val);
            treeBlock.updateSubtree(u, val);

            u = RandInt32::get() % N;
            int v = RandInt32::get() % N;
#if 1
            auto gt = querySubtreeNaive(tree, values, u, tree.P[0][u]);
            auto ans = treeBlock.querySubtree(u);
            if (gt != ans)
                cout << "ERROR" << endl;
            assert(gt == ans);
#else
            auto gt = pathQueryNaive(tree, values, u, v);
            assert(gt == treeBlock.query(u, v));
            assert(gt == treeBlock.queryAccumulative(u, v));
#endif
        }
    }
    {
        int N = 100000;
        int T = 100000;

        auto treeBlock = makeDynamicBlockTreeSubtreeUpdate(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        vector<int> values(N);

        treeBlock.setInitValue(0, values[0]);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            treeBlock.addEdge(u, v);
            treeBlock.setInitValue(v, RandInt32::get() % 100);
        }

        PROFILE_START(0);
        treeBlock.build(0);
        PROFILE_STOP(0);

        PROFILE_START(1);
        for (int i = 0; i < T; i++) {
            //if ((i + 1) % treeBlock.sqrtN == 0)
            //    treeBlock.rebuild(0);

            int u = RandInt32::get() % N;
            int val = RandInt32::get() % 100;
            treeBlock.updateSubtree(u, val);

            u = RandInt32::get() % N;
            int v = RandInt32::get() % N;
#if 1
            auto ans = treeBlock.querySubtree(u);
            if (ans < 0)
                cout << "ERROR" << endl;
#else
            auto ans = treeBlock.query(u, v);
            if (ans < 0)
                cout << "ERROR" << endl;
#endif
        }
        PROFILE_STOP(1);
    }

    cout << "OK!" << endl;
}
