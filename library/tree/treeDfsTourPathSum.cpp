#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "treeDfsTourPathSum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int pathSumNaive(DfsTourTreePathSum<int>& tree, vector<int>& values, int u, int v) {
    if (u == v)
        return values[u];

    int lca = tree.findLCA(u, v);
    int sumU = values[u];
    while (u != lca) {
        u = tree.P[0][u];
        sumU += values[u];
    }

    int sumV = values[v];
    while (v != lca) {
        v = tree.P[0][v];
        sumV += values[v];
    }

    return sumU + sumV - values[lca];
}

void testDfsTourTreePathSum() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Tree Path Sum -----------------------------" << endl;
    {
        int N = 10000;
        int T = 10000;

        DfsTourTreePathSum<int> tree(N);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
        }
        tree.build(0);

        vector<int> values(N);
        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;
            int gt = pathSumNaive(tree, values, u, v);
            int ans = tree.query(u, v);
            if (gt != ans)
                cout << "Mismatch: " << gt << ", " << ans << endl;
            assert(gt == ans);

            int val = RandInt32::get() % 100 - 50;
            values[u] = val;
            tree.set(u, val);
        }
    }
    {
        int N = 10000;
        int T = 10000;

        DfsTourTreePathSum<int> tree(N);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
        }
        tree.build(0);

        vector<int> values(N);
        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % N;
            int v = RandInt32::get() % N;
            int gt = pathSumNaive(tree, values, u, v);
            int ans = tree.query(u, v);
            if (gt != ans)
                cout << "Mismatch: " << gt << ", " << ans << endl;
            assert(gt == ans);

            int val = RandInt32::get() % 100 - 50;
            values[u] += val;
            tree.add(u, val);
        }
    }

    cout << "OK!" << endl;
}