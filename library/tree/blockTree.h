#pragma once

// <How to Use>
//  1. construct without size -> init() -> add() -> build() -> update() / query()
//  2. construct with size -> add() -> build() -> update() / query()

// Sqrt-Tree of Tree
template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct BlockTree {
    struct Fragment {
        const BlockTree& tree;

        int         id;
        T           value;

        int         maxLevel;

        int         parent;
        vector<int> nodes;
        vector<int> levelFreq;

        Fragment(int id, const BlockTree& tree) : tree(tree), id(id) {
            value = tree.defaultValue;
            maxLevel = -1;
            parent = -1;
        }

        void add(int v) {
            int levelDelta = tree.level[v] - tree.level[id];
            nodes.push_back(v);

            if (v >= tree.N)
                return;

            if (levelDelta > maxLevel) {
                maxLevel = levelDelta;
                levelFreq.resize(maxLevel + 1);
            }
            levelFreq[levelDelta]++;
        }

        void update(int level, T val) {
            int levelDelta = level - tree.level[id];
            if (levelDelta < 0 || levelDelta > maxLevel)
                return;
            value = tree.mergeOp(value, tree.blockOp(val, levelFreq[levelDelta]));
        }

        T query() const {
            return value;
        }
    };

    vector<vector<int>> edges;          // all nodes
    int                 N;              // 
    int                 sqrtN;          // 
    vector<int>         level;          // 
    vector<int>         parent;         // 
    vector<int>         treeSize;       // 
    vector<T>           valueInLevel;   // 

    vector<vector<int>> edges2;         // fragment nodes
    vector<int>         fragId;         // node id -> fragment id
    vector<Fragment>    frags;          // 

    T                   defaultValue;
    MergeOp             mergeOp;
    BlockOp             blockOp;

    BlockTree(MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
    }

    BlockTree(int size, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        init(size);
    }

    //--- initialize

    void init(int size) {
        N = size;
        edges.assign(size, vector<int>());
        parent.assign(size, -1);
        level.assign(size, 0);
        fragId.assign(size, -1);
        valueInLevel.assign(size, defaultValue);
        treeSize.assign(size, 0);
        sqrtN = max((int)sqrt(N), 2);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
    }

    //--- build

    void build() {
        dfsL(0);

        vector<vector<int>> nodesByLevel(N);
        for (int i = 0; i < N; ++i)
            nodesByLevel[N - 1 - level[i]].push_back(i);

        for (auto& nodes : nodesByLevel) {
            for (int u : nodes) {
                treeSize[u] = 1;
                for (int v : edges[u])
                    treeSize[u] += treeSize[v];

                int size = 0;
                vector<int> curChildren, newChildren;
                for (int v : edges[u]) {
                    size += treeSize[v];
                    curChildren.push_back(v);

                    if (size > sqrtN) {
                        newChildren.push_back(createFragment(u, curChildren));

                        curChildren.clear();
                        treeSize[u] -= size;
                        size = 0;
                    }
                }

                edges[u] = curChildren;
                edges[u].insert(edges[u].end(), newChildren.begin(), newChildren.end());
            }
        }
        // create a root fragment node and build a tree of fragment nodes
        {
            int node = createFragment(-1, vector<int>(1, 0));

            edges2 = vector<vector<int>>(frags.size());
            for (int i = 0; i < (int)frags.size(); i++) {
                Fragment& f = frags[i];
                if (parent[f.id] == -1)
                    continue;

                f.parent = fragId[parent[f.id]];
                edges2[f.parent].push_back(i);
            }
        }
    }

    //--- basic operation

    // node id of the fragment of node u
    int leader(int u) const {
        return frags[fragId[u]].id;
    }

    int lca(int u, int v) const {
        while (leader(u) != leader(v)) {
            if (level[leader(u)] > level[leader(v)])
                u = parent[leader(u)];
            else
                v = parent[leader(v)];
        }

        while (u != v) {
            if (level[v] > level[u])
                v = parent[v];
            else
                u = parent[u];
        }

        return u;
    }

    //--- query

    // query the value of u's subtree (including u)
    T querySubtree(int u) const {
        T res = defaultValue;

        if (leader(u) == u) {
            res = frags[fragId[u]].query();
            for (int v : edges2[fragId[u]])
                res = mergeOp(res, querySubtree(frags[v].id));
        } else {
            res = valueInLevel[level[u]];
            for (int v : edges[u])
                res = mergeOp(res, querySubtree(v));
        }

        return res;
    }

    //--- update

    // update all nodes to have same level
    void updateLevel(int level, T val) {
        valueInLevel[level] = mergeOp(valueInLevel[level], val);
        for (Fragment& f : frags)
            f.update(level, val);
    }

private:
    void dfsL(int u, int depth = 0) {
        level[u] = depth;
        treeSize[u] = 1;
        for (int v : edges[u]) {
            dfsL(v, depth + 1);
            treeSize[u] += treeSize[v];
        }
    }

    int createNode(int parentNode, const vector<int>& children) {
        fragId.push_back(-1);
        parent.push_back(parentNode);
        level.push_back(parentNode <= 0 ? 0 : level[parentNode] + 1);

        int node = (int)edges.size();
        edges.push_back(children);

        return node;
    }

    int createFragment(int parentNode, const vector<int>& children) {
        int node = createNode(parentNode, children);
        frags.push_back(Fragment(node, *this));
        buildFragment(node, frags.back());

        return node;
    }

    void buildFragment(int u, Fragment& frag) {
        if (fragId[u] != -1)
            return;
        fragId[u] = (int)frags.size() - 1;

        frag.add(u);
        for (int v : edges[u])
            buildFragment(v, frag);
    }
};

template <typename T, typename MergeOp, typename BlockOp>
BlockTree<T, MergeOp, BlockOp> makeBlockTree(int size, MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return BlockTree<T, MergeOp, BlockOp>(size, mop, bop, dfltValue);
}
