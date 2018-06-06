#pragma once

#include "treeBasic.h"

template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct TreePathSqrtDecomposition : public Tree {
    int         root;                   // 
    int         sqrtN;                  // 
    int         branchN;                // 
    vector<int> branchSize;             // 
    vector<int> branchHead;             // 
    vector<int> branchTail;             // 
    vector<int> nodeToBranch;           // 

    enum LazyT {
        lzNone,
        lzSet,
        lzAdd
    };

    vector<T>       values;             // 
    vector<T>       branchValues;       // 
    vector<LazyT>   branchLazy;         // 
    vector<T>       branchLazyValues;   // 

    T               defaultValue;       // 
    MergeOp         mergeOp;            // 
    BlockOp         blockOp;            // 

    TreePathSqrtDecomposition(const MergeOp& mop, const BlockOp& bop, T dfltValue = T())
        : Tree(), mergeOp(mop), blockOp(bop), defaultValue(dfltValue)  {
    }

    explicit TreePathSqrtDecomposition(int N, const MergeOp& mop, const BlockOp& bop, T dfltValue = T(), int logN = 0)
        : Tree(N, logN), mergeOp(mop), blockOp(bop), defaultValue(dfltValue) {
        init(N, logN);
    }

    void init(int N, int logN = 0) {
        Tree::init(N, logN);

        sqrtN = (int)sqrt(N);
        nodeToBranch.assign(N, 0);
        branchSize.reserve((int)sqrt(N) * 2);
        branchHead.reserve((int)sqrt(N) * 2);
        branchTail.reserve((int)sqrt(N) * 2);
    }

    void build(int root) {
        this->root = root;
        branchSize.push_back(1);
        branchHead.push_back(root);
        branchTail.push_back(root);
        nodeToBranch[root] = 0;

        dfs(root, -1);
        //dfsIter(root);
        makeLcaTable();

        branchN = (int)branchSize.size();
        values.assign(N, defaultValue);
        branchValues.assign(branchN, defaultValue);
        branchLazy.assign(branchN, lzNone);
        branchLazyValues.assign(branchN, defaultValue);
    }

    // call after build()
    void setValue(int v, T val) {
        values[v] = val;

        int b = nodeToBranch[v];
        branchValues[b] = mergeOp(branchValues[b], val);
    }

    //--- lca

    //NOTE: findLCA() is faster than lca2()
    int lca2(int u, int v) const {
        while (nodeToBranch[v] != nodeToBranch[u]) {
            if (level[branchHead[nodeToBranch[u]]] > level[branchHead[nodeToBranch[v]]])
                swap(u, v);
            v = branchHead[nodeToBranch[v]];
            v = P[0][v];
        }
        return level[u] <= level[v] ? u : v;
    }

    // kth >= 1, O(sqrt(N))
    // count = valueToCountF(index_of_node, value_or_sqrt_value_of_the_node)
    int climbKth(int v, int kth, const function<int(T)>& valueToCountF) const {
        while (kth > 0) {
            int b = nodeToBranch[v];
            if (v == branchTail[b]) {
                int d = valueToCountF(branchValues[b]);
                if (d >= kth)
                    break;
                kth -= d;
                v = branchHead[b];
                v = P[0][v];
            } else {
                while (kth > 0) {
                    if (valueToCountF(values[v])) {
                        if (--kth == 0)
                            break;
                    }
                    if (v == branchHead[b]) {
                        v = P[0][v];
                        break;
                    }
                    v = P[0][v];
                }
            }
        }

        while (v >= 0 && kth > 0) {
            if (valueToCountF(values[v])) {
                if (--kth == 0)
                    break;
            }
            v = P[0][v];
        }
        return v;
    }

    // from top
    int indexInBranch(int v) const {
        int b = nodeToBranch[v];
        return level[v] - level[branchHead[b]];
    }

    int indexInBranch(int branch, int v) const {
        return level[v] - level[branchHead[branch]];
    }


    //--- update

    // O(sqrt(N))
    void update(int v, int u, T val) {
        while (nodeToBranch[v] != nodeToBranch[u]) {
            if (level[branchHead[nodeToBranch[u]]] > level[branchHead[nodeToBranch[v]]])
                swap(u, v);

            int b = nodeToBranch[v];
            if (v == branchTail[b])
                updateBranch(b, val);
            else
                updateInBranch(v, branchHead[b], val);
            v = branchHead[b];
            v = P[0][v];
        }
        updateInBranch(v, u, val);
    }

    void add(int v, int u, T val) {
        while (nodeToBranch[v] != nodeToBranch[u]) {
            if (level[branchHead[nodeToBranch[u]]] > level[branchHead[nodeToBranch[v]]])
                swap(u, v);

            int b = nodeToBranch[v];
            if (v == branchTail[b])
                addBranch(b, val);
            else
                addInBranch(v, branchHead[b], val);
            v = branchHead[b];
            v = P[0][v];
        }
        addInBranch(v, u, val);
    }

    //--- query

    // O(sqrt(N))
    T query(int u, int v) {
        T res = defaultValue;
        while (nodeToBranch[v] != nodeToBranch[u]) {
            if (level[branchHead[nodeToBranch[u]]] > level[branchHead[nodeToBranch[v]]])
                swap(u, v);

            int b = nodeToBranch[v];
            if (v == branchTail[b])
                res = mergeOp(res, branchValues[b]);
            else
                res = mergeOp(res, queryInBranch(v, branchHead[b]));
            v = branchHead[b];
            v = P[0][v];
        }
        return mergeOp(res, queryInBranch(v, u));
    }

private:
    //--------- DFS -----------------------------------------------------------
    void dfs(int u, int parent) {
        bool first = true;
        if (branchSize.back() >= sqrtN)
            first = false;

        P[0][u] = parent;
        for (int v : edges[u]) {
            if (v == parent)
                continue;

            if (!first) {
                branchSize.push_back(1);
                branchHead.push_back(v);
                branchTail.push_back(v);
            } else {
                branchTail.back() = v;
                ++branchSize.back();
                first = false;
            }
            nodeToBranch[v] = (int)branchSize.size() - 1;

            level[v] = level[u] + 1;
            dfs(v, u);
        }
    }

    void dfsIter(int root) {
        struct Item {
            int u;
            int parent;
            int vi;         // child index
            bool first;
        };
        vector<Item> st;
        st.reserve(N);

        st.push_back(Item{ root, -1, -1 });
        while (!st.empty()) {
            Item& it = st.back();
            if (++it.vi == 0) {
                // enter ...
                it.first = true;
                if (branchSize.back() >= sqrtN)
                    it.first = false;

                P[0][it.u] = it.parent;
            }
            if (it.vi >= (int)edges[it.u].size()) {
                // leave ...
                st.pop_back();
            } else if (edges[it.u][it.vi] != it.parent) {
                // recursion
                int v = edges[it.u][it.vi];

                if (!it.first) {
                    branchSize.push_back(1);
                    branchHead.push_back(v);
                    branchTail.push_back(v);
                } else {
                    branchTail.back() = v;
                    it.first = false;
                }
                nodeToBranch[v] = (int)branchSize.size() - 1;

                level[v] = level[it.u] + 1;
                st.push_back(Item{ v, it.u, -1 });
            }
        }
    }

    void applyBranch(int branch) {
        if (branchLazy[branch] == lzNone)
            return;

        T val = branchLazyValues[branch];

        int u = branchHead[branch];
        int v = branchTail[branch];
        if (branchLazy[branch] == lzSet) {
            values[v] = val;
            while (v != u) {
                v = P[0][v];
                values[v] = val;
            }
        } else {
            values[v] += val;
            while (v != u) {
                v = P[0][v];
                values[v] += val;
            }
        }

        branchLazy[branch] = lzNone;
        branchLazyValues[branch] = defaultValue;
    }

    void recalcBranchValue(int branch) {
        int u = branchHead[branch];
        int v = branchTail[branch];

        T val = values[v];
        while (v != u) {
            v = P[0][v];
            val = mergeOp(val, values[v]);
        }
        branchValues[branch] = val;
    }

    void updateBranch(int branch, T val) {
        branchValues[branch] = blockOp(val, branchSize[branch]);
        branchLazy[branch] = lzSet;
        branchLazyValues[branch] = val;
    }

    void addBranch(int branch, T val) {
        branchValues[branch] += val * branchSize[branch];
        if (branchLazy[branch] != lzSet)
            branchLazy[branch] = lzAdd;
        branchLazyValues[branch] += val;
    }


    void updateInBranch(int v, int u, T val) {
        if (level[u] > level[v])
            swap(u, v);

        int b = nodeToBranch[v];
        applyBranch(b);

        values[v] = val;
        while (v != u) {
            v = P[0][v];
            values[v] = val;
        }
        recalcBranchValue(b);
    }

    void addInBranch(int v, int u, T val) {
        if (level[u] > level[v])
            swap(u, v);

        int b = nodeToBranch[v];
        applyBranch(b);

        values[v] += val;
        while (v != u) {
            v = P[0][v];
            values[v] += val;
        }
        recalcBranchValue(b);
    }

    T queryInBranch(int v, int u) {
        if (level[u] > level[v])
            swap(u, v);

        int b = nodeToBranch[v];
        if (branchLazy[b] == lzSet)
            return blockOp(branchLazyValues[b], level[v] - level[u] + 1);

        applyBranch(b);

        T res = values[v];
        while (v != u) {
            v = P[0][v];
            res = mergeOp(res, values[v]);
        }
        
        return res;
    }
};


template <typename T, typename MergeOp, typename BlockOp>
inline TreePathSqrtDecomposition<T, MergeOp, BlockOp> makeTreePathSqrtDecomposition(const MergeOp& mop, const BlockOp& bop, T dfltValue) {
    return TreePathSqrtDecomposition<T, MergeOp, BlockOp>(mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline TreePathSqrtDecomposition<T, MergeOp, BlockOp> makeTreePathSqrtDecomposition(int size, const MergeOp& mop, const BlockOp& bop, T dfltValue) {
    return TreePathSqrtDecomposition<T, MergeOp, BlockOp>(size, mop, bop, dfltValue);
}
