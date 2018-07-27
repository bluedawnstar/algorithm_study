#pragma once

#ifndef CACHE_BLOCK_NODES
#define CACHE_BLOCK_NODES
#endif

template <typename T>
struct BlockTreeSubtreeSum {
    int                 N;                  // the number of vertex
    int                 sqrtN;              // sqrt(N) or sqrt(H)

    vector<vector<int>> edges;              // edges (vertex number)
    vector<int>         parent;             // 

    vector<vector<int>> blockEdges;         // edges (block number)
    vector<int>         blockRoot;          // 
    vector<int>         blockRootLevel;     // it's valid if a node is a root of subtree
    vector<int>         levelInBlock;       // 
    vector<int>         treeSizeInBlock;    // 

    enum LazyT {
        lzNone,
        lzSet,
        lzAdd
    };
    vector<LazyT>       blockLazy;          // 
    vector<T>           blockLazyValues;    // 

    vector<T>           values;             // 
    vector<T>           pathValuesInBlock;  // 
    vector<T>           subtreeValuesInBlock;// 

    BlockTreeSubtreeSum() : N(0), sqrtN(0) {
    }

    explicit BlockTreeSubtreeSum(int n, int sqrtN = 0) {
        init(n, sqrtN);
    }

    void init(int _n, int _sqrtN = 0) {
        N = _n;
        sqrtN = _sqrtN;
        if (sqrtN <= 0)
            sqrtN = int(sqrt(N));

        edges = vector<vector<int>>(N);
        parent.assign(N, -1);

        blockEdges = vector<vector<int>>(N);
        blockRoot.assign(N, 0);
        blockRootLevel.assign(N, 0);
        levelInBlock.assign(N, 0);
        treeSizeInBlock.assign(N, 0);

        blockLazy.assign(N, lzNone);
        blockLazyValues.assign(N, 0);

        values.assign(N, 0);
        pathValuesInBlock.assign(N, 0);
        subtreeValuesInBlock.assign(N, 0);
    }

    //--- build

    // before calling build()
    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    // before calling build()
    void setInitValue(int u, T value) {
        values[u] = value;
    }

    // O(N)
    void build(int root) {
        dfsBuild(root, -1, 0);
    }

    //--- basic operations

    int depth(int u) const {
        return blockRootLevel[blockRoot[u]] + levelInBlock[u];
    }

    //--- lca

    // O(sqrt(N))
    int lca(int u, int v) const {
        while (blockRoot[u] != blockRoot[v]) {
            if (blockRootLevel[blockRoot[u]] > blockRootLevel[blockRoot[v]])
                swap(u, v);
            v = parent[blockRoot[v]];
        }
        return lcaNaive(u, v);
    }

    // dist >= 0, O(sqrt(N))
    int climb(int u, int dist) const {
        int p = parent[blockRoot[u]];
        while (p >= 0 && levelInBlock[u] < dist) {
            dist -= levelInBlock[u] + 1;
            u = p;
            p = parent[blockRoot[u]];
        }
        return climbNaive(u, dist);
    }

    // kth >= 1, O(sqrt(N))
    // count = valueToCountF(index_of_node, value_or_sqrt_value_of_the_node)
    int climbKth(int u, int kth, const function<int(T)>& valueToCountF) const {
        int p = parent[blockRoot[u]];
        int d;
        while (p >= 0 && (d = valueToCountF(pathValuesInBlock[u])) < kth) {
            kth -= d;
            u = p;
            p = parent[blockRoot[u]];
        }
        return climbKthNaive(u, kth, valueToCountF);
    }

    //--- update

    // O(sqrt(N))
    void update(int u, T val) {
        updateValue(u, false, val);
    }

    // O(sqrt(N))
    void add(int u, T val) {
        updateValue(u, true, val);
    }


    // O(sqrt(N))
    void updateSubtree(int u, T val) {
        updateSubtree(u, false, val);
    }

    // O(sqrt(N))
    void addSubtree(int u, T val) {
        updateSubtree(u, true, val);
    }

