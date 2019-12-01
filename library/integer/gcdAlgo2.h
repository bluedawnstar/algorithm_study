#pragma once

// the number of combinations:
//      gcd(a,b,c,d,...) = i, 1 <= a < b < c < d < ... <= N
template <int mod = 1000000007>
struct GcdCombCounter {
    int maxN;

    vector<int> factorial;      // factorial
    vector<int> factInverse;    // inverse factorial
    vector<int> inverse;

    GcdCombCounter() {
    }

    explicit GcdCombCounter(int maxN) {
        prepare(maxN);
    }

    // O(N)
    void prepare(int maxN) {
        this->maxN = maxN;
        buildFactorial();
    }

    //---

    vector<int> dp;     // dp[i] is the number of combinations in gcd(a,b,c,...) = i, 1 <= a < b < c < ... <= i

    // dp[g] = the number of combinations where gcd(x[0],x[1],x[2],...,x[k-1]) = g,
    //           1 <= x[0] < x[1] < x[2] < ... < x[k - 1] <= N
    //
    // dp(i) = C(M, K) - SUM dp(z)   , M is the number of multiples of i in [1, N]
    //                   i|z,z>i
    // 
    // O(N*logN)
    void build(int N, int K) {
        dp.resize(N + 1);
        for (int i = N; i >= 1; i--) {
            int sum = comb(N / i, K);
            for (int j = 2 * i; j <= N; j += i) {
                sum -= dp[j];
                if (sum < 0)
                    sum += mod;
            }
            dp[i] = sum;
        }
    }

    // the number of combinations where gcd(x[0],x[1],x[2],...,x[k-1]) = g,
    //   1 <= x[0] < x[1] < x[2] < ... < x[k - 1] <= N
    int count(int g) const {
        return dp[g];
    }

    //---

    int comb(int n, int r) const {
        if (n < r)
            return 0;

        if (n == 0 || r == 0 || n == r)
            return 1;

        if (n - r < r)
            r = n - r;

        return int(1ll * factorial[n] * factInverse[n - r] % mod * factInverse[r] % mod);
    }

private:
    void buildFactorial() {
        factorial.resize(maxN + 1);
        factInverse.resize(maxN + 1);
        inverse.resize(maxN + 1);

        inverse[0] = 0;
        inverse[1] = 1;
        for (int i = 2; i <= maxN; i++)
            inverse[i] = int((mod - 1ll * (mod / i) * inverse[mod % i] % mod) % mod);

        factorial[0] = 1;
        factInverse[0] = 1;

        factorial[1] = 1;
        factInverse[1] = 1;

        for (int i = 2; i <= maxN; i++) {
            factorial[i] = int(1ll * factorial[i - 1] * i % mod);
            factInverse[i] = int(1ll * factInverse[i - 1] * inverse[i] % mod);
        }
    }
};
