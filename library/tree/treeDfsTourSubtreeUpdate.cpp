#include <memory.h>
#include <functional>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "treeDfsTourSubtreeUpdate.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static void dfsUpdate(DfsTourTreeSubtreeUpdate<int>& tree, vector<int>& values, int u, int p, int val) {
    values[u] = val;
    for (auto v : tree.edges[u]) {
        if (v != p)
            dfsUpdate(tree, values, v, u, val);
    }
}

static int dfsSum(DfsTourTreeSubtreeUpdate<int>& tree, vector<int>& values, int u, int p) {
    int res = values[u];
    for (auto v : tree.edges[u]) {
        if (v != p)
            res += dfsSum(tree, values, v, u);
    }
    return res;
}

static int dfsMax(DfsTourTreeSubtreeUpdate<int>& tree, vector<int>& values, int u, int p) {
    int res = values[u];
    for (auto v : tree.edges[u]) {
        if (v != p)
            res = max(res, dfsMax(tree, values, v, u));
    }
    return res;
}

void testDfsTourTreeSubtreeUpdate() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Tree Subtree Update -----------------------------" << endl;
    {
        int N = 10000;
        int T = 20000;

        DfsTourTreeSubtreeUpdate<int> tree(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
        }
        tree.build(0);

        vector<int> values(N);
        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;

            int gt = dfsSum(tree, values, u, tree.P[0][u]);
            int ans = tree.query(u);
            if (gt != ans)
                cout << "Mismatch: " << gt << ", " << ans << endl;
            assert(gt == ans);

            int val = RandInt32::get() % 100 - 50;
            dfsUpdate(tree, values, u, tree.P[0][u], val);
            tree.update(u, val);
        }
    }
    {
        int N = 10000;
        int T = 10000;

        DfsTourTreeSubtreeUpdate<int> tree(N, [](int a, int b) { return max(a, b); }, [](int x, int n) { return x; }, 0);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
        }
        tree.build(0);

        vector<int> values(N);
        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;

            int gt = dfsMax(tree, values, u, tree.P[0][u]);
            int ans = tree.query(u);
            if (gt != ans)
                cout << "Mismatch: " << gt << ", " << ans << endl;
            assert(gt == ans);

            int val = RandInt32::get() % 100;
            dfsUpdate(tree, values, u, tree.P[0][u], val);
            tree.update(u, val);
        }
    }

    cout << "OK!" << endl;
}