#pragma once

#include "treeBasic.h"
#include "../rangeQuery/sparseTableMinIndex.h"

struct EulerTourTree : public Tree {
    vector<int>         firstVisTime;   // node index to first visit time
    vector<int>         euler;          // time to node index
    SparseTableMinIndex rmq;            // RMQ for LCA

    //--- tree construction ---------------------------------------------------

    EulerTourTree() : Tree() {
    }

    EulerTourTree(int N, int logN) : Tree(N, logN), firstVisTime(N, -1) {
    }

    void init(int N, int logN) {
        Tree::init(N, logN);
        firstVisTime.assign(N, -1);
    }

    void build(int root) {
        dfs(root, -1);
        makeLcaTable();

        vector<int> level(euler.size());
        for (int i = 0; i < (int)euler.size(); i++)
            level[i] = level[euler[i]];
        rmq.build(level);
    }

    //--------- DFS -----------------------------------------------------------

    void dfs(int u, int parent) {
        if (firstVisTime[u] < 0)
            firstVisTime[u] = (int)euler.size();

        euler.push_back(u);

        P[0][u] = parent;
        for (int v : edges[u]) {
            if (v == parent)
                continue;

            level[v] = level[u] + 1;
            dfs(v, u);

            euler.push_back(u);
        }
    }

    //--------- LCA -----------------------------------------------------------

    // O(1) - faster than findLCA() when N < 75000 experimentally
    //      - slower than findLCA() when N >= 75000 experimentally because Euler path is longer than node size
    int findLCAFast(int nodeL, int nodeR) {
        int timeL = firstVisTime[nodeL];
        int timeR = firstVisTime[nodeR];
        if (timeL > timeR)
            swap(timeL, timeR);
        return euler[rmq.query(timeL, timeR)];
    }
};
