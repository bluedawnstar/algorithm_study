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

#define MAXN    10000           // TODO: modify the maximum number of nodes
#define LOGN    15              // TODO: modify LCA table size (log2(MAXN))

static void makeTree(Tree<MAXN, LOGN>& tree) {
    tree.setVertexCount(10);
    
    tree.addEdge(0, 1);
    tree.addEdge(0, 3);
    tree.addEdge(1, 4);
    tree.addEdge(1, 2);
    tree.addEdge(3, 6);
    tree.addEdge(3, 7);
    tree.addEdge(4, 9);
    tree.addEdge(2, 8);
    tree.addEdge(2, 5);
}

static void makeLcaTree(Tree<MAXN, LOGN>& tree) {
    tree.setVertexCount(MAXN);

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
}

static void printTree(Tree<MAXN, LOGN>& tree) {
    cout << "level : ";
    for (int i = 0; i < tree.mN; i++)
        cout << tree.mLevel[i] << ", ";
    cout << endl;

    cout << "parent : ";
    for (int i = 0; i < tree.mN; i++)
        cout << tree.mP[i][0] << ", ";
    cout << endl;
}

void testTreeBasic() {
    //return; //TODO: if you want to test a split function, make this line to a comment.

    Tree<MAXN, LOGN> tree;

    cout << "-- dfs() vs dfsIter() ----------------------------------" << endl;
    tree.clear();
    makeTree(tree); // make a test tree
    tree.dfs(0, -1);
    printTree(tree);

    makeTree(tree);
    tree.dfsIter(0);
    printTree(tree);

    /*
    cout << "-- dfs() vs dfsIter() - performance test ---------------" << endl;
    tree.clear();
    makeTree(tree); // make a test tree
    clock_t start = clock();
    for (int i = 0; i < 1000000; i++) {
        tree.dfs(1, 0);
    }
    cout << "elapsed time : " << double(clock() - start) / CLOCKS_PER_SEC << endl;

    tree.clear();
    makeTree(tree); // make a test tree
    start = clock();
    for (int i = 0; i < 1000000; i++) {
        tree.dfsIter(1);
    }
    cout << "elapsed time : " << double(clock() - start) / CLOCKS_PER_SEC << endl;
    */

    cout << "-- LCA test --------------------------------------------" << endl;
    tree.clear();       // step1: clear all variables
    makeLcaTree(tree);  // ... make a test tree

    //tree.dfs(0, -1);
    tree.dfsIter(0);    // step2: make depth and parent table
    tree.makeLcaTable();// step3: make LCA table

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
    }
    if (!errCnt)
        cout << "OK!" << endl;
}
