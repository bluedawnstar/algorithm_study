#include <cassert>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

#include "linkCutTreePathQuery.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include <algorithm>
#include <numeric>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "treeBasic.h"

#ifdef _DEBUG
#define MAXN    10000
#define LOGN    18
#else
#define MAXN    100000
#define LOGN    18
#endif

vector<int> gTrV;

template <typename Mop, typename Bop>
static void buildTree(Tree& tr, LinkCutTreePathQueryArray<int,Mop,Bop>& lct) {
    gTrV.clear();
    gTrV.resize(MAXN);

    for (int u = 1; u < MAXN; u++) {
        int v = RandInt32::get() % u;
        tr.addEdge(u, v);

        lct.link(max(u, v), min(u, v));
    }

    tr.dfs(0, -1);
    tr.makeLcaTable();
}

static void update(Tree& tr, int u, int value) {
    gTrV[u] = value;
}

static void add(Tree& tr, int u, int value) {
    gTrV[u] += value;
}

static void update(Tree& tr, int u, int v, int value) {
    int lca = tr.findLCA(u, v);
    while (u != lca) {
        gTrV[u] = value;
        u = tr.P[0][u];
    }
    while (v != lca) {
        gTrV[v] = value;
        v = tr.P[0][v];
    }
    gTrV[lca] = value;
}

static void add(Tree& tr, int u, int v, int value) {
    int lca = tr.findLCA(u, v);
    while (u != lca) {
        gTrV[u] += value;
        u = tr.P[0][u];
    }
    while (v != lca) {
        gTrV[v] += value;
        v = tr.P[0][v];
    }
    gTrV[lca] += value;
}

static int query(Tree& tr, int u, int v) {
    int res = 0;

    int lca = tr.findLCA(u, v);
    while (u != lca) {
        res += gTrV[u];
        u = tr.P[0][u];
    }
    while (v != lca) {
        res += gTrV[v];
        v = tr.P[0][v];
    }
    res += gTrV[lca];

    return res;
}


static int findLCA(const vector<int>& parent, int u, int v) {
    vector<int> ancestorU;
    while (u >= 0) {
        ancestorU.push_back(u);
        u = parent[u];
    }

    vector<int> ancestorV;
    while (v >= 0) {
        ancestorV.push_back(v);
        v = parent[v];
    }

    int lca = 0;
    for (int i = (int)ancestorU.size() - 1, j = (int)ancestorV.size() - 1; i >= 0 && j >= 0; i--, j--) {
        if (ancestorU[i] != ancestorV[j])
            break;
        lca = ancestorU[i];
    }

    return lca;
}

static void update(const vector<int>& parent, vector<int>& values, int u, int v, int val) {
    int lca = findLCA(parent, u, v);

    while (u != lca) {
        values[u] = val;
        u = parent[u];
    }
    while (v != lca) {
        values[v] = val;
        v = parent[v];
    }
    values[lca] = val;
}

static int query(const vector<int>& parent, const vector<int>& values, int u, int v) {
    int lca = findLCA(parent, u, v);

    int res = 0;
    while (u != lca) {
        res += values[u];
        u = parent[u];
    }
    while (v != lca) {
        res += values[v];
        v = parent[v];
    }
    res += values[lca];

    return res;
}


