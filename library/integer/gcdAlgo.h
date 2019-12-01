#pragma once

// the number of combinations:
//      gcd(a,b,c,d,...) = 1, 1 <= a < b < c < d < ... <= N
template <int mod = 1000000007>
struct Gcd1CombCounter {
    int maxN;
    int K;
    vector<int> values;         // values[i] is the number of combinations in gcd(a,b,c,...) = 1, 1 <= a < b < c < ... <= i

    vector<int> factorial;      // factorial
    vector<int> factInverse;    // inverse factorial
    vector<int> inverse;

    vector<int> minFactors;     // minimum prime factors of [0, n]

    Gcd1CombCounter() {
    }

    Gcd1CombCounter(int maxN, int K) {
        build(maxN, K);
    }

    // for gcd(x[0],x[1],x[2],...,x[k-1]) = 1,  1 <= x[0] < x[1] < x[2] < ... < x[k - 1] <= N
    // O(N*logN)?
    void build(int maxN, int K) {
        this->maxN = maxN;
        this->K = K;

        buildFactorial();
        buildMinFactors();

        values.resize(maxN + 1);
        for (int i = K; i <= maxN; i++) {
            long long sum = 0;
            vector<pair<int,int>> pf = getPrimeFactors(i);

            int pfn = int(pf.size());
            for (int j = 1; j < (1 << pfn); j++) {
                int bit = 0;
                int x = 1;
                for (int t = j; t; t &= t - 1) {
                    x *= pf[ctz(t)].first;
                    bit++;
                }

                if (bit & 1)
                    sum = (sum + comb(i / x - 1, K - 1)) % mod;
                else
                    sum = (sum - comb(i / x - 1, K - 1) + mod) % mod;
            }

            values[i] = int((0ll + values[i - 1] + (comb(i - 1, K - 1) - sum + mod)) % mod);
        }
    }

    // the number of combinations where gcd(x[0],x[1],x[2],...,x[k-1]) = 1,
    //   1 <= x[0] < x[1] < x[2] < ... < x[k - 1] <= n
    int count(int n) const {
        return values[n];
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

    vector<pair<int,int>> getPrimeFactors(int x) const {
        vector<pair<int,int>> res;
        while (x > 1) {
            if (!res.empty() && minFactors[x] == res.back().first)
                res.back().second++;
            else
                res.emplace_back(minFactors[x], 1);
            x /= minFactors[x];
        }

        return res;
    }

private:
    inline int ctz(unsigned x) const {
#ifndef __GNUC__
        return int(_tzcnt_u32(x));
#else
        return __builtin_ctz(x);
#endif
    }

    inline int ctz(int x) const {
        return ctz((unsigned int)x);
    }

    //---

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

    //---

    void buildMinFactors() {
        minFactors.resize(maxN + 1);
        minFactors[0] = -1;
        minFactors[1] = -1;

        for (int i = 2; i <= maxN; i++)
            minFactors[i] = i;

        if (maxN >= 4) {
            for (int j = 2 * 2; j <= maxN; j += 2)
                minFactors[j] = 2;
        }

        int root = int(sqrt(maxN));
        for (int i = 3; i <= root; i += 2) {
            if (minFactors[i] == i) {
                for (int j = i * i; j >= 0 && j <= maxN; j += i) {
                    if (minFactors[j] == j)
                        minFactors[j] = i;
                }
            }
        }
    }
};
