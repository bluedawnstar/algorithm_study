#pragma once

// https://codeforces.com/contest/895/problem/C
// https://codeforces.com/blog/entry/68953

template <int mod>
struct SquareSubsets {
    /*
        - 1 <= maxValue <= 70
        - { A[0], A[1], ..., A[at]}

          dp[0][0] = 1
          dp[at][msk = dp[at - 1][msk] * even_count[A[at]] + dp[at - 1][msk ^ mask[at]] * odd_count[A[at]]

        - O(max(A)*2^primeCount)
    */
    static int solve(const int A[], int n, int maxValue, const int primes[], int primeCount) {
        vector<pair<int, int>> count(maxValue + 1);  // (even count, odd count)
        vector<int> mask(maxValue + 1);
        vector<vector<int>> dp(maxValue + 1, vector<int>(size_t(1) << primeCount));

        for (int i = 1; i <= maxValue; i++)
            count[i].first = 1;

        for (int i = 1; i <= n; i++) {
            int a = A[i - 1];
            int tmp = count[a].first;
            count[a].first = (count[a].first + count[a].second) % mod;
            count[a].second = (count[a].second + tmp) % mod;
        }

        // make bit-masks of all values
        for (int i = 1; i <= maxValue; i++) {
            for (int p = 0; p < primeCount; p++) {
                int cnt = 0;
                int at = i;

                while (at % primes[p] == 0) {
                    at /= primes[p];
                    cnt++;
                }

                if (cnt & 1)
                    mask[i] |= (1 << p);
            }
        }

        int maxMask = 1 << primeCount;
        dp[0][0] = 1;
        for (int x = 1; x <= maxValue; x++) {
            for (int msk = 0; msk < maxMask; msk++) {
                dp[x][msk] = (1ll * dp[x - 1][msk          ] * count[x].first
                           +  1ll * dp[x - 1][msk ^ mask[x]] * count[x].second) % mod;
            }
        }

        return (dp[maxValue][0] + mod - 1) % mod;   // subtract empty set
    }

    static int solve(const int A[], int n, int maxValue, const vector<int>& primes) {
        return solve(A, n, maxValue, primes.data(), int(primes.size()));
    }

    //---

    static int solveMax70(const int A[], int n) {
        static const int primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67 };
        return solve(A, n, 70, primes, sizeof(primes) / sizeof(primes[0]));
    }

    static int solveMax70(const vector<int>& A) {
        return solveMax70(A.data(), int(A.size()));
    }
};
