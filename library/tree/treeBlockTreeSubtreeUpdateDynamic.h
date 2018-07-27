#pragma once

template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct DynamicBlockTreeSubtreeUpdate {
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

    DynamicBlockTreeSubtreeUpdate(const MergeOp& mop, const BlockOp& bop, T dflt = T())
        : N(0), sqrtN(0), mergeOp(mop), blockOp(bop), defaultValue(dflt) {
    }

    DynamicBlockTreeSubtreeUpdate(int n, const MergeOp& mop, const BlockOp& bop, T dflt = T(), int sqrtN = 0)
        : mergeOp(mop), blockOp(bop), defaultValue(dflt) {
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

    // O(N)
    void rebuild(int root) {
        dfsApplyBlockToRebuild(root);
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


    // O(sqrt(N))
    void cut(int u) {
        int p = parent[u];
        if (p < 0)
            return;

        applyBlock(u);

        edges[p].erase(find(edges[p].begin(), edges[p].end(), u));
        edges[u].erase(find(edges[u].begin(), edges[u].end(), p));
        parent[u] = -1;

        if (blockRoot[u] == u) {
            int blockP = blockRoot[p];
            blockEdges[blockP].erase(find(blockEdges[blockP].begin(), blockEdges[blockP].end(), u));
        } else {
            int blockId = blockRoot[u];

            pushUpSubtreeValueAndSize(p, blockId, -treeSizeInBlock[u]);

            dfsGetSubBlocks(u, parent[u], blockId, blockEdges[u]);
            if (!blockEdges[u].empty()) {
                vector<unsigned int> S((N + 31) >> 5);
                for (auto v : blockEdges[u])
                    S[v >> 5] |= 1u << (v & 0x1F);

                vector<int>& be = blockEdges[blockId];

                int j = 0;
                for (int i = 0; i < int(be.size()); i++) {
                    int v = be[i];
                    if ((S[v >> 5] & (1u << (v & 0x1F))) == 0)
                        be[j++] = v;
                }
                be.resize(j);
            }
        }
    }

    // O(sqrt(N))
    void link(int u, int par) {
        if (par < 0)
            return;

        applyBlock(par);

        edges[par].push_back(u);
        edges[u].push_back(par);
        parent[u] = par;

        int blockP = blockRoot[par];
        if (blockRoot[u] == u) {
            blockEdges[blockP].push_back(u);
            pushDownBlockLevel(u);
        } else {
            int blockId = blockRoot[u];

            pushUpSubtreeValueAndSize(par, blockRoot[par], treeSizeInBlock[u]);
            dfsPushDownNewParent(u, par, blockRoot[u], blockRoot[par]);

            if (!blockEdges[u].empty()) {
                blockEdges[blockP].insert(blockEdges[blockP].end(), blockEdges[u].begin(), blockEdges[u].end());
                for (auto v : blockEdges[u])
                    pushDownBlockLevel(v);
                blockEdges[u].clear();
            }
        }
    }

    // O(sqrt(N))
    // PRECONDITION: u must be not root
    // PRECONDITION: newParent >= 0
    void changeParent(int u, int newParent) {
        int oldParent = parent[u];
        if (newParent == oldParent)
            return;

        cut(u);
        link(u, newParent);
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
        applyBlock(u);
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

protected:
#if 0
    void dfsBuild(int u, int par, int depth) {
        parent[u] = par;
        if (depth % sqrtN == 0) {
            blockRoot[u] = u;
            blockRootLevel[u] = depth;
            levelInBlock[u] = 0;

            if (par >= 0)
                blockEdges[blockRoot[par]].push_back(u);

            pathValuesInBlock[u] = values[u];
        } else {
            blockRoot[u] = blockRoot[par];
            blockRootLevel[u] = blockRootLevel[par];
            levelInBlock[u] = levelInBlock[par] + 1;

            pathValuesInBlock[u] = mergeOp(pathValuesInBlock[par], values[u]);
        }

        treeSizeInBlock[u] = 1;
        subtreeValuesInBlock[u] = values[u];
        for (auto v : edges[u]) {
            if (v != par)
                dfsBuild(v, u, depth + 1);
        }

        if (depth % sqrtN != 0) {
            treeSizeInBlock[par] += treeSizeInBlock[u];
            subtreeValuesInBlock[par] = mergeOp(subtreeValuesInBlock[par], subtreeValuesInBlock[u]);
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

            if (par >= 0)
                blockEdges[blockRoot[par]].push_back(u);

            pathValuesInBlock[u] = values[u];

            treeSizeInBlock[u] = 1;
            subtreeValuesInBlock[u] = values[u];
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
                subtreeValuesInBlock[p] = mergeOp(subtreeValuesInBlock[p], subtreeValuesInBlock[v]);
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


    void dfsApplyToRebuild(int u, int p, int blockId, LazyT lazy, T val) {
        if (blockRoot[u] != blockId)
            return;

        if (lazy == lzSet)
            values[u] = val;
        else
            values[u] += val;

        for (auto v : edges[u]) {
            if (v != p)
                dfsApplyToRebuild(v, u, blockId, lazy, val);
        }
    }

    void dfsApplyBlockToRebuild(int u) {
        if (blockLazy[u] != lzNone) {
            dfsApplyToRebuild(u, parent[u], u, blockLazy[u], blockLazyValues[u]);
            blockLazy[u] = lzNone;
            blockLazyValues[u] = defaultValue;
        }
        for (auto v : blockEdges[u])
            dfsApplyBlockToRebuild(v);
        blockEdges[u].clear();
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
            if (v != p) {
                dfsPushDownSubtreeValue(v, u, blockId, lazy, val);
                if (blockRoot[v] != v)
                    subtreeValuesInBlock[u] = mergeOp(subtreeValuesInBlock[u], subtreeValuesInBlock[v]);
            }
        }
    }

    void updateSubtree(int u, bool add, T val) {
        if (add && !val)
            return;

        applyBlock(u);

        dfsPushDownSubtreeValue(u, parent[u], blockRoot[u], add ? lzAdd : lzSet, val);
        pushUpSubtreeValue(parent[u], blockRoot[u]);
    }

    //--- change parent

    void dfsGetSubBlocks(int u, int p, int blockId, vector<int>& list) {
        if (blockRoot[u] != blockId) {
            list.push_back(u);
            return;
        }
        for (auto v : edges[u]) {
            if (v != p)
                dfsGetSubBlocks(v, u, blockId, list);
        }
    }

    void pushUpSubtreeValueAndSize(int u, int blockId, int sizeDelta) {
        while (u >= 0 && blockRoot[u] == blockId) {
            auto t = values[u];
            for (auto v : edges[u]) {
                if (v != parent[u] && blockRoot[v] != v)
                    t = mergeOp(t, subtreeValuesInBlock[v]);
            }
            subtreeValuesInBlock[u] = t;
            treeSizeInBlock[u] += sizeDelta;

            u = parent[u];
        }
    }

    void dfsPushDownNewParent(int u, int p, int oldBlockId, int newBlockId) {
        if (blockRoot[u] != oldBlockId)
            return;

        if (blockRoot[u] == u) {
            pathValuesInBlock[u] = values[u];
            levelInBlock[u] = 0;
        } else {
            pathValuesInBlock[u] = mergeOp(pathValuesInBlock[p], values[u]);
            levelInBlock[u] = levelInBlock[p] + 1;
        }
        blockRoot[u] = newBlockId;

        for (auto v : edges[u]) {
            if (v != p)
                dfsPushDownNewParent(v, u, oldBlockId, newBlockId);
        }
    }

    void dfsPushDownBlockLevel(int u, int depth) {
        assert(blockRoot[u] == u);

        for (auto v : blockEdges[u]) {
            blockRootLevel[v] = depth + levelInBlock[parent[v]] + 1;
            dfsPushDownBlockLevel(v, blockRootLevel[v]);
        }
    }

    void pushDownBlockLevel(int u) {
        assert(blockRoot[u] == u);

        int p = parent[u];
        if (p < 0)
            blockRootLevel[u] = 0;
        else
            blockRootLevel[u] = blockRootLevel[blockRoot[p]] + levelInBlock[p] + 1;

        dfsPushDownBlockLevel(u, blockRootLevel[u]);
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
        if (blockRoot[u] != blockId)
            return dfsQueryBlockValues(u);

        T res = defaultValue;
        for (auto v : edges[u]) {
            if (v != p)
                res = mergeOp(res, dfsQuerySubtree(v, u, blockId));
        }
        return res;
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline DynamicBlockTreeSubtreeUpdate<T, MergeOp, BlockOp>
makeDynamicBlockTreeSubtreeUpdate(const MergeOp& mop, const BlockOp& bop, T dfltValue) {
    return DynamicBlockTreeSubtreeUpdate<T, MergeOp, BlockOp>(mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline DynamicBlockTreeSubtreeUpdate<T, MergeOp, BlockOp>
makeDynamicBlockTreeSubtreeUpdate(int size, const MergeOp& mop, const BlockOp& bop, T dfltValue, int sqrtN = 0) {
    return DynamicBlockTreeSubtreeUpdate<T, MergeOp, BlockOp>(size, mop, bop, dfltValue, sqrtN);
}
