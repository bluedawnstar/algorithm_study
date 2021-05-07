#pragma once

/*
  1. how to use

    1) build a tree
       TreeCentroidDecompositionSolverWithUpdate tree(N);
       ...
       // add edges
       tree.addEdge(u, v);
       ...
       // build
       tree.build(0);


    2) update within an distance

        void update(int u, int maxDistance) {
            auto centroids = tree.findCentroidTreeAncestors(u, maxDistance);

            int dist, curr, last;
            for (int i = 0; i < int(centroids.size()); i++) {
                tie(dist, curr, last) = centroids[i];

                auto value = ... calculate value with (u, curr, dist) ...

                if (last == -1) {
                    // curr == u, dist == maxDistance
                    ... add on curr with (dist, value)
                } else {
                    auto value = calculate_path(u, curr)
                    ... add on curr with (dist, value)
                    ... sub on last with (dist, value)
                }
            }
        }


    3) query

        T query(int u) {
            auto centroids = tree.findCentroidTreeAncestors(u);

            T ans = 0;

            int dist, curr, last;
            for (int i = 0; i < int(centroids.size()); i++) {
                tie(dist, curr, last) = centroids[i];

                if (last == -1) {
                    // curr == u, dist == 0
                    auto addValue = ... calculate add value with (curr, u, dist) ...
                    ans += addValue;
                } else {
                    // from -> curr -> u   (OR)   curr -> u
                    auto addValue = ... calculate add value with (curr, dist) and path(curr, u) ...
                    auto subValue = ... calculate sub value with (last, dist) and path(curr, u) ...
                    ans += addValue;
                    ans -= subValue;
                }
            }

            return ans;
        }
*/
struct TreeCentroidDecompositionSolverWithUpdate {
    int N;
    int logN;

    vector<vector<int>> edges;
    vector<vector<int>> P;
    vector<int> level;
    vector<int> treeSize;

    int currTime;
    vector<pair<int, int>> visTime;
    vector<int> time2Node;

    //--- centroid decomposition
    vector<int> ctTreeSize;
    vector<int> ctBigChild;

    int ctRoot;
    vector<int> ctParent;
    vector<bool> ctMark;
    vector<vector<int>> ctDist; // ctDist[u] = { distance from u to the nearest ancestor in centroid tree, next centroid, ..., the farthest ancestor }


    TreeCentroidDecompositionSolverWithUpdate() : N(0), logN(0) {
    }

    explicit TreeCentroidDecompositionSolverWithUpdate(int n, int logN = 0) {
        init(n, logN);
    }


