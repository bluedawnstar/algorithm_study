#include <cassert>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

#include "treeBlockTreeLCA.h"

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

void testBlockTreeLCA() {
    return; //TODO: if you want to test a split function, make this line to a comment.

    cout << "--- Block Tree for LCA ---------------------" << endl;
    {
        int N = 10000;
        int T = 10000;
        
        Tree tree(N);
        BlockTreeLCA treeSqrt(N);

        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
            treeSqrt.addEdge(u, v);
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
    }
    {
        int N = 100000;
        int T = 1000000;
#ifdef _DEBUG
        N = 10000;
        T = 10000;
#endif

        Tree tree(N);
        BlockTreeLCA treeSqrt(N);

        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            tree.addEdge(u, v);
            treeSqrt.addEdge(u, v);
        }

        tree.build(0);
        treeSqrt.build(0);

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
            t += treeSqrt.lca(inLca[i].first, inLca[i].second);
        }
        PROFILE_STOP(1);
        if (t == 0)
            cout << "To prevent removing the loop for optimization" << endl;

        PROFILE_START(2);
        t = 0;
        for (int i = 0; i < T; i++) {
            t += tree.climbTree(inClimb[i].first, inClimb[i].second);
        }
        PROFILE_STOP(2);
        if (t == 0)
            cout << "To prevent removing the loop for optimization" << endl;

        PROFILE_START(3);
        t = 0;
        for (int i = 0; i < T; i++) {
            t += treeSqrt.climb(inClimb[i].first, inClimb[i].second);
        }
        PROFILE_STOP(3);
        if (t == 0)
            cout << "To prevent removing the loop for optimization" << endl;
    }

    cout << "OK!" << endl;
}
