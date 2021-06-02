#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

/*
  <How-to-use>
    1. initialize
       - init(N)

    2. add edges
       - addEdge(u, v)
         ...

    3. build
       - build(0, colors)

    4. query
       - query(node, depth)
*/
struct UniqueValueCounterInDistanceOfSubtree {
    struct PersistentSegmentTree {
        int N;
        vector<int> value;
        vector<int> L;
        vector<int> R;
        int root;           // first root;

        PersistentSegmentTree() {
        }

        explicit PersistentSegmentTree(int n) {
            build(n);
        }

        // return root node index
        int build(int n) {
            N = n;
            value.reserve(N * 4);
            L.reserve(N * 4);
            R.reserve(N * 4);
            return root = dfsBuild(0, N - 1);
        }

        // return root node index, O(logN)
        int add(int root, int index, int val) {
            return dfsAdd(root, 0, N - 1, index, val);
        }

        // O(logN)
        int query(int root, int left, int right) const {
            return dfsQuery(root, 0, N - 1, left, right);
        }

    private:
        int dfsBuild(int left, int right) {
            int l = -1, r = -1;
            if (left < right) {
                int mid = (left + right) >> 1;
                l = dfsBuild(left, mid);
                r = dfsBuild(mid + 1, right);
            }
            value.push_back(0);
            L.push_back(l);
            R.push_back(r);
            return int(value.size()) - 1;
        }

        int dfsAdd(int node, int left, int right, int index, int val) {
            int newValue;
            int l = -1, r = -1;
            if (left == right) {
                newValue = value[node] + val;       //TODO: merge operation
            } else {
                int mid = (left + right) >> 1;
                if (index <= mid) {
                    l = dfsAdd(L[node], left, mid, index, val);
                    r = R[node];
                } else {
                    l = L[node];
                    r = dfsAdd(R[node], mid + 1, right, index, val);
                }
                newValue = value[l] + value[r];     //TODO: merge operation
            }
            value.push_back(newValue);
            L.push_back(l);
            R.push_back(r);
            return int(value.size()) - 1;
        }

        int dfsQuery(int node, int left, int right, int indexL, int indexR) const {
            if (indexR < left || right < indexL)
                return int(0);

            if (indexL <= left && right <= indexR)
                return value[node];

            int mid = (left + right) >> 1;
            return dfsQuery(L[node], left, mid, indexL, indexR)
                 + dfsQuery(R[node], mid + 1, right, indexL, indexR);   //TODO: merge operation
        }
    };

    static const int INF = 0x3f3f3f3f;

    int                 N;              // the number of vertex
    int                 logN;           // log2(N - 1) + 2

    vector<vector<int>> edges;          // edges (vertex number)
    vector<vector<int>> P;              // P[0][n] points to the parent
                                        // parent & acestors

    vector<int>         level;          // depth (root is 0)

    int                 currTime;
    vector<pair<int, int>> visitTime;
    vector<int>         timeToNode;

    PersistentSegmentTree segTree;
    vector<int>         segTreeRoots;   // root = segTreeRoots[level]

    UniqueValueCounterInDistanceOfSubtree() : N(0), logN(0) {
    }

    explicit UniqueValueCounterInDistanceOfSubtree(int n, int logN = 0) {
        init(n, logN);
    }

    void init(int _n, int _logN = 0) {
        N = _n;
        logN = _logN;
        if (logN <= 0) {
#ifndef __GNUC__
            logN = _lzcnt_u32(1u) - _lzcnt_u32((unsigned int)(N - 1)) + 2;
#else
            logN = __builtin_clz(1u) - __builtin_clz((unsigned int)(N - 1)) + 2;
#endif
        }

        edges = vector<vector<int>>(N);
        P = vector<vector<int>>(logN, vector<int>(N));
        level.assign(N, 0);

        currTime = 0;
        visitTime = vector<pair<int, int>>(N);
        timeToNode = vector<int>(N);
    }

    //---

    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    void addEdgeDirected(int u, int v) {
        edges[u].push_back(v);
    }

    //--- LCA

    int climbTree(int node, int dist) const {
        if (dist <= 0)
            return node;

#if 0
        for (int i = 0; dist > 0; i++) {
            if (dist & 1)
                node = P[i][node];
            dist >>= 1;
        }
#else
        for (; dist; dist &= dist - 1) {
#ifndef __GNUC__
            int i = static_cast<int>(_tzcnt_u32(static_cast<unsigned>(dist)));
#else
            int i = __builtin_ctz(static_cast<unsigned>(dist));
#endif
            node = P[i][node];
        }
#endif

        return node;
    }