    void init(int n, int _logN = 0) {
        N = n;
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
        treeSize.assign(N, 0);

        currTime = 0;
        visTime.assign(N, make_pair(0, 0));
        time2Node.assign(N, 0);

        //--- centroid decomposition
        ctTreeSize.assign(N, 0);
        ctBigChild.assign(N, 0);

        ctRoot = 0;
        ctParent.assign(N, 0);
        ctMark.assign(N, false);
        ctDist = vector<vector<int>>(N);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    void addEdgeDirected(int u, int v) {
        edges[u].push_back(v);
    }

    void build(int root) {
        currTime = 0;
        dfs(root, -1);
        makeLcaTable();

        doCentroidDecomposition(root);
    }


    //--- lca

    int climbTree(int node, int dist) {
        if (dist <= 0)
            return node;

        for (int i = 0; dist > 0; i++) {
            if (dist & 1)
                node = P[i][node];
            dist >>= 1;
        }

        return node;
    }

    int findLCA(int A, int B) {
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

    inline int distance(int u, int v) {
        return level[u] + level[v] - level[findLCA(u, v)] * 2;
    }

    inline int distance(int u, int v, int lca) {
        return level[u] + level[v] - level[lca] * 2;
    }

    inline bool isAncestor(int u, int v) {
        return visTime[u].first <= visTime[v].first && visTime[v].second <= visTime[u].second;
    }


    //--- centroid tree

    // return { (distance, addNode, subNode), ... }
    // - add some value of addNode to addNode
    // - sub some value of addNode from subNode
    vector<tuple<int, int, int>> findCentroidTreeAncestors(int u, int maxDistance) {
        vector<tuple<int, int, int>> res;

        int v = u, last = -1, at = 0;
        while (v >= 0) {
            int d = maxDistance - ctDist[u][at];
            if (d >= 0)
                res.emplace_back(d, v, last);

            last = v;
            v = ctParent[v];
            at++;
        }

        return res;
    }

    vector<tuple<int, int, int>> findCentroidTreeAncestors(int u) {
        vector<tuple<int, int, int>> res;

        int v = u, last = -1, at = 0;
        while (v >= 0) {
            res.emplace_back(ctDist[u][at], v, last);

            last = v;
            v = ctParent[v];
            at++;
        }

        return res;
    }


    //--- for update & query with centroid tree

    // T calcOp(int u, int ctNode, int leftDist);
    // void addOp(int ctOpNode, int u, int ctNode, int leftDist, T value);  // add value to ctNode
    // void subOp(int ctOpNode, int u, int ctNode, int leftDist, T value);  // sub value from ctNode
    template <typename T>
    void updateWithCentroidTree(int u, int maxDistance,
                                const function<T(int, int, int)>& calcOp,
                                const function<void(int, int, int, int, T)>& addOp,
                                const function<void(int, int, int, int, T)>& subOp) {
        auto centroids = findCentroidTreeAncestors(u, maxDistance);

        int dist, curr, last;
        for (int i = 0; i < int(centroids.size()); i++) {
            tie(dist, curr, last) = centroids[i];

            auto value = calcOp(u, curr, dist);

            if (last == -1) {
                // curr == u, dist == maxDistance
                addOp(curr, u, curr, dist, value);
            } else {
                addOp(curr, u, curr, dist, value);
                subOp(last, u, curr, dist, value);
            }
        }
    }

    // T queryAddOp(int ctOpNode, int u, int ctNode, int dist); // ctOpNode == ctNode(curr node)
    // T querySubOp(int ctOpNode, int u, int ctNode, int dist); // ctOpNode == last node
    template <typename T>
    T queryWithCentroidTree(int u, const function<T(int, int, int, int)>& queryAddOp,
                                   const function<T(int, int, int, int)>& querySubOp) {
        auto centroids = findCentroidTreeAncestors(u);

        T ans = 0;

        int dist, curr, last;
        for (int i = 0; i < int(centroids.size()); i++) {
            tie(dist, curr, last) = centroids[i];

            if (last == -1) {
                // curr == u, dist == 0
                ans += queryAddOp(curr, u, curr, dist);
            } else {
                // from -> curr -> u   (OR)   curr -> u
                ans += queryAddOp(curr, u, curr, dist);
                ans -= querySubOp(last, u, curr, dist);
            }
        }

        return ans;
    }

private:
    void dfs(int u, int parent) {
        visTime[u].first = currTime;
        time2Node[currTime++] = u;

        treeSize[u] = 1;
        P[0][u] = parent;
        for (int v : edges[u]) {
            if (v == parent)
                continue;

            level[v] = level[u] + 1;
            dfs(v, u);
            treeSize[u] += treeSize[v];
        }

        visTime[u].second = currTime;
    }

    void makeLcaTable() {
        for (int i = 1; i < logN; i++) {
            for (int j = 0; j < N; j++) {
                int pp = P[i - 1][j];
                P[i][j] = pp < 0 ? pp : P[i - 1][pp];
            }
        }
    }


    //--- centroid decomposition

    int dfsSize(int u, int parent = -1) {
        ctTreeSize[u] = 1;
        ctBigChild[u] = -1;
        for (int v : edges[u]) {
            if (v == parent || ctMark[v])
                continue;

            ctTreeSize[u] += dfsSize(v, u);
            if (ctBigChild[u] < 0 || ctTreeSize[ctBigChild[u]] < ctTreeSize[v])
                ctBigChild[u] = v;
        }
        return ctTreeSize[u];
    }

    void dfsDistance(int u, int parent = -1, int d = 0) {
        ctDist[u].push_back(d);
        for (int v : edges[u]) {
            if (v == parent || ctMark[v])
                continue;
            dfsDistance(v, u, d + 1);
        }
    }

    int dfsCentroidDecomposition(int u) {
        int n = dfsSize(u);
        while (ctBigChild[u] >= 0 && 2 * ctTreeSize[ctBigChild[u]] > n)
            u = ctBigChild[u];

        dfsDistance(u, u);
        ctMark[u] = true;
        for (int v : edges[u]) {
            if (ctMark[v])
                continue;
            ctParent[dfsCentroidDecomposition(v)] = u;
        }

        return u;
    }

    void doCentroidDecomposition(int root) {
        ctRoot = dfsCentroidDecomposition(root);
        ctParent[ctRoot] = -1;

        for (int i = 0; i < N; i++)
            reverse(begin(ctDist[i]), end(ctDist[i]));  // sort ancestors of centroid tree by distance
    }
};
