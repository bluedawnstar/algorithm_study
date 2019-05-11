#include <cassert>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>

using namespace std;

#include "treeBasic.h"
#include "treePathSqrtDecomposition.h"

// <Practice Problems>
// https://www.acmicpc.net/problem/12746

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "treeBlockTreePathQuery.h"

static void pathUpdateNaive(Tree& tree, int u, int v, int val, vector<int>& values) {
    if (tree.level[u] > tree.level[v])
        swap(u, v);

    while (tree.level[v] != tree.level[u]) {
        values[v] = val;
        v = tree.P[0][v];
    }

    while (u != v) {
        values[u] = val;
        values[v] = val;
        u = tree.P[0][u];
        v = tree.P[0][v];
    }

    values[u] = val;
}

static void pathAddNaive(Tree& tree, int u, int v, int val, vector<int>& values) {
    if (tree.level[u] > tree.level[v])
        swap(u, v);

    while (tree.level[v] != tree.level[u]) {
        values[v] += val;
        v = tree.P[0][v];
    }

    while (u != v) {
        values[u] += val;
        values[v] += val;
        u = tree.P[0][u];
        v = tree.P[0][v];
    }

    values[u] += val;
}

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

void testTreePathSqrtDecomposition() {
    //return; //TODO: if you want to test a split function, make this line to a comment.

    cout << "--- Tree Path Sqrt Decomposition ---------------------" << endl;
    {
        int N = 10000;
        int T = 10000;

        auto tree = makeTreePathSqrtDecomposition(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
        }

        tree.build(0);

        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;
            int gt = tree.findLCA(u, v);
            int ans = tree.lca2(u, v);
            if (gt != ans)
                cout << "Mismatched: lca(" << u << ", " << v << ") = " << gt << ", " << ans << endl;
            assert(gt == ans);
        }
    }
    {
        int N = 100000;
        int T = 1000000;
#ifdef _DEBUG
        N = 10000;
        T = 10000;
#endif

        auto tree = makeTreePathSqrtDecomposition(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
        }
        tree.build(0);

        vector<pair<int, int>> inLca(T);
        for (int i = 0; i < T; i++) {
            inLca[i].first = RandInt32::get() % N;
            inLca[i].second = RandInt32::get() % N;
        }

        vector<pair<int, int>> inClimb(T);
        for (int i = 0; i < T; i++) {
            inClimb[i].first = RandInt32::get() % N;
            inClimb[i].second = RandInt32::get() % (tree.level[inClimb[i].first] + 1);
        }

        PROFILE_START(0);
        int t = 0;
        for (int i = 0; i < T; i++) {
            t += tree.findLCA(inLca[i].first, inLca[i].second);
        }
        PROFILE_STOP(0);
        if (t == 0)
            cout << "To prevent removing the loop for optimization" << endl;

        PROFILE_START(1);
        t = 0;
        for (int i = 0; i < T; i++) {
            t += tree.lca2(inLca[i].first, inLca[i].second);
        }
        PROFILE_STOP(1);
        if (t == 0)
            cout << "To prevent removing the loop for optimization" << endl;
    }
    {
        int N = 10000;
        int T = 10000;

        auto tree = makeTreePathSqrtDecomposition(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
        }
        tree.build(0);

        vector<int> values(N);
        for (int v = 0; v < N; v++) {
            values[v] = RandInt32::get() & 1;
            tree.setInitValue(v, values[v]);
        }

        for (int i = 0; i < T; i++) {
            int t = RandInt32::get() % N;
            values[t] = !values[t];
            tree.update(t, t, values[t]);

            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;

            assert(tree.findLCA(u, v) == tree.lca2(u, v));

            auto gt = pathQueryNaive(tree, u, v, values);
            auto ans = tree.query(u, v);
            if (gt != ans)
                cout << "Mismatched: " << gt << ", " << ans << endl;
            assert(gt == ans);

            int uKth = RandInt32::get() % (tree.query(u, 0) + 1);
            int vKth = RandInt32::get() % (tree.query(v, 0) + 1);
            assert(climbKthNaive(tree, u, uKth, values) == tree.climbKth(u, uKth, [](int val) { return val; }));
            assert(climbKthNaive(tree, v, vKth, values) == tree.climbKth(v, vKth, [](int val) { return val; }));
        }
    }
    {
        int N = 10000;
        int T = 10000;

        auto tree = makeTreePathSqrtDecomposition(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
        }
        tree.build(0);

        vector<int> values(N);
        for (int v = 0; v < N; v++) {
            values[v] = RandInt32::get() & 1;
            tree.setInitValue(v, values[v]);
        }

        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;

            assert(tree.findLCA(u, v) == tree.lca2(u, v));

            u = RandInt32::get() % N;
            v = RandInt32::get() % N;
            auto gt = pathQueryNaive(tree, u, v, values);
            auto ans = tree.query(u, v);
            if (gt != ans)
                cout << "Mismatched: " << gt << ", " << ans << endl;
            assert(gt == ans);

            int val = RandInt32::get() % 100;
            pathUpdateNaive(tree, u, v, val, values);
            tree.update(u, v, val);
        }
    }
    cout << "Speed test : TreePathSqrtDecomposition vs. BlockTreePathQuery" << endl;
    {
        struct MergeOp {
            int operator()(int a, int b) const {
                return a + b;
            }
        };

        int N = 1000000;
        int T = 1000000;
#ifdef _DEBUG
        N = 10000;
        T = 10000;
#endif

        auto tree = makeTreePathSqrtDecomposition(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        auto treeSqrt = makeBlockTreePathQuery(N, MergeOp(), 0);

        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
            treeSqrt.addEdge(u, v);
        }
        tree.build(0);
        treeSqrt.build(0);

        for (int v = 0; v < N; v++) {
            int val = RandInt32::get() & 1;
            tree.setInitValue(v, val);
            treeSqrt.setInitValue(v, val);
        }

        vector<pair<int, int>> qry(T);
        vector<int> vals(T);
        for (int i = 0; i < T; i++) {
            qry[i].first = RandInt32::get() % N;
            qry[i].second = RandInt32::get() % N;
            vals[i] = RandInt32::get() % 100;
        }

        PROFILE_START(0);
        for (int i = 0; i < T; i++) {
            tree.update(qry[i].first, qry[i].first, vals[i]);
            if (tree.values[0] < 0)
                cout << "This if statements are dummy codes to prevent from optimization" << endl;
        }
        PROFILE_STOP(0);

        PROFILE_START(1);
        for (int i = 0; i < T; i++) {
            treeSqrt.update(qry[i].first, vals[i]);
            if (treeSqrt.values[0] < 0)
                cout << "This if statements are dummy codes to prevent from optimization" << endl;
        }
        PROFILE_STOP(1);

        PROFILE_START(2);
        for (int i = 0; i < T; i++) {
            if (tree.query(qry[i].first, qry[i].second) < 0)
                cout << "This if statements are dummy codes to prevent from optimization" << endl;
        }
        PROFILE_STOP(2);

        PROFILE_START(3);
        for (int i = 0; i < T; i++) {
            if (treeSqrt.query(qry[i].first, qry[i].second) < 0)
                cout << "This if statements are dummy codes to prevent from optimization" << endl;
        }
        PROFILE_STOP(3);
    }


    cout << "OK!" << endl;
}
