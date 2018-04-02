#pragma once

#include "treeDfsTour.h"

/* MO's algorithm

1. How to use
    1) build DfsTourTree
        DfsTourTree tree(N, logN);
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
    DfsTourTree&                tree;

    vector<bool>                active;

    function<void(int, int)>    onAdd;
    function<void(int, int)>    onRemove;

    vector<int>                 lca;        // lca[Q index] -> lca of (L, R)
    vector<tuple<int, int, int>> MO;        // (MO_L, MO_R, Q index)

    // Q[i] = (left, right), inclusive
    TreeMO(DfsTourTree& _tree) : tree(_tree) {
    }

    void build(vector<pair<int, int>>& Q, function<void(int, int)> onAdd, function<void(int, int)> onRemove) {
        active = vector<bool>(tree.N);
        this->onAdd = onAdd;
        this->onRemove = onRemove;

        lca.reserve(Q.size());
        MO.reserve(Q.size());
        for (int i = 0; i < (int)Q.size(); i++) {
            //assert(visTime[Q[i].first][0] <= visTime[Q[i].second][0]);
            int L = Q[i].first, R = Q[i].second;

            int lc = tree.findLCA(L, R);
            lca.push_back(lc == L ? -1 : lc);
            if (lc == L)
                MO.emplace_back(tree.visTime[L].first, tree.visTime[R].first, i);
            else
                MO.emplace_back(tree.visTime[L].second, tree.visTime[R].first, i);
        }

        int blockN = (int)sqrt(2 * tree.N);
        sort(MO.begin(), MO.end(), [blockN](const auto& l, const auto& r) {
            if (get<0>(l) / blockN != get<0>(r) / blockN)
                return get<0>(l) / blockN < get<0>(r) / blockN;
            return get<1>(l) < get<1>(r);
        });
    }

    //--- query ---------------------------------------------------------------
    
    void add(int t) {
        int u = tree.time2Node[t];
        if (active[u]) {
            remove(t);
        } else {
            active[u] = true;
            onAdd(t, u);
        }
    }

    void remove(int t) {
        int u = tree.time2Node[t];
        if (!active[u]) {
            add(t);
        } else {
            active[u] = false;
            onRemove(t, u);
        }
    }
};
