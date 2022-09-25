#pragma once

#include "primeFactors.h"
#include "primeFactorsSmallestLinearSieve.h"

// find all prime numbers and prime factors from 1 to n
// - primes factors of each number are reversed
inline void primeFactorization(int n, vector<int>& primes, vector<vector<pair<int, int>>>& primeFactors) {
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

// find all prime numbers and min prime factors from 1 to n, O(n*loglogn)
inline void primeFactorization(int n, vector<int>& primes, vector<int>& minPrimeFactors) {
    primes.clear();
    minPrimeFactors = vector<int>(n + 1);

    if (n >= 2) {
        primes.push_back(2);
        minPrimeFactors[2] = 2;
        for (int j = 2 * 2; j <= n; j += 2)
            minPrimeFactors[j] = 2;
    }

    for (int i = 3; i <= n; i += 2) {
        if (!minPrimeFactors[i]) {
            primes.push_back(i);
            minPrimeFactors[i] = i;

            if (1ll * i * i <= n) {
                for (int j = i * i; j <= n; j += i) {
                    if (!minPrimeFactors[j])
                        minPrimeFactors[j] = i;
                }
            }
        }
    }
}

// primeFactors = { (factor, power), ... }
inline vector<int> generateFactors(const vector<pair<int, int>>& primeFactors) {
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
