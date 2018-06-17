// http://codeforces.com/contest/342/problem/E

#include <memory.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cctype>
#include <cstring>
#include <climits>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include <numeric>
#include <limits>
#include <functional>
#include <tuple>
#include <set>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

#define PROFILE_START(i)    clock_t start##i = clock()
#define PROFILE_STOP(i)     fprintf(stderr, "elapsed time (" #i ") = %f\n", double(clock() - start##i) / CLOCKS_PER_SEC)

typedef long long           ll;
typedef unsigned long long  ull;

struct TreeWithCentroidDecomposition {
    int                 N;          // the number of vertex
    int                 logN;       // log2(N - 1) + 2

    vector<vector<int>> edges;      // edges (vertex number)
    vector<vector<int>> P;          // P[0][n] points to the parent
                                    // parent & acestors
    vector<int>         level;      // depth (root is 0)
    vector<int>         treeSize;   // call dfsSize() to calculate tree size

    vector<vector<int>> ctEdges;    // 
    vector<int>         ctParent;   // 
    vector<int>         ctTreeSize; // 
    vector<bool>        ctVisited;  // 
    int                 ctRoot;     // 

    TreeWithCentroidDecomposition() : N(0), logN(0) {
    }

    explicit TreeWithCentroidDecomposition(int n, int logN = 0) {
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
        treeSize.assign(N, 0);

        ctEdges = vector<vector<int>>(N);
        ctParent = vector<int>(N);
        ctTreeSize = vector<int>(N);
        ctVisited = vector<bool>(N);
        ctRoot = 0;
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
        //bfs(root);
        makeLcaTable();

        decomposeTree(root);
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
        treeSize[u] = 1;
        P[0][u] = parent;

        for (int v : edges[u]) {
            if (v == parent)
                continue;

            level[v] = level[u] + 1;
            dfs(v, u);

            treeSize[u] += treeSize[v];
        }
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
                treeSize[it.u] = 1;
                P[0][it.u] = it.parent;
            }

            if (it.vi > 0)
                treeSize[it.u] += treeSize[edges[it.u][it.vi - 1]];

            if (it.vi >= (int)edges[it.u].size()) {
                // leave ...
                st.pop_back();
            } else if (edges[it.u][it.vi] != it.parent) {
                // recursion
                int v = edges[it.u][it.vi];
                level[v] = level[it.u] + 1;
                st.push_back(Item{ v, it.u, -1 });
            }
        }
    }

    //--- BFS

    void bfs(int root) {
        vector<bool> visited(N);

        queue<int> Q;
        Q.push(root);
        P[0][root] = -1;
        visited[root] = true;
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (int v : edges[u]) {
                if (visited[v])
                    continue;

                visited[v] = true;

                P[0][v] = u;
                level[v] = level[u] + 1;
                Q.push(v);
            }
        }
    }

    //--- LCA

    void makeLcaTable() {
        for (int i = 1; i < logN; i++) {
            for (int j = 0; j < N; j++) {
                int pp = P[i - 1][j];
                P[i][j] = pp < 0 ? pp : P[i - 1][pp];
            }
        }
    }

    //--- Centroid

    int decomposeTree(int start) {
        ctTreeSize = treeSize;
        for (int i = 0; i < N; i++)
            ctParent[i] = P[0][i];

        ctRoot = decomposeTreeSub(start);
        ctParent[ctRoot] = -1;

        return ctRoot;
    }

    int decomposeTreeSub(int u) {
        int size = ctTreeSize[u];
        int centroid = findCentroid(u, -1, size);

        ctVisited[centroid] = true;
        if (ctParent[centroid] >= 0 && !ctVisited[ctParent[centroid]])
            updateSize(ctParent[centroid], centroid);
        ctTreeSize[centroid] = size;

        for (int v : edges[centroid]) {
            if (ctVisited[v])
                continue;

            int subRoot = decomposeTreeSub(v);
            ctParent[subRoot] = centroid;

            ctEdges[centroid].push_back(subRoot);
            ctEdges[subRoot].push_back(centroid);
        }

        return centroid;
    }

    int findCentroid(int u, int from, int size) {
        bool isMajor = true;
        for (int v : edges[u]) {
            if (v == from || ctVisited[v])
                continue;

            int res = findCentroid(v, u, size);
            if (res != -1)
                return res;

            if (ctTreeSize[v] + ctTreeSize[v] > size)
                isMajor = false;
        }

        if (isMajor && (size - ctTreeSize[u]) * 2 <= size)
            return u;

        return -1;
    }

    int updateSize(int u, int from) {
        if (u < 0 || ctVisited[u])
            return 0;

        ctTreeSize[u] = ctTreeSize[u] - ctTreeSize[from] + updateSize(ctParent[u], u);
        ctParent[u] = from;

        return ctTreeSize[u];
    }
};

struct MinDistanceToSpecialNode {
    vector<int> dist;       // minimum distance between a node and any node in the centroid-decomposition subtree

    MinDistanceToSpecialNode() {
    }

    explicit MinDistanceToSpecialNode(TreeWithCentroidDecomposition& tree) {
        init(tree);
    }

    void init(TreeWithCentroidDecomposition& tree) {
        dist.assign(tree.N, tree.N * 2);
    }


    void update(TreeWithCentroidDecomposition& tree, int u) {
        int v = u;
        while (v >= 0) {
            dist[v] = min(dist[v], tree.distance(u, v));
            v = tree.ctParent[v];
        }
    }

    int query(TreeWithCentroidDecomposition& tree, int u) {
        int res = dist[u];
        int v = u;
        while (v >= 0) {
            res = min(res, tree.distance(u, v) + dist[v]);
            v = tree.ctParent[v];
        }
        return res;
    }
};

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    TreeWithCentroidDecomposition tree(N);
    for (int i = 1; i < N; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        tree.addEdge(u, v);
    }

    tree.build(0);

    MinDistanceToSpecialNode solver(tree);
    solver.update(tree, 0);

    for (int i = 0; i < M; i++) {
        int t, v;
        cin >> t >> v;
        v--;
        if (t == 1)
            solver.update(tree, v);
        else
            cout << solver.query(tree, v) << endl;
    }

    return 0;
}
