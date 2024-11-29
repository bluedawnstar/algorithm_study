#pragma once

// https://www.acmicpc.net/problem/13186

template <int mod = 1'000'000'007>
struct ColoringAllNecklacesMod {
    int maxN;
    int colorCount;
    vector<int> powC;

    vector<int> phi;        // all phi of [0, maxN]
    vector<int> primes;
    vector<vector<pair<int, int>>> primeFactors;

    ColoringAllNecklacesMod(int maxN, int colorCount) {
        build(maxN, colorCount);
    }

    void build(int maxN, int colorCount) {
        this->maxN = maxN;
        this->colorCount = colorCount;

        phi = phiAll(maxN);
        primeFactorization(maxN, primes, primeFactors);

        powC.resize(maxN + 1);
        powC[0] = 1;
        for (int i = 1; i <= maxN; i++)
            powC[i] = int(1ll * powC[i - 1] * colorCount % mod);
    }

    /*
       The number of ways to make unique necklaces, O(N*loglogN)
       - N : the number of beads
       - colorCount : the number of colors
       - flipAllowed : indicates whether flipping is allowed
    */
    int countFast(int N, bool flipAllowed) {
        if (N == 1)
            return colorCount;

        vector<int> factors = generateFactors(primeFactors[N]);

        int ans = 0;
        int G = N;
        for (auto f : factors) {
            ans = modAdd(ans, int(1ll * powC[f] * phi[N / f] % mod));
        }

        if (flipAllowed) {
            if (N & 1) {
                ans = modAdd(ans, modMul(N, powC[N / 2 + 1]));
                G += N;
            } else {
                ans = modAdd(ans, modMul(N / 2, powC[N / 2 + 1]));
                G += N / 2;

                ans = modAdd(ans, modMul(N / 2, powC[N / 2]));
                G += N / 2;
            }
        }

        return modDiv(ans, G);
    }

    int count(int N, bool flipAllowed) {
        if (N == 1)
            return colorCount;

        int ans = 0;
        int G = N;
        for (int i = 0; i < N; i++) {
            ans = modAdd(ans, powC[gcd(i, N)]);
        }

        if (flipAllowed) {
            if (N & 1) {
                ans = modAdd(ans, modMul(N, powC[N / 2 + 1]));
                G += N;
            } else {
                ans = modAdd(ans, modMul(N / 2, powC[N / 2 + 1]));
                G += N / 2;

                ans = modAdd(ans, modMul(N / 2, powC[N / 2]));
                G += N / 2;
            }
        }

        return modDiv(ans, G);
    }

    //---

    static int modPow(int x, int n) {
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

    static int modMul(int a, int b) {
        return int(1ll * a * b % mod);
    }

    static int modDiv(int a, int b) {
        return int(1ll * a * modPow(b, mod - 2) % mod);
    }

    static int modAdd(int a, int b) {
        a += b;
        if (a >= mod)
            a -= mod;
        return a;
    }

    static int gcd(int a, int b) {
        return (b == 0) ? a : gcd(b, a % b);
    }

    // all phi of [0, n]
    static vector<int> phiAll(int n) {
        vector<int> res(n + 1);
        iota(res.begin(), res.end(), 0);

        for (int p = 2; p <= n; p++) {
            if (res[p] == p) {
                res[p] = p - 1;
                for (int i = 2 * p; i <= n; i += p)
                    res[i] = (res[i] / p) * (p - 1);
            }
        }

        return res;
    }

    // find all prime numbers and prime factors from 1 to n
    // - primes factors of each number are reversed
    static void primeFactorization(int n, vector<int>& primes, vector<vector<pair<int, int>>>& primeFactors) {
        primes.clear();
        primeFactors = vector<vector<pair<int, int>>>(n + 1);
        if (n < 2)
            return;

        if (n >= 2) {
            primes.push_back(2);
            primeFactors[2].emplace_back(2, 1);
        }

        for (int i = 3; i <= n; i++) {
            bool isPrime = true;
            int root = int(sqrt(i));
            for (int j = 0; primes[j] <= root; j++) {
                if (i % primes[j] == 0) {
                    primeFactors[i] = primeFactors[i / primes[j]];
                    if (primeFactors[i].back().first == primes[j])
                        primeFactors[i].back().second++;
                    else
                        primeFactors[i].emplace_back(primes[j], 1);
                    isPrime = false;
                    break;
                }
            }
            if (isPrime) {
                primes.push_back(i);
                primeFactors[i].emplace_back(i, 1);
            }
        }
    }

    // primeFactors = { (factor, power), ... }
    static vector<int> generateFactors(const vector<pair<int, int>>& primeFactors) {
        if (primeFactors.empty())
            return vector<int>(1, 1);

        int sz = 1;
        for (auto& it : primeFactors)
            sz *= it.second + 1;

        vector<int> res;
        res.reserve(sz);

        res.push_back(1);

        int x = primeFactors[0].first;
        int n = primeFactors[0].second;
        int xpow = x;
        for (int i = 0; i < n; i++) {
            res.push_back(xpow);
            xpow *= x;
        }
        for (int i = 1; i < int(primeFactors.size()); i++) {
            int prevN = int(res.size());

            x = primeFactors[i].first;
            n = primeFactors[i].second;
            int xpow = x;
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < prevN; k++)
                    res.push_back(res[k] * xpow);
                xpow *= x;
            }
        }

        sort(res.begin(), res.end());

        return res;
    }
};
