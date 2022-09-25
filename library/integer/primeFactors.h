#pragma once

#include "primeFactorsSmallest.h"

// prime factors of a integer number
struct PrimeFactors {
    vector<pair<int, int>>  primeFactors;

    PrimeFactors() {
    }

    explicit PrimeFactors(int x) {
        build(x);
    }

    PrimeFactors(const SmallestPrimeFactors& spf, int x) {
        build(spf, x);
    }

    void build(int x) {
        primeFactors.clear();

        int root = int(sqrt(x));
        if (root >= 2) {
            int cnt = 0;
            while (x % 2 == 0) {
                x /= 2;
                cnt++;
            }
            if (cnt > 0)
                primeFactors.push_back(make_pair(2, cnt));
        }

        for (int i = 3; i <= root; i += 2) {
            int cnt = 0;
            while (x % i == 0) {
                x /= i;
                cnt++;
            }
            if (cnt > 0)
                primeFactors.push_back(make_pair(i, cnt));
        }

        if (x > 1)
            primeFactors.push_back(make_pair(x, 1));
    }

    void build(const SmallestPrimeFactors& spf, int x) {
        primeFactors.clear();

        while (x > 1) {
            if (!primeFactors.empty() && primeFactors.back().first == spf.spf[x])
                primeFactors.back().second++;
            else
                primeFactors.push_back(make_pair(spf.spf[x], 1));
            x /= spf.spf[x];
        }
    }

    // return all factors
    vector<int> getFactors() const {
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
        for (int i = 0, xpow = x; i < n; i++) {
            res.push_back(xpow);
            xpow *= x;
        }
        for (int i = 1; i < int(primeFactors.size()); i++) {
            int prevN = int(res.size());

            x = primeFactors[i].first;
            n = primeFactors[i].second;
            for (int j = 0, xpow = x; j < n; j++) {
                for (int k = 0; k < prevN; k++)
                    res.push_back(res[k] * xpow);
                xpow *= x;
            }
        }

        sort(res.begin(), res.end());

        return res;
    }

    // a^メ * b^モ * c^ャ  ==>  (メ + 1) * (モ + 1) * (ャ + 1)
    int getFactorCount() const {
        int res = 1;
        for (auto& it : primeFactors)
            res *= it.second + 1;
        return res;
    }

    //see the eulerTheorem.h
    int phi(int n) const {
        int res = n;

        for (auto& p : primeFactors)
            res -= res / p.first;

        return res;
    }

    bool isCoprime(const PrimeFactors& rhs) const {
        int N = int(primeFactors.size());
        int M = int(rhs.primeFactors.size());
        for (int i = 0, j = 0; i < N && j < M; ) {
            if (primeFactors[i].first == rhs.primeFactors[j].first)
                return false;
            else if (primeFactors[i].first < rhs.primeFactors[j].first)
                i++;
            else
                j++;
        }
        return true;
    }
};

// find all prime numbers and prime factors from 1 to n
inline void buildAll(int n, vector<int>& primes, vector<PrimeFactors>& primeFactors) {
    primes.clear();
    primeFactors = vector<PrimeFactors>(n + 1);
    if (n < 2)
        return;

    if (n >= 2) {
        primes.push_back(2);
        primeFactors[2].primeFactors.push_back(make_pair(2, 1));
    }

    for (int i = 3; i <= n; i++) {
        bool isPrime = true;
        int root = int(sqrt(i));
        for (int j = 0; primes[j] <= root; j++) {
            if (i % primes[j] == 0) {
                primeFactors[i] = primeFactors[i / primes[j]];
                if (primeFactors[i].primeFactors[0].first == primes[j])
                    primeFactors[i].primeFactors[0].second++;
                else
                    primeFactors[i].primeFactors.insert(primeFactors[i].primeFactors.begin(), make_pair(primes[j], 1));
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            primes.push_back(i);
            primeFactors[i].primeFactors.push_back(make_pair(i, 1));
        }
    }
}