    //--- query

    // O(sqrt(N) * a)
    T queryToRoot(int u) {
        applyBlock(u);

        T res = pathValuesInBlock[u];
        u = parent[blockRoot[u]];
        while (u >= 0) {
            applyBlock(u);
            res += pathValuesInBlock[u];
            u = parent[blockRoot[u]];
        }
        return res;
    }

    // O(sqrt(N) * a)
    T queryToAncestor(int u, int ancestor) {
        applyBlock(ancestor);

        T res = 0;
        while (blockRoot[u] != blockRoot[ancestor]) {
            applyBlock(u);
            res += pathValuesInBlock[u];
            u = parent[blockRoot[u]];
        }
        res += pathValuesInBlock[u] - pathValuesInBlock[ancestor] + values[ancestor];
        return res;
    }

    // dist >= 0, O(sqrt(N) * a)
    T queryTowardRoot(int u, int dist) {
        T res = 0;
        int p = parent[blockRoot[u]];
        while (p >= 0 && levelInBlock[u] < dist) {
            applyBlock(u);
            dist -= levelInBlock[u] + 1;
            res += pathValuesInBlock[u];
            u = p;
            p = parent[blockRoot[u]];
        }
        applyBlock(u);
        res += queryTowardRootNaive(u, dist);
        return res;
    }

    // O(sqrt(N) * a)
    T query(int u, int v) {
        applyBlock(u);
        if (u == v)
            return values[u];

        T res = 0;
        while (blockRoot[u] != blockRoot[v]) {
            if (blockRootLevel[blockRoot[u]] > blockRootLevel[blockRoot[v]])
                swap(u, v);
            applyBlock(v);
            res += pathValuesInBlock[v];
            v = parent[blockRoot[v]];
        }

        int lc = lcaNaive(u, v);
        applyBlock(lc);

        res += pathValuesInBlock[u] + pathValuesInBlock[v] - 2 * pathValuesInBlock[lc] + values[lc];

        return res;
    }

    T querySubtree(int u) {
        applyBlock(u);
        return subtreeValuesInBlock[u] + dfsQuerySubtree(u, parent[u], blockRoot[u]);
    }

protected:
#if 0
    void dfsBuild(int u, int par, int depth) {
        parent[u] = par;
        if (depth % sqrtN == 0) {
            blockRoot[u] = u;
            blockRootLevel[u] = depth;
            levelInBlock[u] = 0;

#ifdef CACHE_BLOCK_NODES
            if (par >= 0)
                blockEdges[par].push_back(u);
#else
            if (par >= 0)
                blockEdges[blockRoot[par]].push_back(u);
#endif
            pathValuesInBlock[u] = values[u];
        } else {
            blockRoot[u] = blockRoot[par];
            blockRootLevel[u] = blockRootLevel[par];
            levelInBlock[u] = levelInBlock[par] + 1;

            pathValuesInBlock[u] = pathValuesInBlock[par] + values[u];
        }

        treeSizeInBlock[u] = 1;
        subtreeValuesInBlock[u] = values[u];
#ifdef CACHE_BLOCK_NODES
        blockEdges[u].clear();
#endif
        for (auto v : edges[u]) {
            if (v != par)
                dfsBuild(v, u, depth + 1);
        }

        if (depth % sqrtN != 0) {
            treeSizeInBlock[par] += treeSizeInBlock[u];
            subtreeValuesInBlock[par] += subtreeValuesInBlock[u];
#ifdef CACHE_BLOCK_NODES
            blockEdges[par].insert(blockEdges[par].end(), blockEdges[u].begin(), blockEdges[u].end());
#endif
        }
    }
#else
    void dfsBuild(int u, int par, int depth) {
        int cnt = 0;
        vector<vector<int>> nodes;

        // a root of a subtree
        {
            parent[u] = par;
            nodes.push_back({ u });
            cnt++;

            blockRoot[u] = u;
            blockRootLevel[u] = depth;
            levelInBlock[u] = 0;

#ifdef CACHE_BLOCK_NODES
            if (par >= 0)
                blockEdges[par].push_back(u);
#else
            if (par >= 0)
                blockEdges[blockRoot[par]].push_back(u);
#endif
            pathValuesInBlock[u] = values[u];

            treeSizeInBlock[u] = 1;
            subtreeValuesInBlock[u] = values[u];
#ifdef CACHE_BLOCK_NODES
            blockEdges[u].clear();
#endif
        }
        while (cnt < sqrtN) {
            vector<int> vec;
            for (auto p : nodes.back()) {
                for (auto v : edges[p]) {
                    if (v == parent[p])
                        continue;

                    parent[v] = p;
                    vec.push_back(v);
                    cnt++;

                    blockRoot[v] = blockRoot[u];
                    blockRootLevel[v] = blockRootLevel[u];
                    levelInBlock[v] = levelInBlock[p] + 1;

                    pathValuesInBlock[v] = pathValuesInBlock[p] + values[v];

                    treeSizeInBlock[v] = 1;
                    subtreeValuesInBlock[v] = values[v];
#ifdef CACHE_BLOCK_NODES
                    blockEdges[v].clear();
#endif
                }
            }
            if (vec.empty())
                break;
            nodes.push_back(move(vec));
        }

        for (auto p : nodes.back()) {
            for (auto v : edges[p]) {
                if (v != parent[p])
                    dfsBuild(v, p, depth + int(nodes.size()));
            }
        }

        for (int i = int(nodes.size()) - 1; i > 0; i--) {
            for (auto v : nodes[i]) {
                auto p = parent[v];
                treeSizeInBlock[p] += treeSizeInBlock[v];
                subtreeValuesInBlock[p] += subtreeValuesInBlock[v];
#ifdef CACHE_BLOCK_NODES
                blockEdges[p].insert(blockEdges[p].end(), blockEdges[v].begin(), blockEdges[v].end());
#endif
            }
        }
    }
#endif

