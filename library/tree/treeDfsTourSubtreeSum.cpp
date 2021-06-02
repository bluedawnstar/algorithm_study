#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "treeDfsTourSubtreeSum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static void dfsAdd(DfsTourTreeSubtreeSum<int>& tree, vector<int>& values, int u, int p, int val) {
    values[u] += val;
    for (auto v : tree.edges[u]) {
        if (v != p)
            dfsAdd(tree, values, v, u, val);
    }
}

static int dfsSum(DfsTourTreeSubtreeSum<int>& tree, vector<int>& values, int u, int p) {
    int res = values[u];
    for (auto v : tree.edges[u]) {
        if (v != p)
            res += dfsSum(tree, values, v, u);
    }
    return res;
}

void testDfsTourTreeSubtreeSum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Tree Subtree Sum -----------------------------" << endl;
    {
        int N = 10000;
        int T = 10000;

        DfsTourTreeSubtreeSum<int> tree(N);
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
            dfsAdd(tree, values, u, tree.P[0][u], val);
            tree.add(u, val);
        }
    }

    cout << "OK!" << endl;
}