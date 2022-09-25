#pragma once

struct SmallestPrimeFactorsWithLinearSieve {
    vector<int> spf;    // minimum prime factors of [0, n]
    vector<int> primes;

    SmallestPrimeFactorsWithLinearSieve() {
    }

    explicit SmallestPrimeFactorsWithLinearSieve(int n) {
        build(n);
    }

    // O(n)
    void build(int n) {
        primes.clear();
        spf.resize(n + 1);
        spf[0] = -1;
        spf[1] = -1;

#if 0
        // from https://cp-algorithms.com/algebra/prime-sieve-linear.html
        for (int i = 2; i <= n; i++) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int j = 0, pn = int(primes.size()); j < pn && primes[j] <= spf[i] && (i * primes[j]) <= n; j++)
                spf[i * primes[j]] = primes[j];
        }
#else
        // modified code

        if (n >= 2) {
            primes.push_back(2);
            for (int j = 2; j <= n; j += 2)
                spf[j] = 2;
        }
        for (int i = 3; i <= n; i += 2) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int j = 1, pn = int(primes.size()); j < pn && primes[j] <= spf[i] && (i * primes[j]) <= n; j++)
                spf[i * primes[j]] = primes[j];
        }
#endif
    }

    vector<pair<int, int>> getPrimeFactors(int x) {
        vector<pair<int, int>> res;
        while (x > 1) {
            if (!res.empty() && spf[x] == res.back().first)
                res.back().second++;
            else
                res.emplace_back(spf[x], 1);
            x /= spf[x];
        }

        return res;
    }
};
