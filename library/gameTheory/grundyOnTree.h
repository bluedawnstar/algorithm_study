#pragma once

// https://www.hackerearth.com/practice/algorithms/graphs/depth-first-search/practice-problems/algorithm/tree-game-2-e6796bef/description/

template <int MaxBitN = 31>
struct GrundyOnTree {
    int N;
    vector<vector<int>> edges;

    vector<MexTrie<MaxBitN>> mexTrie;
    vector<int> lazy;
    vector<int> grundy; // grundy[u] = grundy number of sub-tree of u

    GrundyOnTree() : N(0) {
    }

    explicit GrundyOnTree(int n) {
        init(n);
    }

    void init(int n) {
        N = n;
        edges = vector<vector<int>>(n);

        mexTrie = vector<MexTrie<MaxBitN>>(n);
        lazy = vector<int>(n);
        grundy = vector<int>(n);
    }

    void addEdge(int u, int v) {
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    //---

    void build(int root) {
        dfsGrundy(root, -1);
    }

    int getGrundy(int u) const {
        return grundy[u];
    }

private:
    /*
        grundy number G

        G[u] = mex{ G[state0][v0] ^ G[state0][v1] ^ ...,
                    G[state1][v0] ^ G[state0][v1] ^ ...,
                    G[state2][v0] ^ G[state0][v1] ^ ...,
                    ...
                  }
    */
    void dfsGrundy(int u, int parent) {
        int childGrundy = 0;
        for (auto v : edges[u]) {
            if (v == parent)
                continue;
            dfsGrundy(v, u);
            childGrundy ^= grundy[v];
        }

        for (auto v : edges[u]) {
            if (v == parent)
                continue;
            lazy[v] ^= childGrundy ^ grundy[v];
            // small-to-large
            if (mexTrie[v].size() > mexTrie[u].size()) {
                swap(mexTrie[u], mexTrie[v]);
                swap(lazy[u], lazy[v]);
            }
            // merge
            mexTrie[u].merge(mexTrie[v], lazy[u] ^ lazy[v]);
        }
        grundy[u] = mexTrie[u].mex(lazy[u]);
        mexTrie[u].insert(childGrundy ^ lazy[u]);
    }
};
