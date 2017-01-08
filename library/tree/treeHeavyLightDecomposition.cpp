#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <functional>
#include <algorithm>

using namespace std;

#include "treeSegmentTree.h"

namespace TreeHeavyLightDecomposition {

//--------- Common ------------------------------------------------------------

#define MAXN    50000
#define LOGN    17              // log2(MAXN)

int gN;

vector<int> gE[MAXN];
int gLevel[MAXN];               // depth (root is 0)
int gP[LOGN][MAXN];             // parent & acestors

int gSubTreeN[MAXN];            // subtree size

void clear() {
    if (gN <= 0)
        return;

    for (int i = 0; i < gN; i++)
        gE[i].clear();
    memset(gLevel, 0, sizeof(gLevel));
    memset(gP, 0, sizeof(gP));
    memset(gSubTreeN, 0, sizeof(gSubTreeN));
}

//--------- DFS ---------------------------------------------------------------

void dfs(int u, int parent) {
    gP[0][u] = parent;
    gSubTreeN[u] = 1;

    for (int v : gE[u]) {
        if (v == parent)
            continue;

        gLevel[v] = gLevel[u] + 1;
        dfs(v, u);

        gSubTreeN[u] += gSubTreeN[v];
    }
}

void dfsIter(int root) {
    struct Item {
        int u;
        int parent;
        int vi;         // child index
    };
    vector<Item> st;
    st.reserve(gN);

    st.push_back(Item{ root, -1, -1 });
    while (!st.empty()) {
        Item& it = st.back();
        if (++it.vi == 0) {
            // enter ...
            gP[0][it.u] = it.parent;
            gSubTreeN[it.u] = 1;
        }

        if (it.vi > 0)
            gSubTreeN[it.u] += gSubTreeN[gE[it.u][it.vi - 1]];

        if (it.vi >= (int)gE[it.u].size()) {
            // leave ...
            st.pop_back();
        } else if (gE[it.u][it.vi] != it.parent) {
            // recursion
            int v = gE[it.u][it.vi];
            gLevel[v] = gLevel[it.u] + 1;
            st.push_back(Item{ v, it.u, -1 });
        }
    }
}

//--------- LCA ---------------------------------------------------------------

void makeLcaTable() {
    for (int i = 1; i < LOGN; i++) {
        for (int j = 0; j < gN; j++) {
            gP[i][j] = gP[i - 1][gP[i - 1][j]];
        }
    }
}

int climbTree(int node, int dist) {
    if (dist <= 0)
        return node;

    for (int i = 0; dist > 0; i++) {
        if (dist & 1)
            node = gP[i][node];
        dist >>= 1;
    }

    return node;
}

int findLCA(int A, int B) {
    if (gLevel[A] < gLevel[B])
        swap(A, B);

    A = climbTree(A, gLevel[A] - gLevel[B]);

    if (A == B)
        return A;

    int bitCnt = 0;
    for (int x = gLevel[A]; x; x >>= 1)
        bitCnt++;

    for (int i = bitCnt - 1; i >= 0; i--) {
        if (gP[i][A] > 0 && gP[i][A] != gP[i][B]) {
            A = gP[i][A];
            B = gP[i][B];
        }
    }

    return gP[0][A];
}

//-----------------------------------------------------------------------------

template <typename T, typename BinOp = function<T(T, T)>>
struct HeavyLightDecomposition {
    BinOp               mBinOp;

    vector<vector<int>> mHeavyPaths;        // heavy paths
    vector<int>         mHeavyPathIndex;    // convert node number(0 ~ gN - 1) to index of heavy paths (gHeavyPaths)

    HeavyLightDecomposition() : mBinOp() {
        // no action
    }

    HeavyLightDecomposition(BinOp op) : mBinOp(op) {
        // no action
    }

