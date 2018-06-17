#include <memory.h>
#include <cassert>
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

struct Tree {
    int                 N;          // the number of vertex
    int                 logN;       // log2(N - 1) + 2

    vector<vector<int>> edges;      // edges (vertex number)
    vector<vector<int>> P;          // P[0][n] points to the parent
                                    // parent & acestors

    vector<int>         level;      // depth (root is 0)
    vector<int>         treeSize;   // call dfsSize() to calculate tree size

    //--- tree construction ---------------------------------------------------

    Tree() : N(0), logN(0) {
    }

    explicit Tree(int n, int logN = 0) {
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
        //dfsIter(root, -1);
        //bfs(root);
        makeLcaTable();
    }

    //--- DFS -----------------------------------------------------------------

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

    //--- LCA -----------------------------------------------------------------

    void makeLcaTable() {
        for (int i = 1; i < logN; i++) {
            for (int j = 0; j < N; j++) {
                int pp = P[i - 1][j];
                P[i][j] = pp < 0 ? pp : P[i - 1][pp];
            }
        }
    }

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
};

struct HeavyLightDecomposition {
    Tree& tree;

    int root;
    vector<vector<int>> heavyPaths;         // heavy paths
    vector<int>         heavyPathIndex;     // convert node number(0 ~ N - 1) to index of heavy paths (heavyPaths)

    explicit HeavyLightDecomposition(Tree& tree) : tree(tree), root(0) {
        // no action
    }

    void doHLD(int root) {
        root = root;
        heavyPaths.clear();
        heavyPathIndex.resize(tree.N, -1);

        vector<bool> visited(tree.N);

        queue<int> Q;
        Q.push(root);
        visited[root] = true;
        while (!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for (int v : tree.edges[u]) {
                if (visited[v])
                    continue;
                Q.push(v);
                visited[v] = true;
            }

            if (u == root)
                continue;

            int p = tree.P[0][u];

            if (tree.treeSize[u] * 2 >= tree.treeSize[p] && p != root) {
                // heavy path -> add u to parent's heavy path
                int parentPath = heavyPathIndex[p];
                heavyPaths[parentPath].push_back(u);
                heavyPathIndex[u] = parentPath;
            } else {
                // light path -> make new heavy path
                heavyPathIndex[u] = heavyPaths.size();
                heavyPaths.push_back(vector<int>(2));
                heavyPaths.back()[0] = p;
                heavyPaths.back()[1] = u;
            }
        }
    }

    // PRECONDITION: v is not the root
    int indexInPath(int v) {
        assert(v != root);
        int topOfPath = heavyPaths[heavyPathIndex[v]][0];
        return tree.level[tree.P[0][v]] - tree.level[topOfPath];
    }

    // PRECONDITION: path is the index of v's heavy path
    // PRECONDITION: v is not the root
    int indexInPath(int path, int v) {
        assert(v != root);
        int topOfPath = heavyPaths[path][0];
        return tree.level[tree.P[0][v]] - tree.level[topOfPath];
    }
};

template <typename T, typename BinOp = function<T(T, T)>>
struct CompactSegmentTree {
    int       RealN;
    int       N;            // the size of array
    vector<T> tree;         //

    T         defaultValue;
    BinOp     mergeOp;

    explicit CompactSegmentTree(BinOp op, T dflt = T())
        : RealN(0), N(0), tree(), mergeOp(op), defaultValue(dflt) {
    }

    CompactSegmentTree(int size, BinOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        init(size, alignPowerOf2);
    }

    CompactSegmentTree(T value, int n, BinOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        build(value, n, alignPowerOf2);
    }

    CompactSegmentTree(const T arr[], int n, BinOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        build(arr, n, alignPowerOf2);
    }

    CompactSegmentTree(const vector<T>& v, BinOp op, T dflt = T(), bool alignPowerOf2 = false)
        : mergeOp(op), defaultValue(dflt) {
        build(v, alignPowerOf2);
    }


    void init(int size, bool alignPowerOf2 = false) {
        RealN = size;
        if (alignPowerOf2) {
#ifndef __GNUC__
            N = 1 << (32 - (int)_lzcnt_u32(size - 1));
#else
            N = 1 << (32 - __builtin_clz(size - 1));
#endif
        } else
            N = size + (size & 1);
        tree.assign(N * 2, defaultValue);
    }

