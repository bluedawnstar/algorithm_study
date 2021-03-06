#pragma once

/*
  1. problem
     https://www.codechef.com/problems/MODPARRS
     - input : { A[0], A[1], A[2], ..., A[n-1] }
     - output : the number of integer sequences { X[0], X[1], X[2], ... X[n-1] } such that

         - 0 <= X[i] < 239
         - X[i] != X[j] for each valid i != j
         - X[0] * A[0] + X[1] * A[1] + X[2] * A[2] + ... + X[n-1] * A[n-1] = 0 (mod 239)

  2. properties

        X[0] * A[0] + X[1] * A[1] + X[2] * A[2] + ... + X[n-1] * A[n-1] = 0 (mod 239)

    <=> X[0] * A[0] + X[1] * A[1] + X[2] * A[2] + ... + X[n-2] * A[n-2] = -X[n-1] * A[n-1] = k (mod 239)

      * when X[n-1] is not in { X[0], X[1], X[2], ..., X[n-2] }

          239 * (239 - 1) * (239 - 2) * ... * (239 - (n - 2))

      * but X[n-1] can be in { X[0], X[1], X[2], ..., X[n-2] }, so ...

          239 * (239 - 1) * (239 - 2) * ... * (239 - (n - 2))

          - CASE{ X[0] == X[n-1] } - CASE{ X[1] == X[n-1] } ... - CASE{ X[n-2] == X[n-1] }
          + CASE{ X[0] == X[n-1] && X[1] == X[n-1] } + ... + CASE{ X[n-3] == X[n-1] && X[n-2] == X[n-1] }
          - ...

  3. solution

     - S is a index set of unique values
     - U - S is a index set of a same value (let this same value be 't')

       ex: U = { 0, 1, 2, 3, 4, 5 }, S = { 1, 3 }, then

             dp[S] = dp[{1,3}]  ....> unique value set is { A[1], A[3], A[0] == A[2] == A[4] == A[5] }

     1) t == 0, then

        dp[S] = dp[S - {any_value_in_S}] * (239 - sizeof(S))

     2) t != 0, then

        dp[S] = P(239, size(S)) -  SUM dp[S - {S_i}]
                                  S_i is in S
*/
template <int mod = 998'244'353, int mod2 = 239>
struct DistinctValueSetCounterOfLinearCombination {
    // O(N*2^N)
    static int solve(const vector<int>& X) {
        if (X.size() == 1) {
            if (X[0] == 0)
                return mod2;
            else
                return 1;
        }
        int N = int(X.size());

        vector<int> perm(mod2 + 1); // perm[r] = mod2 * (mod2 - 1) * (mod2 - 2) * ... * (mod2 - r + 1)
        perm[0] = 1;
        for (int i = 1; i <= mod2; i++)
            perm[i] = int(1ll * perm[i - 1] * (mod2 - i + 1) % mod);

        auto sumF = [&X](int mask) {
            int res = 0;
            for (; mask; mask &= mask - 1) {
                res += X[ctz(mask)];
                if (res >= mod2)
                    res -= mod2;
            }
            return res;
        };

        // ex: 010010 = { A[1], A[4], A[0] + A[2] + A[3] + A[5] }
        vector<int> dp(size_t(1) << (N - 1));
        dp[0] = sumF((1 << N) - 1) ? 1 : mod2;
        for (int mask = 1; mask < (1 << (N - 1)); mask++) {
            int sz = popcount(mask);
            if (sumF((1 << N) - 1) == sumF(mask)) {
                // the combined element is 0
                dp[mask] = int(1ll * dp[mask & (mask - 1)] * (mod2 - sz) % mod);
            } else {
                //the combined element is non-zero
                dp[mask] = perm[sz];
                for (int m = mask; m; m &= m - 1) {
                    dp[mask] -= dp[mask ^ (m & -m)];
                    if (dp[mask] < 0)
                        dp[mask] += mod;
                }
            }
        }
        return dp[(1 << (N - 1)) - 1];
    }

private:
    static int ctz(int x) {
        if (!x)
            return 32;
#ifndef __GNUC__
        return int(_tzcnt_u32(static_cast<unsigned>(x)));
#else
        return __builtin_ctz(static_cast<unsigned>(x));
#endif
    }

    static int popcount(int x) {
#ifndef __GNUC__
        return int(__popcnt(static_cast<unsigned>(x)));
#else
        return __builtin_popcount(static_cast<unsigned>(x));
#endif
    }
};
