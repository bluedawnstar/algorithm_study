#pragma once

#include "treeBasic.h"

#include "../rangeQuery/segmentTreeCompactLazy.h"

// It is based on DFS tour tree
template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct DfsTourTreeSubtreeUpdate : public Tree {
    vector<pair<int, int>> visTime;     // node index to (visit time, leave time)
    vector<int>            time2Node;   // time to node index (0 <= time < 2 * N)
    int                    currTime;    //

    CompactSegmentTreeLazyUpdate<T, MergeOp, BlockOp> seg;

    DfsTourTreeSubtreeUpdate(const MergeOp& mop, const BlockOp& bop, T dflt = T())
        : Tree(), seg(mop, bop, dflt) {
        currTime = 0;
    }

    DfsTourTreeSubtreeUpdate(int N, const MergeOp& mop, const BlockOp& bop, T dflt = T())
        : Tree(), seg(mop, bop, dflt) {
        init(N, false, 0);
    }

    void init(int N, bool alignPowerOf2 = false, int logN = 0) {
        Tree::init(N, logN);

        visTime.assign(N, make_pair(0, 0));
        time2Node.assign(N, 0);
        currTime = 0;

        seg.init(N, alignPowerOf2);
    }

    void build(int root) {
        dfs(root, -1);
        //dfsIter(root, -1);
        makeLcaTable();
    }

    //-- update

    void update(int u, T value) {
        seg.updateRange(visTime[u].first, visTime[u].second, value);    // update the subtree of u
    }

    //--- query

    T query(int u) {
        return seg.query(visTime[u].first, visTime[u].second);
    }

private:
    //--------- DFS -----------------------------------------------------------
    void dfs(int u, int parent) {
        visTime[u].first = currTime;
        time2Node[currTime++] = u;

        P[0][u] = parent;
        for (int v : edges[u]) {
            if (v == parent)
                continue;

            level[v] = level[u] + 1;
            dfs(v, u);
        }

        visTime[u].second = currTime - 1;
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
                P[0][it.u] = it.parent;
                visTime[it.u].first = currTime;
                time2Node[currTime++] = it.u;
            }
            if (it.vi >= (int)edges[it.u].size()) {
                // leave ...
                visTime[it.u].second = currTime - 1;
                st.pop_back();
            } else if (edges[it.u][it.vi] != it.parent) {
                // recursion
                int v = edges[it.u][it.vi];
                level[v] = level[it.u] + 1;
                st.push_back(Item{ v, it.u, -1 });
            }
        }
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline DfsTourTreeSubtreeUpdate<T, MergeOp, BlockOp> makeDfsTourTreeSubtreeUpdate(const MergeOp& mop, const BlockOp& bop, T dfltValue = T()) {
    return DfsTourTreeSubtreeUpdate<T, MergeOp, BlockOp>(mop, bop, dfltValue);
}

template <typename T, typename MergeOp, typename BlockOp>
inline DfsTourTreeSubtreeUpdate<T, MergeOp, BlockOp> makeDfsTourTreeSubtreeUpdate(int size, const MergeOp& mop, const BlockOp& bop, T dfltValue = T()) {
    return DfsTourTreeSubtreeUpdate<T, MergeOp, BlockOp>(size, mop, bop, dfltValue);
}