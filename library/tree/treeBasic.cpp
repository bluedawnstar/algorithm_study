#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "treeBasic.h"

/* // TODO: adding this functions to find centers of a tree
vector<int> gDist;
void dfsDist(int v, int p, int d) {
    gDist[v] = d;
    for (int to : gE[v]) {
        if (to != p) {
            dfsDist(to, v, d + 1);
        }
    }
}

int findMax() {
    int index = -1;
    for (int i = 0; i < gN; i++) {
        if (gDist[i] != gN && (index == -1 || gDist[index] < gDist[i]))
            index = i;
    }
    return index;
}

vector<int> gPath;
bool dfsPath(int v, int to, int p) {
    if (v == to) {
        gPath.push_back(to);
        return true;
    }
    gPath.push_back(v);
    for (int u : gE[v]) {
        if (p != u && dfsPath(u, to, v)) {
            return true;
        }
    }
    gPath.pop_back();
    return false;
}

vector<int> findCenters(int v) {
    gDist = vector<int>(gN + 10, gN);
    dfsDist(v, -1, 0);
    v = findMax();
    dfsDist(v, -1, 0);
    int to = findMax();
    gPath.clear();
    dfsPath(v, to, -1);
    if (gPath.size() % 2 == 0) {
        return vector<int>{ gPath[(int)gPath.size() / 2 - 1], gPath[(int)gPath.size() / 2] };
    } else {
        return vector<int>{ gPath[(int)gPath.size() / 2] };
    }
}
*/

/* //TODO: add this functions for hashing of a tree
private static final int B1 = 31;
private static final int M1 = 1000000007;
private static final int Z1 = 16983;
private static final int B2 = 37;
private static final int M2 = 1000000009;
private static final int Z2 = 18657;

public static long unrootedHash(int[][] g) {
    int[] cs = center(g);
    long[] lhs = new long[cs.length];
    int p = 0;
    for (int c : cs) {
        lhs[p++] = rootedHash(c, -1, g);
    }
    Arrays.sort(lhs);
    long hl = Z1, hr = Z2;
    for (long lh : lhs) {
        long lhl = lh >> >32, lhr = (int)lh;
        hl = (hl * B1 + lhl*lhl) % M1;
        hr = (hr * B2 + lhr*lhr) % M2;
    }
    return hl << 32 | hr;
}

public static long rootedHash(int cur, int pre, int[][] g) {
    long[] hs = new long[g[cur].length];
    int p = 0;
    for (int e : g[cur]) {
        if (e != pre) {
            hs[p++] = rootedHash(e, cur, g);
        }
    }
    Arrays.sort(hs, 0, p);
    long hl = Z1, hr = Z2;
    for (int i = 0; i < p; i++) {
        hl = (hl * B1 + hs[i] * hs[i]) % M1;
        hr = (hr * B2 + hs[i] * hs[i]) % M2;
    }
    return hl << 32 | hr;
}

//--- OR ---

long dfsHash(int v, int p) {
    int count = 0;
    for (int to : g[v]) {
        if (p != to) {
            count++;
        }
    }
    long[] result = new long[count];
    int len = 0;
    for (int to : g[v]) {
        if (p != to) {
            result[len++] = dfsHash(to, v);
        }
    }
    Arrays.sort(result);
    long ans = 4242424242424243L;
    for (int i = 0; i < len; i++) {
        ans ^= rnd[i] * result[i];
    }
    return ans;
}
*/

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#define MAXN    10000           // TODO: modify the maximum number of nodes
#define LOGN    15              // TODO: modify LCA table size (log2(MAXN))

static void makeTree(Tree& tree) {
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

static void makeLcaTree(Tree& tree) {
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

static void printTree(Tree& tree) {
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
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Tree Basic ----------------------------------" << endl;

    Tree tree(MAXN, LOGN);

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
    }
    PROFILE_STOP(0);
    if (!errCnt)
        cout << "OK!" << endl;
}
