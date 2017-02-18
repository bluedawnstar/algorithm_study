#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#if 0
namespace lct {
    const int N = 100005;
    int go[N << 1][26], fail[N << 1], len[N << 1], tot, last;
    //---------
    int l[N << 2], r[N << 2], fa[N << 2];
    int last[N << 2];
    inline bool top(int x) { return (!fa[x]) || (l[fa[x]] != x&&r[fa[x]] != x); }
    inline void left(int x) {
        int y = fa[x]; int z = fa[y];
        r[y] = l[x]; if (l[x])fa[l[x]] = y;
        fa[x] = z; if (l[z] == y)l[z] = x; else if (r[z] == y)r[z] = x;
        l[x] = y; fa[y] = x;
    }
    inline void right(int x) {
        int y = fa[x]; int z = fa[y];
        l[y] = r[x]; if (r[x])fa[r[x]] = y;
        fa[x] = z; if (l[z] == y)l[z] = x; else if (r[z] == y)r[z] = x;
        r[x] = y; fa[y] = x;
    }
    inline void down(int x) {
        if (l[x])last[l[x]] = last[x];
        if (r[x])last[r[x]] = last[x];
    }
    int q[N << 2];
    inline void splay(int x) {
        q[q[0] = 1] = x;
        for (int k = x; !top(k); k = fa[k])q[++q[0]] = fa[k];
        for (int i = q[0]; i >= 1; i--)
            down(q[i]);
        while (!top(x)) {
            int y = fa[x]; int z = fa[y];
            if (top(y)) {
                if (l[y] == x)right(x); else left(x);
            } else {
                if (r[z] == y) {
                    if (r[y] == x)left(y), left(x);
                    else right(x), left(x);
                } else {
                    if (l[y] == x)right(y), right(x);
                    else left(x), right(x);
                }
            }
        }
    }
    void Access(int x, int cov) {
        int y = 0;
        for (; x; y = x, x = fa[x]) {
            splay(x);
            down(x);
            r[x] = 0;

            int L, R;
            int z = x;
            while (l[z])z = l[z];
            L = len[fail[z]] + 1;
            splay(z); splay(x);
            z = x;
            while (r[z])z = r[z];
            R = len[z];
            splay(z); splay(x);
            seg::Do(last[x], cov, L, R);
            r[x] = y;
            last[x] = cov;
        }
    }
    void SetFa(int x, int y, int po) {
        fa[x] = y;
        Access(x, po);
    }
    void split(int x, int y, int d) {
        splay(y);
        down(y);
        r[y] = 0;
        fa[d] = y;
        splay(x);
        fa[x] = d;
        last[d] = last[x];
    }
};

namespace LinkCutTreeSpace {

    //--------- Common ------------------------------------------------------------

#define MAXN    10000           // TODO: modify the maximum number of nodes
#define LOGN    15              // TODO: modify LCA table size (log2(MAXN))

    int gN;                         // TODO: set a value

    vector<int> gE[MAXN];           // TODO: make a tree
    int gP[LOGN][MAXN];             // TODO: set all gP[0][n] to their parent
                                    // parent & acestors

    int gLevel[MAXN];               // depth (root is 0)

    void clear() {
        if (gN <= 0)
            return;

        for (int i = 0; i < gN; i++)
            gE[i].clear();
        memset(gLevel, 0, sizeof(gLevel));
        memset(gP, 0, sizeof(gP));
    }

    //--------- DFS ---------------------------------------------------------------

    void dfs(int u, int parent) {
        gP[0][u] = parent;

        for (int v : gE[u]) {
            if (v == parent)
                continue;

            gLevel[v] = gLevel[u] + 1;
            dfs(v, u);
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
            }
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

    //-----------------------------------------------------------------------------

    void bfs(int root) {
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

                visited[v] = true;

                gP[0][v] = u;
                gLevel[v] = gLevel[u] + 1;
                Q.push(v);
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

}

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

using namespace LinkCutTreeSpace;

static void makeTree() {
    gN = 10;

    gE[0].push_back(1); gE[1].push_back(0);
    gE[0].push_back(3); gE[3].push_back(0);
    gE[1].push_back(4); gE[4].push_back(1);
    gE[1].push_back(2); gE[2].push_back(1);
    gE[3].push_back(6); gE[6].push_back(3);
    gE[3].push_back(7); gE[7].push_back(3);
    gE[4].push_back(9); gE[9].push_back(4);
    gE[2].push_back(8); gE[8].push_back(2);
    gE[2].push_back(5); gE[5].push_back(2);
}

static void makeLcaTree() {
    gN = MAXN;

    gE[0].push_back(1); gE[1].push_back(0);
    gE[0].push_back(2); gE[2].push_back(0);

    int i, p = 1;
    for (i = 3; i < gN / 4; i++) {
        gE[p].push_back(i); gE[i].push_back(p);
        p = i;
    }

    p = 1;
    for (; i < gN * 2 / 4; i++) {
        gE[p].push_back(i); gE[i].push_back(p);
        p = i;
    }

    p = 2;
    for (; i < gN * 3 / 4; i++) {
        gE[p].push_back(i); gE[i].push_back(p);
        p = i;
    }

    p = 2;
    for (; i < gN; i++) {
        gE[p].push_back(i); gE[i].push_back(p);
        p = i;
    }
}

static void printData() {
    cout << "level : ";
    for (int i = 0; i < gN; i++)
        cout << gLevel[i] << ", ";
    cout << endl;

    cout << "parent : ";
    for (int i = 0; i < gN; i++)
        cout << gP[i][0] << ", ";
    cout << endl;
}

void testLinkCutTree() {
    return; //TODO: if you want to test a split function, make this line to a comment.

            //Usage:
            //    step1 : make a tree (gE, gP[0][x])

    cout << "-- dfs() vs dfsIter() ----------------------------------" << endl;
    clear();
    makeTree(); // make a test tree
    dfs(0, -1);
    printData();

    makeTree();
    dfsIter(0);
    printData();

    /*
    cout << "-- dfs() vs dfsIter() - performance test ---------------" << endl;
    clear();
    makeTree(); // make a test tree
    clock_t start = clock();
    for (int i = 0; i < 1000000; i++) {
    gCurrTime = 0;
    dfs(1, 0);
    }
    cout << "elapsed time : " << double(clock() - start) / CLOCKS_PER_SEC << endl;

    clear();
    makeTree(); // make a test tree
    start = clock();
    for (int i = 0; i < 1000000; i++) {
    gCurrTime = 0;
    dfsIter(1);
    }
    cout << "elapsed time : " << double(clock() - start) / CLOCKS_PER_SEC << endl;
    */

    cout << "-- LCA test --------------------------------------------" << endl;
    clear();        // step1: clear all variables
    makeLcaTree();  // ... make a test tree

                    //dfs(0, -1);
    dfsIter(0);     // step2: make depth and parent table
    makeLcaTable(); // step3: make LCA table

    int errCnt = 0;
    for (int i = 0; i < 100000; i++) {
        int u = rand() % gN;
        int v = rand() % gN;
        int lca = findLCA(u, v);
        int lcaAns;
        if (u == 0 || v == 0) {
            lcaAns = 0;
        } else if ((u != 2 && u < gN / 2) != (v != 2 && v < gN / 2)) {
            lcaAns = 0;
        } else if (u != 2 && u < gN / 2) {
            if ((u >= gN / 4) != (v >= gN / 4))
                lcaAns = 1;
            else
                lcaAns = min(u, v);
        } else {
            if ((u >= gN * 3 / 4) != (v >= gN * 3 / 4))
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

#endif

void testLinkCutTree() {

}