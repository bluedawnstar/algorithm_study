#pragma once

#include "treeBasic.h"

struct DfsTourTree : public Tree {
    vector<pair<int, int>>  mVisTime;   // node index to (visit time, leave time)
    vector<int>             mTime2Node; // time to node index (0 <= time < 2 * N)
    int                     mCurrTime;  //

    //--- tree construction ---------------------------------------------------

    DfsTourTree() {
        mN = 0;
        mLogN = 0;
    }

    DfsTourTree(int N, int logN) : Tree(N, logN), mVisTime(N), mTime2Node(N * 2) {
        mCurrTime = 0;
    }

    void init(int N, int logN) {
        Tree::init(N, logN);

        mVisTime.assign(N, make_pair(0, 0));
        mTime2Node.assign(N * 2, 0);
        mCurrTime = 0;
    }

    void build(int root) {
        dfs(root, -1);
        //dfsIter(root, -1);
        makeLcaTable();
    }

    // return true if u is an ancestor of v
    bool isAncestor(int u, int v) const {
        return mVisTime[u].first <= mVisTime[v].first && mVisTime[v].second <= mVisTime[u].second;
    }

private:
    //--------- DFS -----------------------------------------------------------
    void dfs(int u, int parent) {
        mVisTime[u].first = mCurrTime;
        mTime2Node[mCurrTime++] = u;

        mP[0][u] = parent;
        for (int v : mE[u]) {
            if (v == parent)
                continue;

            mLevel[v] = mLevel[u] + 1;
            dfs(v, u);
        }

        mVisTime[u].second = mCurrTime;
        mTime2Node[mCurrTime++] = u;
    }

    void dfsIter(int root) {
        struct Item {
            int u;
            int parent;
            int vi;         // child index
        };
        vector<Item> st;
        st.reserve(mN);

        st.push_back(Item{ root, -1, -1 });
        while (!st.empty()) {
            Item& it = st.back();
            if (++it.vi == 0) {
                // enter ...
                mP[0][it.u] = it.parent;
                mVisTime[it.u].first = mCurrTime;
                mTime2Node[mCurrTime++] = it.u;
            }
            if (it.vi >= (int)mE[it.u].size()) {
                // leave ...
                mVisTime[it.u].second = mCurrTime;
                mTime2Node[mCurrTime++] = it.u;
                st.pop_back();
            } else if (mE[it.u][it.vi] != it.parent) {
                // recursion
                int v = mE[it.u][it.vi];
                mLevel[v] = mLevel[it.u] + 1;
                st.push_back(Item{ v, it.u, -1 });
            }
        }
    }
};
