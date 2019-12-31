#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

#include "../rangeQuery/sparseTableMinIndex.h"

struct EulerTourTree {
    int                 N;          // the number of vertex
    int                 logN;       // log2(N - 1) + 2

    vector<vector<int>> edges;      // edges (vertex number)
    vector<vector<int>> P;          // P[0][n] points to the parent
                                    // parent & acestors

    vector<int>         level;      // depth (root is 0)
    vector<int>         treeSize;   // call dfsSize() to calculate tree size

    //---

    vector<int>         firstVisTime;   // node index to first visit time
    vector<int>         euler;          // time to node index
    SparseTableMinIndex rmq;            // RMQ for LCA

    //--- tree construction

    EulerTourTree() : N(0), logN(0) {
    }

    explicit EulerTourTree(int N, int logN = 0) {
        init(N, logN);
    }

    void init(int N, int logN = 0) {
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

        firstVisTime.assign(N, -1);
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
        makeLcaTable();

        vector<int> lvl(euler.size());
        for (int i = 0; i < int(euler.size()); i++)
            lvl[i] = level[euler[i]];
        rmq.build(lvl);
    }

    //--- LCA

    // O(1) - faster than findLCA() when N < 75000 experimentally
    //      - slower than findLCA() when N >= 75000 experimentally because Euler path is longer than node size
    int findLCAFast(int nodeL, int nodeR) {
        int timeL = firstVisTime[nodeL];
        int timeR = firstVisTime[nodeR];
        if (timeL > timeR)
            swap(timeL, timeR);
        return euler[rmq.query(timeL, timeR)];
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
        if (firstVisTime[u] < 0)
            firstVisTime[u] = int(euler.size());

        euler.push_back(u);

        P[0][u] = parent;
        for (int v : edges[u]) {
            if (v == parent)
                continue;

            level[v] = level[u] + 1;
            dfs(v, u);

            euler.push_back(u);
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