void testLinkCutTreePathQuery() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Link-Cut Tree with Path Query ----------------------------------" << endl;
    {
        Tree tr(MAXN, LOGN);
        auto lct = makeLinkCutTreePathQueryArray(MAXN, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        buildTree(tr, lct);

        for (int j = 0; j < 1000; j++) {
            int u = RandInt32::get() % MAXN;
            int v = RandInt32::get() % MAXN;
            int value = RandInt32::get() % 1000;

            if (RandInt32::get() % 2) {
                update(tr, u, value);
                lct.update(u, value);
            } else {
                int ans1 = query(tr, u, v);
                int ans2 = lct.query(u, v);
                if (ans1 != ans2)
                    cerr << "Mismatched! : " << ans1 << ", " << ans2 << endl;
                assert(ans1 == ans2);
            }
        }
    }
    cout << "OK!" << endl;
    {
        Tree tr(MAXN, LOGN);
        auto lct = makeLinkCutTreePathQueryArray(MAXN, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        buildTree(tr, lct);

        for (int j = 0; j < 1000; j++) {
            int u = RandInt32::get() % MAXN;
            int v = RandInt32::get() % MAXN;
            int value = RandInt32::get() % 1000;

            if (RandInt32::get() % 2) {
                add(tr, u, value);
                lct.add(u, value);
            } else {
                int ans1 = query(tr, u, v);
                int ans2 = lct.query(u, v);
                if (ans1 != ans2)
                    cerr << "Mismatched! : " << ans1 << ", " << ans2 << endl;
                assert(ans1 == ans2);
            }
        }
    }
    cout << "OK!" << endl;
    {
        Tree tr(MAXN, LOGN);
        auto lct = makeLinkCutTreePathQueryArray(MAXN, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        buildTree(tr, lct);

        for (int j = 0; j < 1000; j++) {
            int u = RandInt32::get() % MAXN;
            int v = RandInt32::get() % MAXN;
            int value = RandInt32::get() % 1000;

            if (RandInt32::get() % 2) {
                update(tr, u, v, value);
                lct.updateRange(u, v, value);
            } else {
                int ans1 = query(tr, u, v);
                int ans2 = lct.query(u, v);
                if (ans1 != ans2)
                    cerr << "Mismatched! : " << ans1 << ", " << ans2 << endl;
                assert(ans1 == ans2);
            }
        }
    }
    cout << "OK!        " << endl;
    {
        Tree tr(MAXN, LOGN);
        auto lct = makeLinkCutTreePathQueryArray(MAXN, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);
        buildTree(tr, lct);

        for (int j = 0; j < 1000; j++) {
            int u = RandInt32::get() % MAXN;
            int v = RandInt32::get() % MAXN;
            int value = RandInt32::get() % 1000;

            if (RandInt32::get() % 2) {
                add(tr, u, v, value);
                lct.addRange(u, v, value);
            } else {
                int ans1 = query(tr, u, v);
                int ans2 = lct.query(u, v);
                if (ans1 != ans2)
                    cerr << "Mismatched! : " << ans1 << ", " << ans2 << endl;
                assert(ans1 == ans2);
            }
        }
    }
    cout << "OK!" << endl;
    // test dynamic update
    {
        int N = 5000000;
        int T = 1000000;
#ifdef _DEBUG
        N = 10000;
        T = 10000;
#endif

        auto lct = makeLinkCutTreePathQueryArray(N, [](int a, int b) { return a + b; }, [](int x, int n) { return x * n; }, 0);

        vector<int> values(N);
        vector<int> parent(N, -1);

        values[0] = RandInt32::get() % 100;
        lct.update(0, values[0]);
        for (int v = 1; v < N; v++) {
            int u = RandInt32::get() % v;
            lct.link(v, u);
            parent[v] = u;

            values[v] = RandInt32::get() % 100;
            lct.update(v, values[v]);
        }

        for (int i = 0; i < T; i++) {
            int u = RandInt32::get() % MAXN;
            int v = RandInt32::get() % MAXN;
            int val = RandInt32::get() % 100;

            if (RandInt32::get() % 2) {
                update(parent, values, u, v, val);
                lct.updateRange(u, v, val);
            } else {
                int ans1 = query(parent, values, u, v);
                int ans2 = lct.query(u, v);
                //int ans2 = lct.queryAccumulative(u, v);
                if (ans1 != ans2) {
                    cerr << "Mismatched! : " << ans1 << ", " << ans2 << endl;
                }
                assert(ans1 == ans2);
            }

            // change tree structure
            {
                int u, newP;
                do {
                    u = RandInt32::get() % N;
                    newP = RandInt32::get() % N;
                    if (newP > u)
                        swap(u, newP);
                } while (u > 0 && u == newP);

                lct.cut(u);
                lct.link(u, newP);
                parent[u] = newP;
            }
        }
    }
    cout << "OK!" << endl;
}
