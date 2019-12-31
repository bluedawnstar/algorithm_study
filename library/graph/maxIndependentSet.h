#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

// https://en.wikipedia.org/wiki/Independent_set_(graph_theory)

// Undirected graph
struct MaxIndependentSet {
    static int getMaxWeightedSum(vector<unsigned long long>& G, const vector<int>& weights) {
        for (int u = 0; u < int(G.size()); u++)
            G[u] |= (1ull << u);

        return getMaxWeightedSum(G, (1ull << int(G.size())) - 1ull, weights);
    }

    static int getMaxWeightedSum(const vector<vector<int>>& edges, const vector<int>& weights) {
        int N = int(edges.size());

        vector<unsigned long long> G(N);
        for (int u = 0; u < N; u++) {
            G[u] |= (1ull << u);
            for (int v : edges[u]) {
                G[u] |= (1ull << v);
                G[v] |= (1ull << u);
            }
        }
        return getMaxWeightedSum(G, (1ull << N) - 1ull, weights);
    }

    //--- bipartite graph -------------------------
    // See "bipartiteGraph.h" and "bipartiteGraph.cpp"

private:
    // O(3^(V/3))
    // returns maximum weighted independent set
    static int getMaxWeightedSum(const vector<unsigned long long>& G, unsigned long long unused, const vector<int>& weights) {
        if (!unused)
            return 0;

        int N = int(G.size());

        int v = -1;
        for (int u = ctz(unused); u < N; u += ctz(unused >> (u + 1)) + 1) {
            if (v == -1 || popcount(G[v] & unused) > popcount(G[u] & unused))
                v = u;
        }

        int res = 0;
        unsigned long long nv = G[v] & unused;
        for (int y = ctz(nv); y < N; y += ctz(nv >> (y + 1)) + 1)
            res = max(res, weights[y] + getMaxWeightedSum(G, unused & ~G[y], weights));

        return res;
    }


    static int clz(unsigned x) {
#ifndef __GNUC__
        return int(__lzcnt(x));
#else
        return __builtin_clz(x);
#endif
    }

    static int clz(unsigned long long x) {
        if ((x >> 32) != 0)
            return clz(unsigned(x >> 32));
        else
            return 32 + clz(unsigned(x));
    }

    static int ctz(unsigned long long x) {
        if (!x)
            return 64;

        long long xx = (long long)x;
        return 63 - clz((unsigned long long)(xx & -xx));
    }

    // pop count
    static int popcount(unsigned x) {
#ifndef __GNUC__
        return int(__popcnt(x));
        /*
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x + (x >> 4)) & 0x0F0F0F0F;
        x = x + (x >> 8);
        x = x + (x >> 16);
        return x & 0x0000003F;
        */
#else
        return __builtin_popcount(x);
#endif
    }

    static int popcount(unsigned long long x) {
#ifndef __GNUC__
        return int(__popcnt(unsigned(x))) + int(__popcnt(unsigned(x >> 32)));
#else
        return __builtin_popcountll(x);
#endif
    }
};
