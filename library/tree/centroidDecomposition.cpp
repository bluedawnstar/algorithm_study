#include <vector>
#include <queue>

using namespace std;

#include "treeBasic.h"
#include "centroidDecomposition.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static Tree makeTree() {
    Tree tree(16, 4);
    tree.addEdge(0, 3);
    tree.addEdge(1, 3);
    tree.addEdge(2, 3);
    tree.addEdge(3, 4);
    tree.addEdge(4, 5);
    tree.addEdge(6, 5);
    tree.addEdge(7, 6);
    tree.addEdge(8, 6);
    tree.addEdge(5, 9);
    tree.addEdge(9, 10);
    tree.addEdge(10, 11);
    tree.addEdge(11, 13);
    tree.addEdge(10, 12);
    tree.addEdge(12, 14);
    tree.addEdge(12, 15);
    return tree;
}

static Tree makeTree2() {
    Tree tree(15, 4);
    tree.addEdge(0, 1);
    tree.addEdge(2, 1);
    tree.addEdge(3, 1);
    tree.addEdge(1, 4);
    tree.addEdge(4, 5);
    tree.addEdge(5, 6);
    tree.addEdge(4, 7);
    tree.addEdge(7, 8);
    tree.addEdge(8, 9);
    tree.addEdge(9, 10);
    tree.addEdge(9, 11);
    tree.addEdge(8, 12);
    tree.addEdge(12, 13);
    tree.addEdge(12, 14);
    return tree;
}

static void dumpTree(vector<vector<int>>& tree, int N) {
    cout << "graph {" << endl;
    for (int u = 0; u < N; u++) {
        for (int v : tree[u]) {
            if (u < v)
                cout << "    " << (u + 1) << " -- " << (v + 1) << endl;
        }
    }
    cout << "}" << endl;
}

#define MAXN    1000
#define LOGN    11

void testCentroidDecomposition() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Centroid Decomposition ---------------------------" << endl;
    cout << "*** Tree #1 ***" << endl;
    {
        auto tree = makeTree(); // make a test tree
        dumpTree(tree.edges, 16);

        CentroidDecomposition cd;

        cd.decomposeTree(tree, 0);
        cout << "root = " << (cd.root + 1) << endl;
        dumpTree(cd.edges, 16);
    }

    cout << "*** Tree #2 ***" << endl;
    {
        auto tree = makeTree2(); // make a test tree
        dumpTree(tree.edges, 15);

        CentroidDecomposition cd;

        cd.decomposeTree(tree, 0);
        cout << "root = " << (cd.root) << endl;
        dumpTree(cd.edges, 15);
    }
}
