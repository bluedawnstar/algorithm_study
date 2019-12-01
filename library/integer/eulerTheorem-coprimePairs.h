#pragma once

// 1) Multiplication-sum of all coprime pairs in [1, n]

/*
     n   n           n         i         n
    SUM SUM i * j = SUM i * 2 SUM j   = SUM i * 2 * { 1/2 * i * phi(i) }
    i=1 j=1         i=1       j=1       i=1                                  
      gcd(i,j)=1            gcd(i,j)=1

       n
    = SUM i * i * phi(i)
      i=1
*/
struct CoprimePairMultSum {
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

        //          i
        // dp[i] = SUM j * j * phi(j)
        //         j=1
        dp = vector<long long>(maxN + 1);
        for (int i = 1; i <= maxN; i++)
            dp[i] = (dp[i - 1] + 1ll * i * i * phi[i]);
    }

    // O(1)
    long long sum(int n) const {
        return dp[n];
    }
};

template <typename T, T mod = 1000000007>
struct CoprimePairMultSumMod {
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

        //          i
        // dp[i] = SUM j * j * phi(j)
        //         j=1
        dp = vector<T>(maxN + 1);
        for (int i = 1; i <= maxN; i++)
            dp[i] = T((dp[i - 1] + 1ll * i * i * phi[i]) % mod);
    }

    // O(1)
    T sum(int n) const {
        return dp[n];
    }
};


// 2) Special multiplication-sum of all pairs in [1, n]
// 
//     n   n
//    SUM SUM i/gcd(i,j) * j/gcd(i,j)
//    i=1 j=1

/*
https://www.hackerearth.com/problem/algorithm/delta-a6f44e8a/

     n   n
    SUM SUM i/gcd(i,j) * j/gcd(i,j)
    i=1 j=1

       n  n/g n/g
    = SUM SUM SUM i * j            (=>  (i*1)/1 * (j*1)/1 + (i*2)/2 * (j*2)/2 + (i*3)/3 * (j*3)/3 + ...)
      g=1 i=1 j=1
            gcd(i,j) = 1

       n  n/g        i              n  n/g                                 n  n/g
    = SUM SUM i * 2 SUM j       =  SUM SUM i * 2 * { 1/2 * i * phi(i) } = SUM SUM i * i * phi(i)
      g=1 i=1       j=1            g=1 i=1                                g=1 i=1
                  gcd(i,j) = 1

       n  n/i
    = SUM SUM j * j * phi(j)
      i=1 j=1
*/
struct CoprimeMultSumOfAllPairs {
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

        //          i
        // dp[i] = SUM j * j * phi(j)
        //         j=1
        dp = vector<long long>(maxN + 1);
        for (int i = 1; i <= maxN; i++)
            dp[i] = (dp[i - 1] + 1ll * i * i * phi[i]);
    }

    // O(n)
    long long sum(int n) const {
        long long res = 0;
        for (int i = 1; i <= n; i++)
            res += dp[n / i];
        return res;
    }
};

template <typename T, T mod = 1000000007>
struct CoprimeMultSumOfAllPairsMod {
    vector<int> phi;
    vector<T> dp;

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

        //          i
        // dp[i] = SUM j * j * phi(j)
        //         j=1
        dp = vector<T>(maxN + 1);
        for (int i = 1; i <= maxN; i++)
            dp[i] = T((dp[i - 1] + 1ll * i * i * phi[i]) % mod);
    }

    // O(n)
    T sum(int n) const {
        T res = 0;
        for (int i = 1; i <= n; i++)
            res = (res + dp[n / i]) % mod;
        return res;
    }
};
