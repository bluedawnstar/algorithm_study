#pragma once

/*
    https://www.codechef.com/problems/DIVPERMS

    result[k] = 
      The number of permutations of 1 ~ N where the number of adjacent numbers P[i] divided by P[i-1] is k
*/
template <int mod = 998'244'353>
struct PermutationCounterByDivisibleAdjacentPairCount {
    // return permutation_count[divisible_adjacent_pair_count], O(N^2 * 2^(N/2))
    static vector<int> solve(const vector<bool>& possibleScale, int N) {
        vector<int> res(N);
        if (N == 1) {
            res[0] = 1;
            return res;
        }

        int halfN = N / 2;
        vector<int> dp[2];
        dp[0].resize(size_t(1) << halfN);
        dp[1].resize(size_t(1) << halfN);

        int from = 0, to = 1;
        dp[0][0] = 1;
        for (int i = 2; i <= N; i++) {
            memset(dp[to].data(), 0, sizeof(dp[to][0]) * dp[to].size());

            /*
                1) j == 0 || j >= i / 2
                    dp[N][divisible_number_mask] += dp[N - 1][divisible_number_mask]
                2) i % j == 0 && possibleScale[i / j]
                    dp[N][divisible_number_mask | (1 << (j - 1))] += dp[N - 1][divisible_number_mask]
                3) i % j != 0 && !possibleScale[i / j]
                    dp[N][divisible_number_mask & ~(1 << (j - 1))] += dp[N - 1][divisible_number_mask]

                if P[i] % P[i - 1] == 0 then
                  (P[i - 1] - 1)th bit of divisible_number_mask is ON
            */
            int half = i / 2;
            for (int mask = 0; mask < (1 << half); mask++) {
                // insert number i after number j ("j = 0" means to insert at front)
                for (int j = 0; j < i; j++) {
                    int mask2 = mask;
                    if (0 < j && j <= half) {
                        if (i % j == 0 && possibleScale[i / j])
                            mask2 |= (1 << (j - 1));
                        else
                            mask2 &= ~(1 << (j - 1));
                    }
                    dp[to][mask2] += dp[from][mask];
                    if (dp[to][mask2] >= mod)
                        dp[to][mask2] -= mod;
                }
            }
            swap(from, to);
        }

        // make answer
        vector<int> bitCount(size_t(1) << halfN);
        res[0] = dp[from][0];
        for (int i = 1; i < (1 << halfN); i++) {
            int bn = bitCount[i & (i - 1)] + 1;
            bitCount[i] = bn;
            res[bn] += dp[from][i];
            if (res[bn] >= mod)
                res[bn] -= mod;
        }

        return res;
    }
};
