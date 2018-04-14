#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "treeBasic.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#define MAXN    10000           // TODO: modify the maximum number of nodes

static Tree makeTree() {
    Tree tree(10);

    tree.addEdge(0, 1);
    tree.addEdge(0, 3);
    tree.addEdge(1, 4);
    tree.addEdge(1, 2);
    tree.addEdge(3, 6);
    tree.addEdge(3, 7);
    tree.addEdge(4, 9);
    tree.addEdge(2, 8);
    tree.addEdge(2, 5);

    return tree;
}

static Tree makeLcaTree() {
    Tree tree(MAXN);

    tree.addEdge(0, 1);
    tree.addEdge(0, 2);

    int i, p = 1;
    for (i = 3; i < tree.N / 4; i++) {
        tree.addEdge(p, i);
        p = i;
    }

    p = 1;
    for (; i < tree.N * 2 / 4; i++) {
        tree.addEdge(p, i);
        p = i;
    }

    p = 2;
    for (; i < tree.N * 3 / 4; i++) {
        tree.addEdge(p, i);
        p = i;
    }

    p = 2;
    for (; i < tree.N; i++) {
        tree.addEdge(p, i);
        p = i;
    }

    return tree;
}

static Tree makeTreeForCenter() {
    Tree tree(4);

    tree.addEdge(0, 3);
    tree.addEdge(3, 1);
    tree.addEdge(2, 3);

    return tree;
}

static void printTree(Tree& tree) {
    cout << "level : ";
    for (int i = 0; i < tree.N; i++)
        cout << tree.level[i] << ", ";
    cout << endl;

    cout << "parent : ";
    for (int i = 0; i < tree.N; i++)
        cout << tree.P[0][i] << ", ";
    cout << endl;
}

void testTreeBasic() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Tree Basic ----------------------------------" << endl;

    cout << "* dfs() vs dfsIter()" << endl;
    {
        auto tree = makeTree(); // make a test tree
        tree.dfs(0, -1);
        printTree(tree);
    }
    {
        auto tree = makeTree();
        tree.dfsIter(0);
        printTree(tree);
    }
    cout << "* centroid, center, diameter" << endl;
    {
        auto tree = makeTreeForCenter();
        tree.build(0);
        
        assert(tree.findCentroid(0) == 3);

        auto center = tree.findCenters(0);
        assert(center.size() == 1 && center[0] == 3);

        auto centerEx = tree.findCentersEx();
        assert(centerEx.size() == 1 && centerEx[0] == 3);

        auto diam = tree.getDiameter();
        assert(diam == 2);
    }
    cout << "* LCA test" << endl;
    {
        auto tree = makeLcaTree();  // make a test tree
        tree.build(0);

        PROFILE_START(0);
        int errCnt = 0;
        for (int i = 0; i < 100000; i++) {
            int u = RandInt32::get() % tree.N;
            int v = RandInt32::get() % tree.N;
            int lca = tree.findLCA(u, v);
            int lcaAns;
            if (u == 0 || v == 0) {
                lcaAns = 0;
            } else if ((u != 2 && u < tree.N / 2) != (v != 2 && v < tree.N / 2)) {
                lcaAns = 0;
            } else if (u != 2 && u < tree.N / 2) {
                if ((u >= tree.N / 4) != (v >= tree.N / 4))
                    lcaAns = 1;
                else
                    lcaAns = min(u, v);
            } else {
                if ((u >= tree.N * 3 / 4) != (v >= tree.N * 3 / 4))
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
    cout << "OK!" << endl;
}
