#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

#include "../rangeQuery/segmentTreeCompactLazy.h"

// It is based on DFS tour tree
template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct DfsTourTreeSubtreeUpdate {
    int                 N;          // the number of vertex
    int                 logN;       // log2(N - 1) + 2

    vector<vector<int>> edges;      // edges (vertex number)
    vector<vector<int>> P;          // P[0][n] points to the parent
                                    // parent & acestors

    vector<int>         level;      // depth (root is 0)
    vector<int>         treeSize;   // call dfsSize() to calculate tree size

    //---

    vector<pair<int, int>> visTime;     // node index to (visit time, leave time)
    vector<int>            time2Node;   // time to node index (0 <= time < 2 * N)
    int                    currTime;    //

    CompactSegmentTreeLazyUpdate<T, MergeOp, BlockOp> seg;

    DfsTourTreeSubtreeUpdate(const MergeOp& mop, const BlockOp& bop, T dflt = T())
        : N(0), logN(0), seg(mop, bop, dflt) {
        currTime = 0;
    }

    DfsTourTreeSubtreeUpdate(int N, const MergeOp& mop, const BlockOp& bop, T dflt = T())
        : N(0), logN(0), seg(mop, bop, dflt) {
        init(N, false, 0);
    }

    void init(int N, bool alignPowerOf2 = false, int logN = 0) {
        if (logN <= 0) {
#ifndef __GNUC__
            logN = _lzcnt_u32(1u) - _lzcnt_u32((unsigned int)(N - 1)) + 2;
#else
            logN = __builtin_clz(1u) - __builtin_clz((unsigned int)(N - 1)) + 2;
#endif
        }

        this->N = N;
        this->logN = logN;

        edges = vector<vector<int>>(N);
        P = vector<vector<int>>(logN, vector<int>(N));
        level.assign(N, 0);
        treeSize.assign(N, 0);

        visTime.assign(N, make_pair(0, 0));
        time2Node.assign(N, 0);
        currTime = 0;

        seg.init(N, alignPowerOf2);
    }


    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    void addEdgeDirected(int u, int v) {
        edges[u].push_back(v);
    }

    void build(int root) {
        dfs(root, -1);
        //dfsIter(root);
        makeLcaTable();
    }

    //-- update

    void update(int u, T value) {
        seg.update(visTime[u].first, visTime[u].second, value);    // update the subtree of u
    }

    //--- query

    T query(int u) {
        return seg.query(visTime[u].first, visTime[u].second);
    }

    // return true if u is an ancestor of v
    bool isAncestor(int u, int v) const {
        return visTime[u].first <= visTime[v].first && visTime[v].second <= visTime[u].second;
    }

    //--- LCA

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

    // find LCA when the root is changed
    int findLCA(int root, int A, int B) const {
        int lca = findLCA(A, B);

        int temp = findLCA(A, root);
        if (level[temp] > level[lca])
            lca = temp;

        temp = findLCA(B, root);
        if (level[temp] > level[lca])
            lca = temp;

        return lca;
    }

    int distance(int u, int v) const {
        return level[u] + level[v] - level[findLCA(u, v)] * 2;
    }

private:
    //--- DFS

    void dfs(int u, int parent) {
        visTime[u].first = currTime;
        time2Node[currTime++] = u;

        P[0][u] = parent;
        for (int v : edges[u]) {
            if (v == parent)
                continue;

            level[v] = level[u] + 1;
            dfs(v, u);
        }

        visTime[u].second = currTime - 1;
    }

    void dfsIter(int root) {
        struct Item {
            int u;
            int parent;
            int vi;         // child index
        };
        vector<Item> st;
        st.reserve(N);

        st.push_back(Item{ root, -1, -1 });
        while (!st.empty()) {
            Item& it = st.back();
            if (++it.vi == 0) {
                // enter ...
                P[0][it.u] = it.parent;
                visTime[it.u].first = currTime;
                time2Node[currTime++] = it.u;
            }
            if (it.vi >= int(edges[it.u].size())) {
                // leave ...
                visTime[it.u].second = currTime - 1;
                st.pop_back();
            } else if (edges[it.u][it.vi] != it.parent) {
                // recursion
                int v = edges[it.u][it.vi];
                level[v] = level[it.u] + 1;
                st.push_back(Item{ v, it.u, -1 });
            }
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

template <typename T, typename MergeOp, typename BlockOp>
inline DfsTourTreeSubtreeUpdate<T, MergeOp, BlockOp> makeDfsTourTreeSubtreeUpdate(const MergeOp& mop, const BlockOp& bop, T dfltValue = T()) {
    return DfsTourTreeSubtreeUpdate<T, MergeOp, BlockOp>(mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline DfsTourTreeSubtreeUpdate<T, MergeOp, BlockOp> makeDfsTourTreeSubtreeUpdate(int size, const MergeOp& mop, const BlockOp& bop, T dfltValue = T()) {
    return DfsTourTreeSubtreeUpdate<T, MergeOp, BlockOp>(size, mop, bop, dfltValue);
}