    int findLCA(int A, int B) const {
        if (level[A] < level[B])
            swap(A, B);

        A = climbTree(A, level[A] - level[B]);

        if (A == B)
            return A;

#if 0
        int bitCnt = 0;
        for (int x = level[A]; x; x >>= 1)
            bitCnt++;
#else
#ifndef __GNUC__
        int bitCnt = 32 - static_cast<int>(__lzcnt(static_cast<unsigned>(level[A])));
#else
        int bitCnt = 32 - __builtin_clz(static_cast<unsigned>(level[A]));
#endif
#endif

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

    // return if u is an ancestor of v
    bool isAncestor(int u, int v) const {
        return visitTime[u].first <= visitTime[v].first && visitTime[v].second <= visitTime[u].second;
    }

    //---

    // O(NlogN)
    void build(int root, vector<int> values) {
        dfs(root, -1);
        makeLcaTable();

        segTree.build(N);
        segTreeRoots.assign(N, segTree.root);

        //--- grouping
        vector<vector<int>> groups;
        {
            vector<int> v = values;
            sort(v.begin(), v.end());
            v.resize(unique(v.begin(), v.end()) - v.begin());

            groups.resize(v.size());
            for (int i = 0; i < N; i++) {
                values[i] = int(lower_bound(v.begin(), v.end(), values[i]) - v.begin());
                groups[values[i]].push_back(i);
            }
        }

        //--- 

        vector<int> parent(N);
        vector<pair<int, int>> best(N);                 // (depth, node)
        vector<vector<pair<int, int>>> operations(N);   // operations[depth] = { (time, delta), ... }
        for (int i = 0; i < int(groups.size()); i++) {
            auto& g = groups[i];

            sort(g.begin(), g.end(), [this](int u, int v) {
                return visitTime[u].first < visitTime[v].first;
            });

            int size = int(g.size());

            g.push_back(root);
            for (int j = 1; j < size; j++)
                g.push_back(findLCA(g[j - 1], g[j]));

            sort(g.begin(), g.end(), [this](int u, int v) {
                return visitTime[u].first < visitTime[v].first;
            });
            g.erase(unique(g.begin(), g.end()), g.end());

            for (int node : g) {
                parent[node] = -1;
                best[node] = { INF, INF };
                if (values[node] == i)
                    best[node] = { level[node], node };
            }

            int last = root;
            for (int j = 1; j < int(g.size()); j++) {
                int node = g[j];
                while (last != root && !isAncestor(last, node))
                    last = parent[last];
                parent[node] = last;
                last = node;
            }

            /*
                           (1) -- lca
                          /   \
                 lca -- (2)   (3) -- lca
                        / \   / \
                       A   B C   D

               DFS order: (1) - (2) - A - B - (3) - C - D

               D  :                           -1 ...... 1
               C  :                           -1 .. 1
              (3) :       -1 ................. 1
               B  :             -1 ...... 1
               A  :             -1 .. 1
              (2) :       -1 ... 1
              (1) :        1
            */
            for (int j = int(g.size()) - 1; j >= 0; j--) {
                int u = g[j];
                int p = parent[u];
                operations[best[u].first].emplace_back(visitTime[u].first, 1);
                if (p != -1) {
                    best[p] = min(best[p], best[u]);
                    operations[best[u].first].emplace_back(visitTime[p].first, -1);
                }
            }
        }

        for (int i = 0; i < N; i++) {
            if (i > 0)
                segTreeRoots[i] = segTreeRoots[i - 1];

            for (auto& op : operations[i])
                segTreeRoots[i] = segTree.add(segTreeRoots[i], op.first, op.second);
        }
    }

    // 0 <= node < N, 0 <= depth, O(logN)
    int query(int node, int depth) const {
        return segTree.query(segTreeRoots[min(N - 1, level[node] + depth)],
            visitTime[node].first,
            visitTime[node].second - 1);
    }

private:
    void dfs(int u, int parent) {
        P[0][u] = parent;

        timeToNode[currTime] = u;
        visitTime[u].first = currTime++;

        for (int v : edges[u]) {
            if (v == parent)
                continue;

            level[v] = level[u] + 1;
            dfs(v, u);
        }

        visitTime[u].second = currTime;
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