    void doHLD(int root) {
        mHeavyPaths.clear();
        mHeavyPathIndex.resize(gN, -1);

        vector<bool> visited(gN);

        queue<int> Q;
        Q.push(root);
        visited[root] = true;
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (int v : gE[u]) {
                if (visited[v])
                    continue;
                Q.push(v);
                visited[v] = true;
            }

            if (u == root)
                continue;

            int p = gP[0][u];

            if (gSubTreeN[u] * 2 >= gSubTreeN[p] && p != root) {
                // heavy path -> add u to parent's heavy path
                int parentPathIndex = mHeavyPathIndex[p];
                mHeavyPaths[parentPathIndex].push_back(u);
                mHeavyPathIndex[u] = parentPathIndex;
            } else {
                // light path -> make new heavy path
                mHeavyPathIndex[u] = mHeavyPaths.size();
                mHeavyPaths.push_back(vector<int>(2));
                mHeavyPaths.back()[0] = p;
                mHeavyPaths.back()[1] = u;
            }
        }
    }

    int findEdgeInPath(int pathIndex, int v) {
        int topOfPath = mHeavyPaths[pathIndex][0];
        return gLevel[gP[0][v]] - gLevel[topOfPath];
    }

    //----- Segment Tree Part ------------------------------------------

    typedef SegmentTree<T, BinOp> SegTreeT;
    vector<SegTreeT> mSegTrees;

    void initSegTree(T defaultValue) {
        mSegTrees.clear();
        mSegTrees.reserve(mHeavyPaths.size());

        // make segment trees on all heavy path
        for (const auto& path : mHeavyPaths) {
            int m = path.size();
            mSegTrees.push_back(SegTreeT(m - 1));
            for (int i = 0; i < m - 1; ++i)
                mSegTrees.back().update(i, defaultValue);   // TODO: initialize all path values
        }
    }

    void update(int u, int v, T cost) {
        if (gP[0][u] == v)
            swap(u, v);
        //assert(gP[0][v] == u);

        int pathIndex = mHeavyPathIndex[v];
        int indexInPath = findEdgeInPath(pathIndex, v);
        mSegTrees[pathIndex].update(indexInPath, cost);
    }

    T query(int u, int v) {
        int t = findLCA(u, v);
        return mBinOp(queryTopdown(t, u), queryTopdown(t, v));
    }

    // return max value a path from u to v
    T queryTopdown(int u, int v) {
        if (u == v)
            return -1;

        if (mHeavyPathIndex[u] == mHeavyPathIndex[v]) {
            int pathIndex = mHeavyPathIndex[u];

            int firstEdge = findEdgeInPath(pathIndex, u) + 1;
            int lastEdge = findEdgeInPath(pathIndex, v);
            return mSegTrees[pathIndex].query(firstEdge, lastEdge);
        }

        int pathIndex = mHeavyPathIndex[v];
        int topOfPath = mHeavyPaths[pathIndex][0];

        //assert(topOfPath != v);

        int lastEdge = findEdgeInPath(pathIndex, v);
        return mBinOp(queryTopdown(u, topOfPath), mSegTrees[pathIndex].query(0, lastEdge));
    }
};

}

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

using namespace TreeHeavyLightDecomposition;

// 문제 풀이 solution : https://algospot.com/judge/problem/read/NAVI
void testHeavyLightDecomposition() {
    return; //TODO: if you want to test a split function, make this line to a comment.

    int T;
    scanf("%d", &T);

    while (T-- > 0) {
        clear();

        scanf("%d", &gN);
        for (int v = 0; v < gN; v++) {
            int u;
            scanf("%d", &u);
            if (u < 0)
                continue;

            gE[u].push_back(v);         // CHECK: make a tree
            gE[v].push_back(u);         // CHECK: make a tree
        }

        dfs(0, -1);                     // CHECK: make tree information
        makeLcaTable();                 // CHECK: make a LCA table

        //HeavyLightDecomposition<int, MaxOp<int>> hld;
        HeavyLightDecomposition<int> hld([](int a, int b) { return max(a, b); });

        hld.doHLD(0);
        hld.initSegTree(1);

        int ans = 0;

        int Q;
        scanf("%d", &Q);
        while (Q-- > 0) {
            char type[10];
            int u, v, cost;

            scanf("%s", type);
            if (!strcmp(type, "update")) {
                scanf("%d %d %d", &u, &v, &cost);
                hld.update(u, v, cost);
            } else {
                scanf("%d %d", &u, &v);
                ans ^= hld.query(u, v);
            }
        }
        printf("%d\n", ans);
    }
}
