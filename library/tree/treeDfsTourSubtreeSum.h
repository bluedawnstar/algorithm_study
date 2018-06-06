#pragma once

#include "treeBasic.h"

#include "../rangeQuery/fenwickTreeMultAdd.h"

// It is based on DFS tour tree
template <typename T>
struct DfsTourTreeSubtreeSum : public Tree {
    vector<pair<int, int>> visTime;     // node index to (visit time, leave time)
    vector<int>            time2Node;   // time to node index (0 <= time < 2 * N)
    int                    currTime;    //

    FenwickTreeMultAdd<T>  ft;

    DfsTourTreeSubtreeSum() : Tree() {
        currTime = 0;
    }

    explicit DfsTourTreeSubtreeSum(int N, int logN = 0) {
        init(N, logN);
    }

    void init(int N, int logN = 0) {
        Tree::init(N, logN);

        visTime.assign(N, make_pair(0, 0));
        time2Node.assign(N, 0);
        currTime = 0;

        ft.init(N);
    }

    void build(int root) {
        dfs(root, -1);
        //dfsIter(root);
        makeLcaTable();
    }

    //-- update

    void add(int u, T value) {
        ft.addRange(visTime[u].first, visTime[u].second, value);
    }

    //--- query

    T query(int u) {
        return ft.sumRange(visTime[u].first, visTime[u].second);
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