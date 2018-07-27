#pragma once

#include "treeBasic.h"

#include "../rangeQuery/fenwickTree.h"

// It is based on DFS tour tree
template <typename T>
struct DfsTourTreePathSum : public Tree {
    vector<pair<int, int>> visTime;     // node index to (visit time, leave time)
    vector<int>            time2Node;   // time to node index (0 <= time < 2 * N)
    int                    currTime;    //

    FenwickTree<T>  ft;

    DfsTourTreePathSum() : Tree() {
        currTime = 0;
    }

    explicit DfsTourTreePathSum(int N, int logN = 0) {
        init(N, logN);
    }

    void init(int N, int logN = 0) {
        Tree::init(N, logN);

        visTime.assign(N, make_pair(0, 0));
        time2Node.assign(N * 2, 0);
        currTime = 0;

        ft.init(N * 2);
    }

    void build(int root) {
        dfs(root, -1);
        //dfsIter(root);
        makeLcaTable();
    }

    //-- update

    void add(int u, T value) {
        ft.add(visTime[u].first, value);
        ft.add(visTime[u].second, -value);
    }

    void set(int u, T value) {
        ft.set(visTime[u].first, value);
        ft.set(visTime[u].second, -value);
    }

    //--- query

    T query(int u) {
        return ft.get(visTime[u].first);
    }

    T query(int u, int v) {
        int lc = findLCA(u, v);
        return ft.sum(visTime[u].first) + ft.sum(visTime[v].first) - ft.sum(visTime[lc].first) * 2 + ft.get(visTime[lc].first);
    }

    T queryExcludeLCA(int u, int v) {
        int lc = findLCA(u, v);
        return ft.sum(visTime[u].first) + ft.sum(visTime[v].first) - ft.sum(visTime[lc].first) * 2;
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
            if (it.vi >= int(edges[it.u].size())) {
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

// tree connectivity test

// PRECONDITION: u-v is directly connected
template <typename T>
inline void breakEdge(DfsTourTreePathSum<T>& connectivityTestTree, int u, int v) {
    if (u < 0 || v < 0)
        return;
    if (connectivityTestTree.level[u] > connectivityTestTree.level[v])
        swap(u, v);
    if (connectivityTestTree.query(v) == 0)
        connectivityTestTree.add(v, 1);
}

// PRECONDITION: u-v is directly connected
template <typename T>
inline void recoverEdge(DfsTourTreePathSum<T>& connectivityTestTree, int u, int v) {
    if (u < 0 || v < 0)
        return;
    if (connectivityTestTree.level[u] > connectivityTestTree.level[v])
        swap(u, v);
    if (connectivityTestTree.query(v) != 0)
        connectivityTestTree.add(v, -1);
}

template <typename T>
inline bool isConnected(DfsTourTreePathSum<T>& connectivityTestTree, int u, int v) {
    if (u == v)
        return true;
    return connectivityTestTree.queryExcludeLCA(u, v) == 0;
}
