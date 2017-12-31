#pragma once

// Undirected graph
struct MaxClique {
    // O(3^(V/3))
    // returns maximum weight sum
    static int doBronKerbosch(const vector<unsigned long long>& G, unsigned long long cur,
                              unsigned long long allowed, unsigned long long forbidden, const vector<int>& weights) {
        int N = (int)G.size();

        if (allowed == 0 && forbidden == 0) {
            int res = 0;
            for (int u = ctz(cur); u < N; u += ctz(cur >> (u + 1)) + 1)
                res += weights[u];
            return res;
        }
        if (allowed == 0)
            return -1;

        int res = -1;
        int pivot = ctz(allowed | forbidden);
        unsigned long long z = allowed & ~G[pivot];
        for (int u = ctz(z); u < N; u += ctz(z >> (u + 1)) + 1) {
            res = max(res, doBronKerbosch(G, cur | (1ull << u), allowed & G[u], forbidden & G[u], weights));
            allowed ^= 1ull << u;
            forbidden |= 1ull << u;
        }

        return res;
    }

    static int doBronKerbosch(const vector<unsigned long long>& G, const vector<int>& weights) {
        return doBronKerbosch(G, 0ull, (1ull << (int)G.size()) - 1ull, 0ull, weights);
    }

    static int doBronKerbosch(const vector<vector<int>>& edges, const vector<int>& weights) {
        int N = (int)edges.size();

        vector<unsigned long long> G(N);
        for (int u = 0; u < N; u++) {
            for (int v : edges[u]) {
                G[u] |= (1ull << v);
                G[v] |= (1ull << u);
            }
        }
        return doBronKerbosch(G, 0ull, (1ull << N) - 1ull, 0ull, weights);
    }

private:
    static int clz(unsigned x) {
#ifndef __GNUC__
        return (int)__lzcnt(x);
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
};
