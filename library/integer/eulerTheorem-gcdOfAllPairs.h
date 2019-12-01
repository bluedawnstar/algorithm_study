#pragma once

// 1) GCD-sum of all pairs in [1, n]
// 
//     n   n
//    SUM SUM gcd(i,j)
//    i=1 j=1

/*
     n   n
    SUM SUM gcd(i,j)
    i=1 j=1

       n      n/g  n/g        n       n/g            i          n      n/g
    = SUM g * SUM  SUM 1  =  SUM g *  SUM { 1 + 2 * SUM 1 } =  SUM g * SUM { 2 * phi(i) - phi(1) }
      g=1     i=1  j=1       g=1      i=1           j=1        g=1     i=1
              gcd(i,j)=1                     gcd(i,j)=1

       n      n/i
    = SUM i * SUM { 2 * phi(j) - phi(1) }
      i=1     j=1
*/
struct GcdSumOfAllPairs {
    vector<int> phi;
    vector<long long> dp;

    // O(NloglogN)
    void build(int maxN) {
        // phi
        phi.resize(maxN + 1);
        iota(phi.begin(), phi.end(), 0);

        for (int p = 2; p <= maxN; p++) {
            if (phi[p] == p) {
                phi[p] = p - 1;
                for (int i = 2 * p; i <= maxN; i += p)
                    phi[i] = (phi[i] / p) * (p - 1);
            }
        }

        //                   i
        // dp[i] = phi(1) + SUM 2 * phi(j)
        //                  j=2
        dp = vector<long long>(maxN + 1);
        dp[1] = phi[1];
        for (int i = 2; i <= maxN; i++)
            dp[i] = dp[i - 1] + 2ll * phi[i];
    }

    // O(n)
    long long sum(int n) const {
        long long res = 0;
        for (int i = 1; i <= n; i++)
            res += 1ll * i * dp[n / i];
        return res;
    }
};

template <typename T, T mod = 1000000007>
struct GcdSumOfAllPairsMod {
    vector<int> phi;
    vector<T>   dp;

    // O(NloglogN)
    void build(int maxN) {
        // phi
        phi.resize(maxN + 1);
        iota(phi.begin(), phi.end(), 0);

        for (int p = 2; p <= maxN; p++) {
            if (phi[p] == p) {
                phi[p] = p - 1;
                for (int i = 2 * p; i <= maxN; i += p)
                    phi[i] = (phi[i] / p) * (p - 1);
            }
        }

        //                   i
        // dp[i] = phi(1) + SUM 2 * phi(j)
        //                  j=2
        dp = vector<T>(maxN + 1);
        dp[1] = phi[1];
        for (int i = 2; i <= maxN; i++)
            dp[i] = T((dp[i - 1] + 2ll * phi[i]) % mod);
    }

    // O(n)
    T sum(int n) const {
        T res = 0;
        for (int i = 1; i <= n; i++)
            res = T((res + 1ll * i * dp[n / i]) % mod);
        return res;
    }
};