    //--- lca

    int lcaNaive(int u, int v) const {
        if (u == v)
            return u;

        if (levelInBlock[u] > levelInBlock[v])
            swap(u, v);

        while (levelInBlock[u] != levelInBlock[v])
            v = parent[v];

        while (u != v) {
            u = parent[u];
            v = parent[v];
        }
        return u;
    }

    int climbNaive(int u, int dist) const {
        while (u >= 0 && dist-- > 0)
            u = parent[u];
        return u;
    }

    int climbKthNaive(int u, int kth, const function<int(T)>& valueToCountF) const {
        while (u >= 0 && kth > 0) {
            if (valueToCountF(values[u])) {
                if (--kth == 0)
                    break;
            }
            u = parent[u];
        }
        return u;
    }

    //--- applyBlock

    void dfsApply(int u, int p, int blockId, LazyT lazy, T val) {
        if (blockRoot[u] != blockId)
            return;

        if (lazy == lzSet)
            values[u] = val;
        else
            values[u] += val;

        if (blockRoot[u] == u)
            pathValuesInBlock[u] = values[u];
        else
            pathValuesInBlock[u] = pathValuesInBlock[p] + val;

        subtreeValuesInBlock[u] = values[u];
        for (auto v : edges[u]) {
            if (v != p)
                dfsApply(v, u, blockId, lazy, val);
        }
        if (blockRoot[u] != u)
            subtreeValuesInBlock[p] += subtreeValuesInBlock[u];
    }

    void applyBlock(int u) {
        int blockId = blockRoot[u];
        if (blockLazy[blockId] == lzNone)
            return;

        dfsApply(blockId, parent[blockId], blockId, blockLazy[blockId], blockLazyValues[blockId]);

        blockLazy[blockId] = lzNone;
        blockLazyValues[blockId] = 0;
    }

    //--- update a value

