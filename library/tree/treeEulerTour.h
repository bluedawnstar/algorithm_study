#pragma once

#include "treeBasic.h"
#include "../rangeQuery/sparseTableMinIndex.h"

struct EulerTourTree : public Tree {
    vector<int>     mFirstVisTime;  // node index to first visit time
    vector<int>     mEuler;         // time to node index
    SparseTableMinIndex mRMQ;       // RMQ for LCA

    //--- tree construction ---------------------------------------------------

    EulerTourTree() {
        mN = 0;
        mLogN = 0;
    }

    EulerTourTree(int N, int logN) : Tree(N, logN), mFirstVisTime(N, -1) {
    }

    void init(int N, int logN) {
        Tree::init(N, logN);
        mFirstVisTime.assign(N, -1);
    }

    void build(int root) {
        dfs(root, -1);
        makeLcaTable();

        vector<int> level(mEuler.size());
        for (int i = 0; i < (int)mEuler.size(); i++)
            level[i] = mLevel[mEuler[i]];
        mRMQ.build(level, (int)mEuler.size());
    }

    //--------- DFS -----------------------------------------------------------

    void dfs(int u, int parent) {
        if (mFirstVisTime[u] < 0)
            mFirstVisTime[u] = (int)mEuler.size();

        mEuler.push_back(u);

        mP[0][u] = parent;
        for (int v : mE[u]) {
            if (v == parent)
                continue;

            mLevel[v] = mLevel[u] + 1;
            dfs(v, u);

            mEuler.push_back(u);
        }
    }

    //--------- LCA -----------------------------------------------------------

    // O(1) - faster than findLCA() when N < 75000 experimentally
    //      - slower than findLCA() when N >= 75000 experimentally because Euler path is longer than node size
    int findLCAFast(int nodeL, int nodeR) {
        int timeL = mFirstVisTime[nodeL];
        int timeR = mFirstVisTime[nodeR];
        if (timeL > timeR)
            swap(timeL, timeR);
        return mEuler[mRMQ.query(timeL, timeR)];
    }
};
