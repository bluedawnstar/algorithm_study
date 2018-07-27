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


static int lcaNaive(const vector<int>& parent, int u, int v) {
    vector<int> ancestorU;
    while (u >= 0) {
        ancestorU.push_back(u);
        u = parent[u];
    }

    vector<int> ancestorV;
    while (v >= 0) {
        ancestorV.push_back(v);
        v = parent[v];
    }

    int res = ancestorU.back();
    for (int i = int(ancestorU.size()) - 1, j = int(ancestorV.size()) - 1; i >= 0 && j >= 0; i--, j--) {
        if (ancestorU[i] != ancestorV[j])
            break;
        res = ancestorU[i];
    }

    return res;
}

static int pathQueryNaive(const vector<int>& parent, int u, int v, vector<int>& values) {
    if (u == v)
        return values[u];

    auto lca = lcaNaive(parent, u, v);

    int res = 0;

    while (u != lca) {
        res += values[u];
        u = parent[u];
    }
    while (v != lca) {
        res += values[v];
        v = parent[v];
    }

    return res + values[lca];
}

static int subtreeQueryNaive(const vector<vector<int>>& edges, const vector<int>& parent, int u, int p, vector<int>& values) {
    int res = values[u];

    for (auto v : edges[u]) {
        if (v != p)
            res += subtreeQueryNaive(edges, parent, v, u, values);
    }

    return res;
}


void testBlockTreeSubtreeUpdateDynamic() {
    return; //TODO: if you want to test a split function, make this line to a comment.

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
            int u = RandInt32::get() % N;
            int val = RandInt32::get() % 100;
            updateSubtreeNaive(tree, values, u, tree.P[0][u], val);
            treeBlock.updateSubtree(u, val);

            u = RandInt32::get() % N;
            int v = RandInt32::get() % N;

            auto gt1 = querySubtreeNaive(tree, values, u, tree.P[0][u]);
            auto ans1 = treeBlock.querySubtree(u);
            if (gt1 != ans1)
                cout << "ERROR" << endl;
            assert(gt1 == ans1);

            auto gt2 = pathQueryNaive(tree, values, u, v);
            assert(gt2 == treeBlock.query(u, v));
        }
    }
    // test dynamic update
    {
        int N = 10000;
        int T = 10000;

        auto treeBlock = makeDynamicBlockTreeSubtreeUpdate(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        vector<vector<int>> edges(N);
        vector<int> parent(N, -1);
        vector<int> values(N);

        values[0] = RandInt32::get() & 1;
        treeBlock.setInitValue(0, values[0]);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            treeBlock.addEdge(u, v);
            edges[u].push_back(v);
            edges[v].push_back(u);
            parent[v] = u;

            values[v] = RandInt32::get() & 1;
            treeBlock.setInitValue(v, values[v]);
        }

        treeBlock.build(0);

        for (int i = 0; i < T; i++) {
            int t = RandInt32::get() % N;
            values[t] = !values[t];
            treeBlock.update(t, values[t]);

            // change tree structure
            {
                int u, newP;
                do {
                    u = RandInt32::get() % N;
                    newP = RandInt32::get() % N;
                    if (newP > u)
                        swap(u, newP);
                } while (u > 0 && u == newP);
                treeBlock.changeParent(u, newP);

                if (parent[u] >= 0) {
                    int p = parent[u];
                    edges[p].erase(find(edges[p].begin(), edges[p].end(), u));
                    edges[u].erase(find(edges[u].begin(), edges[u].end(), p));
                }

                edges[newP].push_back(u);
                edges[u].push_back(newP);
                parent[u] = newP;
            }
            // query
            {
                int u = RandInt32::get() % N;
                int v = RandInt32::get() % N;

                auto gt1 = subtreeQueryNaive(edges, parent, u, parent[u], values);
                assert(gt1 == treeBlock.querySubtree(u));

                auto gt2 = pathQueryNaive(parent, u, v, values);
                assert(gt2 == treeBlock.query(u, v));
            }
        }
    }
    cout << "speed test of subtree update" << endl;
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
    {
        int N = 100000;
        int T = 100000;

        auto treeBlock = makeDynamicBlockTreeSubtreeUpdate(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        treeBlock.setInitValue(0, RandInt32::get() % 100);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            treeBlock.addEdge(u, v);
            treeBlock.setInitValue(v, RandInt32::get() % 100);
        }

        PROFILE_START(2);
        treeBlock.build(0);
        PROFILE_STOP(2);

        PROFILE_START(3);
        for (int i = 0; i < T; i++) {
            int t = RandInt32::get() % N;
            treeBlock.update(t, RandInt32::get() % 100);

            // change tree structure
            {
                int u, newP;
                do {
                    u = RandInt32::get() % N;
                    newP = RandInt32::get() % N;
                    if (newP > u)
                        swap(u, newP);
                } while (u > 0 && u == newP);
                treeBlock.changeParent(u, newP);
            }
            // query
            {
                int u = RandInt32::get() % N;
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
        }
        PROFILE_STOP(3);
    }
    cout << "OK!" << endl;
}
