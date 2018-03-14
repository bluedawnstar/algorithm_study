#pragma once

#include "prefixFunction.h"

// PrefixFunction's Sparse Table => pi[pi[pi[...[pi[i] - 1]... - 1] - 1] - 1]
struct PrefixFunctionSparseTable {
    int                 N;          // the number of vertex
    int                 logN;       // 
    vector<vector<int>> P;          // mP[0][n] points to the parent
                                    // parent & acestors

    PrefixFunctionSparseTable(const PrefixFunction& pf) {
        build(pf);
    }

    // the length of the longest proper prefix of the substring S[0..pos]
    //   which is also a suffix of this substring
    int getPI(int pos) const {
        return P[0][pos] + 1;
    }

    // f(i, 0) = pi[i], f(i, j) = pi[pi[...[pi[i] - 1]... - 1] - 1], (pi[0] = 0)
    // getPI(i) == getPI(i, 0)
    int getPI(int pos, int dist) const {
        for (int i = 0; dist > 0; i++) {
            if (dist & 1) {
                pos = P[i][pos];
                if (pos < 0)
                    return 0;
            }
            dist >>= 1;
        }

        return P[0][pos] + 1;
    }

    // the length of the shortest proper prefix of the substring S[0..pos]
    //   which is also a suffix of this substring
    int getMinPrefixSuffix(int pos) const {
        for (int i = logN - 1; i >= 0 && pos >= 0; i--) {
            if (P[i][pos] >= 0)
                pos = P[i][pos];
        }

        return pos + 1;
    }

    // the length of the shortest proper prefix of the substring S[0..pos]
    //   which is also a suffix of this substring (length <= k)
    int getMaxPrefixSuffixLessThanEqualK(int pos, int k) const {
        for (int i = logN - 1; i >= 0; i--) {
            if (P[i][pos] + 1 > k) {
                pos = P[i][pos];
                if (pos < 0)
                    return 0;
            }
        }

        return P[0][pos] + 1;
    }

private:
    void build(const PrefixFunction& pf) {
        N = (int)pf.p.length();
        logN = (int)log2(N) + 1;

        P = vector<vector<int>>(logN, vector<int>(N));

        for (int i = 0; i < N; i++)
            P[0][i] = pf.pi[i] - 1;

        for (int i = 1; i < logN; i++) {
            for (int j = 0; j < N; j++) {
                int pp = P[i - 1][j];
                P[i][j] = pp < 0 ? pp : P[i - 1][pp];
            }
        }
    }
};
