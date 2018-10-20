#pragma once

// https://www.codechef.com/OCT18B/problems/BBRICKS

/*
    Number of cases in which K blocks that are not adjacent in a 2xN block are selected
    (Not all blocks are adjacent)

        <Bad>          <Good>
    +---+---+---+   +---+---+---+
    | O | O |   |   | O |   |   |
    +---+---+---+   +---+---+---+
    |   |   |   |   |   | O |   |
    +---+---+---+   +---+---+---+
*/
struct SelectBlock2xN {
    /** DP Method
        f(N, K) = f(N - 1, K) + f(N - 1, K - 1) + f(N - 2, K - 1)
          - f(N, N) = 2
          - f(N, N+i) = 0  (i > 0)
          - f(N, 0) = 1
          - f(0, K) = 0    (K > 0)
    */
    // select K blocks of 2xN blocks, O(NK)
    static long long countWays(int N, int K) {
        if (N < K)
            return 0;
        else if (N == K)
            return 2ll;
        else if (K <= 0)
            return 1ll;
        else if (K == 1)
            return 2ll * N;

        vector<vector<long long>> dp(N + 1, vector<long long>(K + 1));
        dp[0][0] = 1;
        for (int i = 1; i <= N && i <= K; i++)
            dp[i][i] = 2;

        for (int i = 1; i <= N; i++) {
            dp[i][0] = 1;
            for (int j = 1; j < i && j <= K; j++) {
                dp[i][j] = 1ll * dp[i - 2][j - 1] + dp[i - 1][j - 1] + dp[i - 1][j];
            }
        }

        return dp[N][K];
    }

    // select K blocks of 2xN blocks, O(NK)
    static long long countWays(int N, int K, int MOD) {
        if (N < K)
            return 0;
        else if (N == K)
            return 2ll;
        else if (K <= 0)
            return 1ll;
        else if (K == 1)
            return 2ll * N % MOD;

        vector<vector<long long>> dp(N + 1, vector<long long>(K + 1));
        dp[0][0] = 1;
        for (int i = 1; i <= N && i <= K; i++)
            dp[i][i] = 2;

        for (int i = 1; i <= N; i++) {
            dp[i][0] = 1;
            for (int j = 1; j < i && j <= K; j++) {
                dp[i][j] = (1ll * dp[i - 2][j - 1] + dp[i - 1][j - 1] + dp[i - 1][j]) % MOD;
            }
        }

        return dp[N][K];
    }

    //---

    /** Combinatorics
        https://cs.uwaterloo.ca/journals/JIS/VOL17/Janjic/janjic45.pdf, P.14 ~ 15
        http://oeis.org/A001105, https://oeis.org/A035597, https://oeis.org/A035598, https://oeis.org/A035599, http://oeis.org/A035600,
        https://oeis.org/A035601, https://oeis.org/A035602, http://oeis.org/A035603, http://oeis.org/A035604, http://oeis.org/A035605

                  N-K+1
        f(N, K) =  SUM { 2^i * C(N - K + 1, i) * C(K - 1, i - 1) },   (0 <= i <= min(K, N - K + 1))
                   i=1

         <All of the following conditions must be met.>
              C(N - K + 1, i) --> 0 <=   i   <= N - K + 1
              C(K - 1, i - 1) --> 0 <= i - 1 <= K - 1

              ==>   0 <= i <= min(K, N - K + 1)
    */

    // select K blocks of 2xN block (optimal method)
    static long long countWaysFast(int N, int K) {
        if (N < K)
            return 0;
        else if (N == K)
            return 2;
        else if (K <= 0)
            return 1;
        else if (K == 1)
            return 2ll * N;

        long long res = 0ll;

        long long pow2 = 1ll;
        long long comb1 = 1ll;
        long long comb2 = 1ll;
        for (int i = 1, c = N - K + 1, ck = K; i <= N - K + 1 && i <= K; i++, c--, ck--) {
            pow2 *= 2;
            comb1 = comb1 * c / i;
            if (i > 1)
                comb2 = comb2 * ck / (i == 2 ? 1 : i - 1);
            res += pow2 * comb1 * comb2;
        }

        return res;
    }

    // select K blocks of 2xN block (optimal method)
    static long long countWaysFast(int N, int K, int MOD) {
        if (N < K)
            return 0;
        else if (N == K)
            return 2;
        else if (K <= 0)
            return 1;
        else if (K == 1)
            return 2ll * N % MOD;
        else if (K == 2)
            return 2ll * modPowPrime(N - 1, 2, MOD) % MOD;

        long long res = 0ll;

        long long pow2 = 1;
        long long comb1 = 1;
        long long comb2 = 1;
        for (int i = 1, c = N - K + 1, ck = K; i <= N - K + 1 && i <= K; i++, c--, ck--) {
            pow2 = (pow2 * 2) % MOD;
            if (i == 1)
                comb1 = comb1 * c % MOD;
            else
                comb1 = comb1 * c % MOD * modInvPrime(i, MOD) % MOD;
            if (i > 1)
                comb2 = comb2 * ck % MOD * (i == 2 ? 1 : modInvPrime(i - 1, MOD)) % MOD;
            res = (res + pow2 * comb1 % MOD * comb2 % MOD) % MOD;
        }

        return res;
    }

private:
    // iterative version
    static int modPow(int x, int n, int M) {
        if (n == 0)
            return 1;

        long long t = x % M;
        int res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = int(res * t % M);
            t = t * t % M;
        }
        return res;
    }

    // M is a prime number
    static int modPowPrime(int x, int n, int M) {
        return modPow(x, n % (M - 1), M);
    }

    // a and M are coprime
    static int modInvIter(int a, int M) {
        int b = M;
        int y = 0, x = 1;

        if (M == 1)
            return 0;

        while (a > 1 && b != 0) {
            int q = a / b;

            int t = b;
            b = a % b;
            a = t;

            t = y;
            y = x - q * y;
            x = t;
        }

        return (x % M + M) % M;
    }

    static int modInvPrime(int a, int M) {
        return modPow(a, M - 2, M);
    }
};
