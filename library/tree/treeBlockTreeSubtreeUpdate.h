#pragma once

#define CACHE_BLOCK_NODES

template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct BlockTreeSubtreeUpdate {
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

    T                   defaultValue;       // 
    MergeOp             mergeOp;            // 
    BlockOp             blockOp;            // 

    BlockTreeSubtreeUpdate(const MergeOp& mop, const BlockOp& bop, T dflt = T())
        : N(0), sqrtN(0), mergeOp(mop), blockOp(bop), defaultValue(dflt) {
    }

    BlockTreeSubtreeUpdate(int n, const MergeOp& mop, const BlockOp& bop, T dflt = T(), int sqrtN = 0)
        : mergeOp(mop), blockOp(bop), defaultValue(dflt) {
        init(n, sqrtN);
    }

    void init(int _n, int _sqrtN = 0) {
        N = _n;
        sqrtN = _sqrtN;
        if (sqrtN <= 0)
            sqrtN = (int)sqrt(N);

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
            res = mergeOp(res, pathValuesInBlock[u]);
            u = parent[blockRoot[u]];
        }
        return res;
    }

    // O(sqrt(N) * a)
    T queryToAncestor(int u, int ancestor) {
        applyBlock(ancestor);

        T res = defaultValue;
        while (blockRoot[u] != blockRoot[ancestor]) {
            applyBlock(u);
            res = mergeOp(res, pathValuesInBlock[u]);
            u = parent[blockRoot[u]];
        }
        return mergeOp(res, queryToAncestorNaive(u, ancestor));
    }

    // dist >= 0, O(sqrt(N) * a)
    T queryTowardRoot(int u, int dist) {
        T res = defaultValue;
        int p = parent[blockRoot[u]];
        while (p >= 0 && levelInBlock[u] < dist) {
            applyBlock(u);
            dist -= levelInBlock[u] + 1;
            res = mergeOp(res, pathValuesInBlock[u]);
            u = p;
            p = parent[blockRoot[u]];
        }
        applyBlock(u);
        return mergeOp(res, queryTowardRootNaive(u, dist));
    }

    // O(sqrt(N) * a)
    T query(int u, int v) {
        if (u == v)
            return values[u];

        T res = defaultValue;
        while (blockRoot[u] != blockRoot[v]) {
            if (blockRootLevel[blockRoot[u]] > blockRootLevel[blockRoot[v]])
                swap(u, v);
            applyBlock(v);
            res = mergeOp(res, pathValuesInBlock[v]);
            v = parent[blockRoot[v]];
        }

        int lc = lcaNaive(u, v);
        applyBlock(lc);

        if (lc == u)
            return mergeOp(res, queryToAncestorNaive(v, u));
        else if (lc == v)
            return mergeOp(res, queryToAncestorNaive(u, v));
        else
            return mergeOp(res, mergeOp(queryToAncestorNaive(u, lc), queryTowardRootNaive(v, levelInBlock[v] - levelInBlock[lc] - 1)));
    }

    T querySubtree(int u) {
        applyBlock(u);
        return mergeOp(subtreeValuesInBlock[u], dfsQuerySubtree(u, parent[u], blockRoot[u]));
    }

    //--- for accumulative operation
    // Use this functions when MergeOp is 'add' (supporting subtraction)

    // O(sqrt(N) * a)
    T queryToAncestorAccumulative(int u, int ancestor) {
        applyBlock(ancestor);

        T res = defaultValue;
        while (blockRoot[u] != blockRoot[ancestor]) {
            applyBlock(u);
            res = mergeOp(res, pathValuesInBlock[u]);
            u = parent[blockRoot[u]];
        }
        return mergeOp(res, pathValuesInBlock[u] - pathValuesInBlock[ancestor] + values[ancestor]);
    }

    // O(sqrt(N) * a)
    T queryAccumulative(int u, int v) {
        if (u == v)
            return values[u];

        T res = defaultValue;
        while (blockRoot[u] != blockRoot[v]) {
            if (blockRootLevel[blockRoot[u]] > blockRootLevel[blockRoot[v]])
                swap(u, v);
            applyBlock(v);
            res = mergeOp(res, pathValuesInBlock[v]);
            v = parent[blockRoot[v]];
        }

        int lc = lcaNaive(u, v);
        applyBlock(lc);

        return mergeOp(res, pathValuesInBlock[u] + pathValuesInBlock[v] - 2 * pathValuesInBlock[lc] + values[lc]);
    }

    //---------------------

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

            pathValuesInBlock[u] = mergeOp(pathValuesInBlock[par], values[u]);
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
            subtreeValuesInBlock[par] = mergeOp(subtreeValuesInBlock[par], subtreeValuesInBlock[u]);
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

                    pathValuesInBlock[v] = mergeOp(pathValuesInBlock[p], values[v]);

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
                    dfsBuild(v, p, depth + (int)nodes.size());
            }
        }

        for (int i = (int)nodes.size() - 1; i > 0; i--) {
            for (auto v : nodes[i]) {
                auto p = parent[v];
                treeSizeInBlock[p] += treeSizeInBlock[v];
                subtreeValuesInBlock[p] = mergeOp(subtreeValuesInBlock[p], subtreeValuesInBlock[v]);
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
            pathValuesInBlock[u] = mergeOp(pathValuesInBlock[p], val);

        subtreeValuesInBlock[u] = values[u];
        for (auto v : edges[u]) {
            if (v != p)
                dfsApply(v, u, blockId, lazy, val);
        }
        if (blockRoot[u] != u)
            subtreeValuesInBlock[p] = mergeOp(subtreeValuesInBlock[p], subtreeValuesInBlock[u]);
    }

    void applyBlock(int u) {
        int blockId = blockRoot[u];
        if (blockLazy[blockId] == lzNone)
            return;

        dfsApply(blockId, parent[blockId], blockId, blockLazy[blockId], blockLazyValues[blockId]);

        blockLazy[blockId] = lzNone;
        blockLazyValues[blockId] = defaultValue;
    }

    //--- update a value

    void dfsPushDownPathValue(int u, int p, int blockId) {
        if (blockRoot[u] != blockId)
            return;

        if (blockRoot[u] == u)
            pathValuesInBlock[u] = values[u];
        else
            pathValuesInBlock[u] = mergeOp(pathValuesInBlock[p], values[u]);

        for (auto v : edges[u]) {
            if (v != p)
                dfsPushDownPathValue(v, u, blockId);
        }
    }

    void pushUpSubtreeValue(int u, int blockId) {
        while (u >= 0 && blockRoot[u] == blockId) {
            auto t = values[u];
            for (auto v : edges[u]) {
                if (v != parent[u] && blockRoot[v] != v)
                    t = mergeOp(t, subtreeValuesInBlock[v]);
            }
            subtreeValuesInBlock[u] = t;

            u = parent[u];
        }
    }

    void updateValue(int u, bool add, T val) {
        applyBlock(u);

        if (!add) {
            if (values[u] == val)
                return;
            values[u] = val;
        } else {
            if (!val)
                return;
            values[u] += val;
        }

        dfsPushDownPathValue(u, parent[u], blockRoot[u]);
        pushUpSubtreeValue(u, blockRoot[u]);
    }

    //--- update a subtree

    void dfsPushDownBlockValues(int u, bool add, T val) {
        assert(blockRoot[u] == u);

        if (add == false) {
            blockLazy[u] = lzSet;
            blockLazyValues[u] = val;
            subtreeValuesInBlock[u] = blockOp(val, treeSizeInBlock[u]);
        } else {
            if (blockLazy[u] == lzNone)
                blockLazy[u] = lzAdd;
            blockLazyValues[u] += val;
            subtreeValuesInBlock[u] += val * treeSizeInBlock[u];            //TODO: fix it
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
            pathValuesInBlock[u] = mergeOp(pathValuesInBlock[p], val);

        subtreeValuesInBlock[u] = values[u];
        for (auto v : edges[u]) {
            if (v != p)
                dfsPushDownSubtreeValue(v, u, blockId, lazy, val);
        }
        if (blockRoot[u] != u)
            subtreeValuesInBlock[p] = mergeOp(subtreeValuesInBlock[p], subtreeValuesInBlock[u]);
    }

    void updateSubtree(int u, bool add, T val) {
        if (add && !val)
            return;

        applyBlock(u);

        dfsPushDownSubtreeValue(u, parent[u], blockRoot[u], add ? lzAdd : lzSet, val);
        pushUpSubtreeValue(parent[u], blockRoot[u]);
    }

    //--- query

    T queryToAncestorNaive(int u, int ancestor) const {
        T res = values[u];
        if (u != ancestor) {
            do {
                u = parent[u];
                res = mergeOp(res, values[u]);
            } while (u != ancestor);
        }
        return res;
    }

    T queryTowardRootNaive(int u, int dist) const {
        T res = values[u];
        if (dist > 0) {
            do {
                u = parent[u];
                res = mergeOp(res, values[u]);
            } while (u >= 0 && --dist > 0);
        }
        return res;
    }

    T dfsQueryBlockValues(int u) {
        T res = subtreeValuesInBlock[u];
        for (auto v : blockEdges[u])
            res = mergeOp(res, dfsQueryBlockValues(v));
        return res;
    }

    T dfsQuerySubtree(int u, int p, int blockId) {
#ifdef CACHE_BLOCK_NODES
        T res = defaultValue;
        for (auto v : blockEdges[u]) {
            res = mergeOp(res, dfsQueryBlockValues(v));
        }
        return res;
#else
        if (blockRoot[u] != blockId)
            return dfsQueryBlockValues(u);

        T res = defaultValue;
        for (auto v : edges[u]) {
            if (v != p)
                res = mergeOp(res, dfsQuerySubtree(v, u, blockId));
        }
        return res;
#endif
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline BlockTreeSubtreeUpdate<T, MergeOp, BlockOp>
makeBlockTreeSubtreeUpdate(const MergeOp& mop, const BlockOp& bop, T dfltValue) {
    return BlockTreeSubtreeUpdate<T, MergeOp, BlockOp>(mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline BlockTreeSubtreeUpdate<T, MergeOp, BlockOp>
makeBlockTreeSubtreeUpdate(int size, const MergeOp& mop, const BlockOp& bop, T dfltValue, int sqrtN = 0) {
    return BlockTreeSubtreeUpdate<T, MergeOp, BlockOp>(size, mop, bop, dfltValue, sqrtN);
}
