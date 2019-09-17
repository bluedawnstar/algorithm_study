#pragma once

// New style knapsack problem
//   https://www.codechef.com/SEPT19A/problems/PSUM

// See also "math/multinomialTheorem.h"

template <int mod = 998244353, int root = 3>
struct PowerSumOfAllCombinationsWithNTT {
    int size, invSize;
    int base, invBase;

    void init(int n) {
        size = 1;
        while (size < n)
            size <<= 1;

        invSize = modPow(size, mod - 2);

        base = modPow(root, (mod - 1) / size);
        invBase = modPow(base, mod - 2);
    }

    // cv[i] = { cost, value }, 1 <= cost
    // S = max cost sum
    // 
    // return  SUM {each combination sum}^K
    // O(N*S*K*logK)
    int solve(vector<pair<int, int>>& cv, int S, int K) {
        vector<int> fact(K + 1), factInv(K + 1);
        fact[0] = 1;
        for (int i = 1; i <= K; i++)
            fact[i] = int(1ll * fact[i - 1] * i % mod);

        factInv[K] = modPow(fact[K], mod - 2);
        for (int i = K - 1; i >= 0; i--)
            factInv[i] = int(1ll * factInv[i + 1] * (i + 1) % mod);

        vector<bool> used(S + 1);
        vector<vector<int>> dp(S + 1, vector<int>(K + 1));

        for (int i = 0; i < int(cv.size()); i++) {
            if (cv[i].first > S)
                continue;

            vector<int> baseA(size), A(size), B(size), C(size);
            baseA[0] = 1;
            A[0] = 1;
            for (int j = 1; j <= K; j++) {
                baseA[j] = int(1ll * baseA[j - 1] * cv[i].second % mod);    // baseA[j] = v^j
                A[j] = int(1ll * baseA[j] * factInv[j] % mod);              // A[j] = v^j / j!
            }
            ntt(A);

            for (int c1 = S - cv[i].first; c1 > 0; c1--) {
                if (!used[c1])
                    continue;

                for (int j = 0; j <= K; j++)
                    B[j] = int(1ll * dp[c1][j] * factInv[j] % mod);         // B[j] = dp[c][j] / j!
                for (int j = K + 1; j < size; j++)
                    B[j] = 0;
                ntt(B);

                for (int j = 0; j < size; j++)
                    C[j] = int(1ll * A[j] * B[j] % mod);
                ntt(C, true);

                int c2 = cv[i].first + c1;
                for (int j = 0; j <= K; j++)
                    dp[c2][j] = int((dp[c2][j] + 1ll * fact[j] * C[j]) % mod);  // dp[c2][j] += j! * C[j]
                used[c2] = true;
            }

            for (int j = 0; j <= K; j++)
                dp[cv[i].first][j] = int((dp[cv[i].first][j] + baseA[j]) % mod);
            used[cv[i].first] = true;
        }

        int res = 0;
        for (int i = 1; i <= S; i++) {
            res += dp[i][K];
            if (res >= mod)
                res -= mod;
        }

        return res;
    }

private:
    void ntt(vector<int>& a, bool inverse = false) {
        int base = !inverse ? this->base : this->invBase;

        for (int m = size; m >= 2; m >>= 1) {
            int mh = m >> 1;
            int w = 1;
            for (int i = 0; i < mh; i++) {
                for (int j = i; j < size; j += m) {
                    int k = j + mh;
                    int x = int((a[j] - a[k] + mod) % mod);
                    a[j] = (a[j] + a[k]) % mod;
                    a[k] = int(1ll * w * x % mod);
                }
                w = int(1ll * w * base % mod);
            }
            base = int(1ll * base * base % mod);
        }

        int i = 0;
        for (int j = 1; j < size - 1; j++) {
            for (int k = size >> 1; k > (i ^= k); k >>= 1)
                ;
            if (j > i)
                swap(a[i], a[j]);
        }

        if (inverse) {
            for (int j = 0; j < size; j++)
                a[j] = int(1ll * a[j] * invSize % mod);
        }
    }

    template <typename T>
    int modPow(T x, int n) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return int(res);
    }
};
