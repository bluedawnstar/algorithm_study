#pragma once

#include "consecutiveSubarray_DivideCombineTree.h"

// https://discuss.codechef.com/t/goodsegs-editorial/56718
// https://oi-wiki.org/ds/divide-combine/

struct DivideCombineTreeWithLCA : public DivideCombineTree {
    int                 logN;       // log2(N - 1) + 2
    vector<vector<int>> P;          // P[0][n] points to the parent
    vector<int>         level;      // depth (root is 0)

    void build(const int P[], int n) {
        DivideCombineTree::build(P, n);

        initLcaTable(nodeCount);
        dfs(root, -1);
        makeLcaTable();
    }

    void build(const vector<int>& P) {
        build(P.data(), int(P.size()));
    }

    //--- query based on array indexes

    // find smallest consecutive subarray to include [left, right]
    // - 0 <= left <= right < N
    // - result.first <= left <= right <= result.second, O(logN)
    pair<int, int> findSmallestConsecutiveSubarray(int left, int right) {
        int u = indexToNode[left];
        int v = indexToNode[right];
        int lca = findLCA(u, v);
        if (nodes[lca].ok) {
            u = climbTree(u, level[u] - level[lca] - 1);
            v = climbTree(v, level[v] - level[lca] - 1);
            left = nodes[u].L;
            right = nodes[v].R;
        } else {
            left = nodes[lca].L;
            right = nodes[lca].R;
        }
        return make_pair(left, right);
    }

    //--- query based on tree nodes

    int climbTree(int node, int dist) const {
        if (dist <= 0)
            return node;

        for (int i = 0; dist > 0; i++) {
            if (dist & 1)
                node = P[i][node];
            dist >>= 1;
        }

        return node;
    }

    int findLCA(int A, int B) const {
        if (level[A] < level[B])
            swap(A, B);

        A = climbTree(A, level[A] - level[B]);

        if (A == B)
            return A;

        int bitCnt = 0;
        for (int x = level[A]; x; x >>= 1)
            bitCnt++;

        for (int i = bitCnt - 1; i >= 0; i--) {
            if (P[i][A] > 0 && P[i][A] != P[i][B]) {
                A = P[i][A];
                B = P[i][B];
            }
        }

        return P[0][A];
    }

    int distance(int u, int v) const {
        return level[u] + level[v] - level[findLCA(u, v)] * 2;
    }

private:
    void initLcaTable(int _n, int _logN = 0) {
        N = _n;
        logN = _logN;
        if (logN <= 0) {
#ifndef __GNUC__
            logN = _lzcnt_u32(1u) - _lzcnt_u32((unsigned int)(N - 1)) + 2;
#else
            logN = __builtin_clz(1u) - __builtin_clz((unsigned int)(N - 1)) + 2;
#endif
        }

        P = vector<vector<int>>(logN, vector<int>(N));
        level.assign(N, 0);
    }

    void dfs(int u, int parent) {
        P[0][u] = parent;
        for (int v : nodes[u].children) {
            level[v] = level[u] + 1;
            dfs(v, u);
        }
    }

    void makeLcaTable() {
        for (int i = 1; i < logN; i++) {
            for (int j = 0; j < N; j++) {
                int pp = P[i - 1][j];
                P[i][j] = pp < 0 ? pp : P[i - 1][pp];
            }
        }
    }
};