    void build(T value, int size, bool alignPowerOf2 = false) {
        init(size, alignPowerOf2);

        for (int i = 0; i < size; i++)
            tree[N + i] = value;

        for (int i = N - 1; i > 0; i--)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    void build(const T arr[], int size, bool alignPowerOf2 = false) {
        init(size, alignPowerOf2);

        for (int i = 0; i < size; i++)
            tree[N + i] = arr[i];

        for (int i = N - 1; i > 0; i--)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    void build(const vector<T>& v, bool alignPowerOf2 = false) {
        build(&v[0], (int)v.size(), alignPowerOf2);
    }

    //--- query

    T query(int index) const {
        return tree[index + N];
    }

    // inclusive
    T query(int left, int right) const {
        T resL = defaultValue;
        T resR = defaultValue;

        for (int L = left + N, R = right + N + 1; L < R; L >>= 1, R >>= 1) {
            if (L & 1)
                resL = mergeOp(resL, tree[L++]);
            if (R & 1)
                resR = mergeOp(tree[--R], resR);
        }

        return mergeOp(resL, resR);
    }

    //--- update

    void update(int index, T newValue) {
        tree[index + N] = newValue;

        for (int i = (index + N) >> 1; i > 0; i >>= 1)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    // inclusive
    void updateRange(int left, int right, T newValue) {
        for (int L = left + N, R = right + N + 1; L < R; L++)
            tree[L] = newValue;

        for (int L = (left + N) >> 1, R = (right + N) >> 1; L > 0; L >>= 1, R >>= 1) {
            for (int i = L; i <= R; i++)
                tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
        }
    }

    //--- add

    void add(int index, T value) {
        tree[index + N] += value;

        for (int i = (index + N) >> 1; i > 0; i >>= 1)
            tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
    }

    // inclusive
    void addRange(int left, int right, T value) {
        for (int L = left + N, R = right + N + 1; L < R; L++)
            tree[L] += value;

        for (int L = (left + N) >> 1, R = (right + N) >> 1; L > 0; L >>= 1, R >>= 1) {
            for (int i = L; i <= R; i++)
                tree[i] = mergeOp(tree[i << 1], tree[(i << 1) | 1]);
        }
    }
};

template <typename T, typename BinOp = function<T(T, T)>>
struct HLDPathQuery {
    HeavyLightDecomposition& hld;

    typedef CompactSegmentTree<T, BinOp> SegTreeT;

    T                   defaultValue;
    BinOp               mergeOp;
    vector<SegTreeT>    segTrees;

    explicit HLDPathQuery(HeavyLightDecomposition& hld, T dflt = T()) : hld(hld), mergeOp(), defaultValue(dflt) {
        // no action
    }

    HLDPathQuery(HeavyLightDecomposition& hld, BinOp op, T dflt = T()) : hld(hld), mergeOp(op), defaultValue(dflt) {
        // no action
    }

    void build() {
        segTrees.clear();
        segTrees.reserve(hld.heavyPaths.size());

        // make segment trees on all heavy path
        for (const auto& path : hld.heavyPaths) {
            int m = (int)path.size() - 1;
            segTrees.push_back(SegTreeT(m, mergeOp, defaultValue));
        }
    }

    void build(T initValue) {
        segTrees.clear();
        segTrees.reserve(hld.heavyPaths.size());

        // make segment trees on all heavy path
        for (const auto& path : hld.heavyPaths) {
            int m = (int)path.size() - 1;
            segTrees.push_back(SegTreeT(m, mergeOp, defaultValue));
            for (int i = 0; i < m; ++i)
                segTrees.back().update(i, initValue);
        }
    }


    // update a value of an edge(u-v)
    void update(int u, int v, T cost) {
        if (u >= 0 && hld.tree.P[0][u] == v)
            swap(u, v);
        //assert(gP[0][v] == u);

        int path = hld.heavyPathIndex[v];
        int index = hld.indexInPath(path, v);
        segTrees[path].update(index, cost);
    }

    T query(int u, int v) const {
        int lca = hld.tree.findLCA(u, v);
        return mergeOp(queryTopdown(lca, u), queryTopdown(lca, v));
    }

protected:
    // return max value a path from u to v (u is an ancestor of v)
    T queryTopdown(int u, int v) const {
        if (u == v)
            return defaultValue;

        if (hld.heavyPathIndex[u] == hld.heavyPathIndex[v]) {
            int path = hld.heavyPathIndex[u];

            int first = hld.indexInPath(path, u) + 1;
            int last = hld.indexInPath(path, v);
            return segTrees[path].query(first, last);
        }

        int path = hld.heavyPathIndex[v];
        int topOfPath = hld.heavyPaths[path][0];

        //assert(topOfPath != v);

        int last = hld.indexInPath(path, v);
        return mergeOp(queryTopdown(u, topOfPath), segTrees[path].query(0, last));
    }
};

template <typename T, typename BinOp>
HLDPathQuery<T, BinOp> makeHLDPathQuery(HeavyLightDecomposition& hld, BinOp op, T dfltValue = T()) {
    return HLDPathQuery<T, BinOp>(hld, op, dfltValue);
}


int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    Tree tree(N);

    unordered_set<ll> road;
    unordered_set<ll> broken;

    vector<pair<int, int>> edges;
    for (int i = 1; i < N; i++) {
        int x, y;
        cin >> x >> y;
        x--, y--;

        int u = min(x, y);
        int v = max(x, y);
        edges.emplace_back(u, v);

        tree.addEdge(u, v);
        road.insert((1ll * u << 32) + v);
    }

    tree.build(0);
    HeavyLightDecomposition hld(tree);
    hld.doHLD(0);

    auto query = makeHLDPathQuery<ll>(hld, [](ll a, ll b) { return a + b; }, 0);
    query.build(1);

#define INF 1000000ll

    int Q;
    cin >> Q;
    while (Q-- > 0) {
        char ch;
        int x, y;
        cin >> ch >> x >> y;
        x--, y--;

        int u = min(x, y);
        int v = max(x, y);
        ll key = (1ll * u << 32) + v;

        if (ch == 'd') {
            if (broken.find(key) != broken.end() || road.find(key) == road.end())
                continue;
            broken.insert(key);
            query.update(u, v, INF);
        } else if (ch == 'c') {
            if (broken.find(key) == broken.end() || road.find(key) == road.end())
                continue;
            broken.erase(key);
            query.update(u, v, 1ll);
        } else {
            ll d = query.query(u, v);
            if (d >= INF)
                cout << "Impossible" << endl;
            else
                cout << d << endl;
        }
    }

    return 0;
}