    void dfsPushDownPathValue(int u, int p, int blockId) {
        if (blockRoot[u] != blockId)
            return;

        if (blockRoot[u] == u)
            pathValuesInBlock[u] = values[u];
        else
            pathValuesInBlock[u] = pathValuesInBlock[p] + values[u];

        for (auto v : edges[u]) {
            if (v != p)
                dfsPushDownPathValue(v, u, blockId);
        }
    }

    void pushUpSubtreeValue(int u, int blockId, T delta) {
        while (u >= 0 && blockRoot[u] == blockId) {
            subtreeValuesInBlock[u] += delta;
            u = parent[u];
        }
    }

    void updateValue(int u, bool add, T val) {
        applyBlock(u);

        T delta = val;
        if (!add) {
            if (values[u] == val)
                return;
            delta -= values[u];
            values[u] = val;
        } else {
            if (!val)
                return;
            values[u] += val;
        }

        dfsPushDownPathValue(u, parent[u], blockRoot[u]);
        pushUpSubtreeValue(u, blockRoot[u], delta);
    }

    //--- update a subtree

    void dfsPushDownBlockValues(int u, bool add, T val) {
        assert(blockRoot[u] == u);

        if (add == false) {
            blockLazy[u] = lzSet;
            blockLazyValues[u] = val;
            subtreeValuesInBlock[u] = val * treeSizeInBlock[u];
        } else {
            if (blockLazy[u] == lzNone)
                blockLazy[u] = lzAdd;
            blockLazyValues[u] += val;
            subtreeValuesInBlock[u] += val * treeSizeInBlock[u];
        }

        for (auto v : blockEdges[u])
            dfsPushDownBlockValues(v, add, val);
    }

    void dfsPushDownSubtreeValue(int u, int p, int blockId, LazyT lazy, T val) {
        if (blockRoot[u] != blockId) {
            dfsPushDownBlockValues(u, lazy == lzAdd, val);
            return;
        }

        if (lazy == lzSet)
            values[u] = val;
        else
            values[u] += val;

        if (blockRoot[u] == u)
            pathValuesInBlock[u] = values[u];
        else
            pathValuesInBlock[u] = pathValuesInBlock[p] + val;

        subtreeValuesInBlock[u] = values[u];
        for (auto v : edges[u]) {
            if (v != p) {
                dfsPushDownSubtreeValue(v, u, blockId, lazy, val);
                if (blockRoot[v] != v)
                    subtreeValuesInBlock[u] += subtreeValuesInBlock[v];
            }
        }
    }

    void updateSubtree(int u, bool add, T val) {
        if (add && !val)
            return;

        applyBlock(u);

        auto oldSubtreeValue = subtreeValuesInBlock[u];
        dfsPushDownSubtreeValue(u, parent[u], blockRoot[u], add ? lzAdd : lzSet, val);
        pushUpSubtreeValue(parent[u], blockRoot[u], subtreeValuesInBlock[u] - oldSubtreeValue);
    }

    //--- query

    T queryToAncestorNaive(int u, int ancestor) const {
        T res = values[u];
        if (u != ancestor) {
            do {
                u = parent[u];
                res += values[u];
            } while (u != ancestor);
        }
        return res;
    }

    T queryTowardRootNaive(int u, int dist) const {
        T res = values[u];
        if (dist > 0) {
            do {
                u = parent[u];
                res += values[u];
            } while (u >= 0 && --dist > 0);
        }
        return res;
    }

    T dfsQueryBlockValues(int u) {
        T res = subtreeValuesInBlock[u];
        for (auto v : blockEdges[u])
            res += dfsQueryBlockValues(v);
        return res;
    }

    T dfsQuerySubtree(int u, int p, int blockId) {
#ifdef CACHE_BLOCK_NODES
        T res = 0;
        for (auto v : blockEdges[u]) {
            res += dfsQueryBlockValues(v);
        }
        return res;
#else
        if (blockRoot[u] != blockId)
            return dfsQueryBlockValues(u);

        T res = 0;
        for (auto v : edges[u]) {
            if (v != p)
                res += dfsQuerySubtree(v, u, blockId);
        }
        return res;
#endif
    }
};
