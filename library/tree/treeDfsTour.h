#pragma once

#include "treeBasic.h"

struct DfsTourTree : public Tree {
    vector<pair<int, int>> visTime;     // node index to (visit time, leave time)
    vector<int>            time2Node;   // time to node index (0 <= time < 2 * N)
    int                    currTime;    //

    //--- tree construction ---------------------------------------------------

    DfsTourTree() : Tree() {
        currTime = 0;
    }

    DfsTourTree(int N, int logN) : Tree(N, logN), visTime(N), time2Node(N * 2) {
        currTime = 0;
    }

    void init(int N, int logN) {
        Tree::init(N, logN);

        visTime.assign(N, make_pair(0, 0));
        time2Node.assign(N * 2, 0);
        currTime = 0;
    }

    void build(int root) {
        dfs(root, -1);
        //dfsIter(root, -1);
        makeLcaTable();
    }

    // return true if u is an ancestor of v
    bool isAncestor(int u, int v) const {
        return visTime[u].first <= visTime[v].first && visTime[v].second <= visTime[u].second;
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

        visTime[u].second = currTime;
        time2Node[currTime++] = u;
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
                visTime[it.u].second = currTime;
                time2Node[currTime++] = it.u;
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
