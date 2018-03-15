#include <cmath>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "lcaSchieberVishkin.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#include "treeBasic.h"

#define MAXN    10000           // TODO: modify the maximum number of nodes
#define LOGN    15              // TODO: modify LCA table size (log2(MAXN))

static Tree makeLcaTree() {
    Tree tree(MAXN, LOGN);

    tree.addEdge(0, 1);
    tree.addEdge(0, 2);

    int i, p = 1;
    for (i = 3; i < tree.mN / 4; i++) {
        tree.addEdge(p, i);
        p = i;
    }

    p = 1;
    for (; i < tree.mN * 2 / 4; i++) {
        tree.addEdge(p, i);
        p = i;
    }

    p = 2;
    for (; i < tree.mN * 3 / 4; i++) {
        tree.addEdge(p, i);
        p = i;
    }

    p = 2;
    for (; i < tree.mN; i++) {
        tree.addEdge(p, i);
        p = i;
    }

    return tree;
}

void testLcaShieberVishkin() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- LCA with Shieber Vishkin algorithm ------------------------------" << endl;
    {
        auto tree = makeLcaTree();  // make a test tree
        tree.build(0);

        PROFILE_START(0);
        int errCnt = 0;
        for (int i = 0; i < 100000; i++) {
            int u = rand() % tree.mN;
            int v = rand() % tree.mN;
            int lca = tree.findLCA(u, v);
            int lcaAns;
            if (u == 0 || v == 0) {
                lcaAns = 0;
            } else if ((u != 2 && u < tree.mN / 2) != (v != 2 && v < tree.mN / 2)) {
                lcaAns = 0;
            } else if (u != 2 && u < tree.mN / 2) {
                if ((u >= tree.mN / 4) != (v >= tree.mN / 4))
                    lcaAns = 1;
                else
                    lcaAns = min(u, v);
            } else {
                if ((u >= tree.mN * 3 / 4) != (v >= tree.mN * 3 / 4))
                    lcaAns = 2;
                else
                    lcaAns = min(u, v);
            }
            if (lca != lcaAns) {
                cout << "mismatch : LCA(" << u << ", " << v << ") = " << lca << " (!= " << lcaAns << ")" << endl;
                errCnt++;
            }
            assert(lca == lcaAns);
        }
        PROFILE_STOP(0);
    }
    {
        auto tree = makeLcaTree();  // make a test tree

        LcaSchieberVishkin svLCA(tree.mE, 0);

        PROFILE_START(1);
        int errCnt = 0;
        for (int i = 0; i < 100000; i++) {
            int u = rand() % tree.mN;
            int v = rand() % tree.mN;
            int lca = svLCA.lca(u, v);
            int lcaAns;
            if (u == 0 || v == 0) {
                lcaAns = 0;
            } else if ((u != 2 && u < tree.mN / 2) != (v != 2 && v < tree.mN / 2)) {
                lcaAns = 0;
            } else if (u != 2 && u < tree.mN / 2) {
                if ((u >= tree.mN / 4) != (v >= tree.mN / 4))
                    lcaAns = 1;
                else
                    lcaAns = min(u, v);
            } else {
                if ((u >= tree.mN * 3 / 4) != (v >= tree.mN * 3 / 4))
                    lcaAns = 2;
                else
                    lcaAns = min(u, v);
            }
            if (lca != lcaAns) {
                cout << "mismatch : LCA(" << u << ", " << v << ") = " << lca << " (!= " << lcaAns << ")" << endl;
                errCnt++;
            }
            assert(lca == lcaAns);
        }
        PROFILE_STOP(1);
    }

    cout << "OK!" << endl;
}
