#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

/*
  Count pairs to make a value with bitwise operation in an array

  - See "../polynomial/walshHadamard.h" for more general cases
*/
struct PairCounterWithBitwiseOperation {
    // https://codeforces.com/blog/entry/20174
    // O(MAXX*logMAXX)
    // return the number of pairs with bitwise AND zeros in an array
    static long long countPairsWithAndZeroDP(const vector<int>& A) {
        int maxX = *max_element(A.begin(), A.end());
        int N = 1;
        while (N <= maxX)
            N <<= 1;

        vector<int> count(N);
        for (auto x : A)
            ++count[x];

        long long ans = count[0];
        for (int bit = 1; bit < N; bit <<= 1) {
            for (int i = 0; i < N; i++) {
                if ((i & bit) == 0)
                    count[i] += count[i ^ bit];
            }
        }
        for (int i = 0; i < N; i++) {
            if (popcount(i) & 1)
                ans -= (1ll * count[i] * (count[i] - 1));
            else
                ans += (1ll * count[i] * (count[i] - 1));
        }
        return ans;
    }

    // O(M*logM), M = MAXX/2^bitcount(target)
    // return the number of pairs with bitwise AND target in an array
    static long long countPairsWithAndDP(const vector<int>& A, int target) {
        int maxX = max(target, *max_element(A.begin(), A.end()));
        int N = 1;
        while (N <= maxX)
            N <<= 1;

        vector<int> count(N);
        for (auto x : A)
            ++count[x];

        int targetBitCount = popcount(target);

        long long ans = count[target];
#if 0
        int mask = (N - 1) ^ target;
        for (int bit = (mask & -mask); bit; bit = (mask & -mask)) {
            mask &= mask - 1;
#else
        for (int bit = 1; bit < N; bit <<= 1) {
            if (target & bit)
                continue;
#endif
            for (int i = target; i < N; i = (i + 1) | target) {
                if ((i & bit) == 0)
                    count[i] += count[i ^ bit];
            }
        }
        for (int i = target; i < N; i = (i + 1) | target) {
            if ((popcount(i) - targetBitCount) & 1)
                ans -= (1ll * count[i] * (count[i] - 1));
            else
                ans += (1ll * count[i] * (count[i] - 1));
        }
        return ans;
    }

    //---

    // O(N^2)
    // return the number of pairs with bitwise OR target in an array
    static long long countPairsWithOrDP(const vector<int>& A, int target) {
        long long res = 0;

        int N = int(A.size());
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if ((A[i] | A[j]) == target) {
                    res++;
                }
            }
        }

        return res;
    }

    //---

    // O(N)
    // return the number of pairs with bitwise XOR target in an array
    static long long countPairsWithXOR(const vector<int>& A, int target) {
        unordered_map<int, int> M;

        long long res = (target == 0) ? int(A.size()) : 0;
        for (int i = 0; i < int(A.size()); i++) {
            auto it = M.find(A[i] ^ target);
            if (it != M.end())
                res += it->second * 2;
            ++M[A[i]];
        }

        return res;
    }

private:
    // pop count
    static int popcount(int x) {
#ifndef __GNUC__
        return int(__popcnt(static_cast<unsigned>(x)));
#else
        return __builtin_popcount(static_cast<unsigned>(x));
#endif
    }
};
