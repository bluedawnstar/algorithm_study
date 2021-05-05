#include <functional>
#include <vector>
#include <algorithm>
#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

using namespace std;

#include "treeCentroidDecompositionSolverWithUpdate.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <tuple>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "../rangeQuery/bitTrieForest.h"


// https://www.codechef.com/problems/RWALKS
// https://discuss.codechef.com/t/rwalks-editorial/86602

template <int mod = 1'000'000'007>
struct RandomWalkQueriesSolver {
    TreeCentroidDecompositionSolverWithUpdate tree;

    vector<int> inv;
    vector<int> prefixProb;
    vector<int> prefixProbInv;

    BitTrieForest<long long> trie;
    vector<int> add;
    vector<int> sub;
    vector<int> addSrc;


    void init(int n) {
        tree.init(n);
        trie.init(tree.logN);

        inv = vector<int>(n + 1);
        prefixProb = vector<int>(n + 1);
        prefixProbInv = vector<int>(n + 1);
    }


    //--- build

    void addEdge(int u, int v) {
        tree.addEdge(u, v);
    }

    void build() {
        inv[0] = 1;
        inv[1] = 1;
        for (int i = 2; i <= tree.N; i++) {
            inv[i] = int((mod - 1ll * (mod / i) * inv[mod % i] % mod) % mod);
        }

        add = vector<int>(tree.N);
        sub = vector<int>(tree.N);
        addSrc = vector<int>(tree.N);
        for (int i = 0; i < tree.N; i++) {
            add[i] = trie.createTrie();
            sub[i] = trie.createTrie();
            addSrc[i] = trie.createTrie();
        }

        tree.build(0);
        dfs2(0, -1, 1ll, 1ll);
    }


    //--- update & query

    void update(int u, int d) {
        tree.updateWithCentroidTree<int>(u, d,
            // calc op
            [&](int u, int ctNode, int leftDist) -> int {
                if (u == ctNode)
                    return 1;
                return getProb(u, ctNode);
            },
            // add op
            [&](int ctOpNode, int u, int ctNode, int leftDist, int value) {
                if (u == ctNode)
                    trie.insert(addSrc[ctOpNode], leftDist, value);
                else {
                    trie.insert(add[ctOpNode], leftDist, value);
                }
            },
            // sub op
            [&](int ctOpNode, int u, int ctNode, int leftDist, int value) {
                if (u != ctNode)
                    trie.insert(sub[ctOpNode], leftDist, value);
            }
        );
    }

    int query(int u) {
        long long res = tree.queryWithCentroidTree<long long>(u,
            // queryAddOp
            [this](int ctOpNode, int u, int ctNode, int dist) -> long long {
                long long res = 0;
                if (u == ctNode) {
                    res += trie.sumGreaterThanOrEqual(add[ctOpNode], dist) % mod;
                    res += trie.sumGreaterThanOrEqual(addSrc[ctOpNode], dist) % mod;
                } else {
                    // src -> ctNode -> u
                    res += trie.sumGreaterThanOrEqual(add[ctOpNode], dist) % mod * getMidProb(ctNode, u) % mod;
                    // ctNode -> u
                    res += trie.sumGreaterThanOrEqual(addSrc[ctOpNode], dist) % mod * getProb(ctNode, u) % mod;
                }
                return res;
            },
            // querySubOp
            [this](int ctOpNode, int u, int ctNode, int dist) -> long long {
                long long res = 0;
                if (u != ctNode) {
                    // src -> ctNode -> u
                    res = trie.sumGreaterThanOrEqual(sub[ctOpNode], dist) % mod * getMidProb(ctNode, u) % mod;
                }
                return res;
            }
        );

        res %= mod;
        if (res < 0)
            res += mod;

        return int(res);
    }


private:
    void dfs2(int u, int parent, long long x, long long invX) {
        x = x * max(1, int(tree.edges[u].size()) - 1) % mod;
        invX = invX * inv[int(tree.edges[u].size()) - 1] % mod;

        prefixProbInv[u] = int(x);
        prefixProb[u] = int(invX);
        for (int v : tree.edges[u]) {
            if (v == parent)
                continue;
            dfs2(v, u, x, invX);
        }
    }

    // [u, v), u is source
    int getProb(int u, int v) {
        if (u == v)
            return 1;

        if (tree.isAncestor(v, u)) {            // if v is an ancestor of u
            return int(1ll * prefixProb[tree.P[0][u]] * prefixProbInv[v] % mod
                           * inv[tree.edges[u].size()] % mod);
        } else if (tree.isAncestor(u, v)) {     // if u is an ancestor of v
            return int(1ll * prefixProb[tree.P[0][v]] * prefixProbInv[u] % mod
                           * inv[tree.edges[u].size()] % mod);
        } else {
            int lca = tree.findLCA(u, v);

            return int(1ll * prefixProb[tree.P[0][u]] * prefixProbInv[lca] % mod
                           * prefixProb[tree.P[0][v]] % mod * prefixProbInv[lca] % mod
                           * inv[tree.edges[lca].size() - 1] % mod
                           * inv[tree.edges[u].size()] % mod);
        }
    }

    // [u, v), u is not source
    int getMidProb(int u, int v) {
        if (u == v)
            return 1;

        if (tree.isAncestor(v, u)) {            // if v is an ancestor of u
            return int(1ll * prefixProb[u] * prefixProbInv[v] % mod);
        } else if (tree.isAncestor(u, v)) {     // if u is an ancestor of v
            return int(1ll * prefixProb[tree.P[0][v]] * prefixProbInv[u] % mod
                           * inv[tree.edges[u].size() - 1] % mod);
        } else {
            int lca = tree.findLCA(u, v);

            return int(1ll * prefixProb[u] * prefixProbInv[lca] % mod
                           * prefixProb[tree.P[0][v]] % mod * prefixProbInv[lca] % mod
                           * inv[tree.edges[lca].size() - 1] % mod);
        }
    }
};


void testCentroidDecompositionSolverWithUpdate() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Centroid Decomposition Solver with Update --------------------------" << endl;
    {
        RandomWalkQueriesSolver<> solver;

        solver.init(6);

        //--- build
        solver.addEdge(5, 1);
        solver.addEdge(2, 1);
        solver.addEdge(3, 0);
        solver.addEdge(0, 1);
        solver.addEdge(4, 3);
        solver.build();

        //--- update & query
        solver.update(0, 3);
        assert(solver.query(4) == 500'000'004);

        solver.update(2, 4);
        assert(solver.query(4) == 1);
    }

    cout << "OK!" << endl;
}
