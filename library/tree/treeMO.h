#pragma once

#include "treeEulerTour.h"

/* MO's algorithm

1. How to use
    1) build EulerTourTree
        EulerTourTree tree(N, logN);
        //... add edges to tree
        tree.build();

    2) construct TreeMO
        TreeMO treeMO(tree);
        treeMO.build(Q, [](int t, int u) { ... }, [](int t, int u) { ... });

2. onAdd and onRemove
    int curAns;

    // t : time, u : node index
    void onAdd(int t, int u) {
        // TODO: implement adding function
        curAns += ...;
    }

    // t : time, u : node index
    void onRemove(int t, int u) {
        //TODO: implement removing function
        curAns -= ...;
    }
*/
struct TreeMO {
    EulerTourTree&              tree;

    vector<bool>                mActive;

    function<void(int, int)>    mOnAdd;
    function<void(int, int)>    mOnRemove;

    vector<int>                 lca;        // lca[Q index] -> lca of (L, R)
    vector<tuple<int, int, int>> MO;        // (MO_L, MO_R, Q index)

    // Q[i] = (left, right), inclusive
    TreeMO(EulerTourTree& _tree) : tree(_tree) {
    }

    void build(vector<pair<int, int>>& Q, function<void(int, int)> onAdd, function<void(int, int)> onRemove) {
        mActive = vector<bool>(tree.mN);
        mOnAdd = onAdd;
        mOnRemove = onRemove;

        lca.reserve(Q.size());
        MO.reserve(Q.size());
        for (int i = 0; i < (int)Q.size(); i++) {
            //assert(mVisTime[Q[i].first][0] <= mVisTime[Q[i].second][0]);
            int L = Q[i].first, R = Q[i].second;

            int lc = tree.findLCA(L, R);
            lca.push_back(lc == L ? -1 : lc);
            if (lc == L)
                MO.push_back(make_tuple(tree.mVisTime[L].first, tree.mVisTime[R].first, i));
            else
                MO.push_back(make_tuple(tree.mVisTime[L].second, tree.mVisTime[R].first, i));
        }

        int blockN = (int)sqrt(2 * tree.mN);
        sort(MO.begin(), MO.end(), [blockN](const auto& l, const auto& r) {
            if (get<0>(l) / blockN != get<0>(r) / blockN)
                return get<0>(l) / blockN < get<0>(r) / blockN;
            return get<1>(l) < get<1>(r);
        });
    }

    //--- query ---------------------------------------------------------------
    
    void add(int t) {
        int u = tree.mTime2Node[t];
        if (mActive[u]) {
            remove(t);
        } else {
            mActive[u] = true;
            mOnAdd(t, u);
        }
    }

    void remove(int t) {
        int u = tree.mTime2Node[t];
        if (!mActive[u]) {
            add(t);
        } else {
            mActive[u] = false;
            mOnRemove(t, u);
        }
    }
};
