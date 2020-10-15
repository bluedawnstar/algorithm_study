#pragma once

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

//--------- Prime Factors -------------------------------------------------

// all minimum prime factors of [0, n]
struct MinFactors {
    vector<int> minFactors; // minimum prime factors of [0, n]

    MinFactors() {
    }

    explicit MinFactors(int n) {
        build(n);
    }

    void build(int n) {
        minFactors.resize(n + 1);
        minFactors[0] = -1;
        minFactors[1] = -1;

        for (int i = 2; i <= n; i++)
            minFactors[i] = i;

        if (n >= 4) {
            for (int j = 2 * 2; j <= n; j += 2)
                minFactors[j] = 2;
        }

        int root = int(sqrt(n));
        for (int i = 3; i <= root; i += 2) {
            if (minFactors[i] == i && 1ll * i * i <= n) {
                for (int j = i * i; j <= n; j += i) {
                    if (minFactors[j] == j)
                        minFactors[j] = i;
                }
            }
        }
    }

    bool isPrimeNumber(int x) const {
        return minFactors[x] == x;
    }

    vector<int> getPrimeFactors(int x) const {
        vector<int> res;
        while (x > 1) {
            res.push_back(minFactors[x]);
            x /= minFactors[x];
        }
        return res;
    }

    vector<pair<int,int>> getPrimeFactorsEx(int x) const {
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

    // O(N) (the time complexity to get min prime factors is not included)
    // return : factorCount - the number of factors of each number from 0 to n
    vector<int> getAllFactorCount() const {
        int n = int(minFactors.size());

        vector<int> minFactorPower(n);
        vector<int> factorCount(n);

        factorCount[0] = 0;
        factorCount[1] = 1;

        for (int i = 2; i < n; i++) {
            if (minFactors[i] == i) { // prime number
                minFactorPower[i] = 1;
                factorCount[i] = 2;
            } else {
                int p = minFactors[i];
                int j = i / p;

                if (minFactors[j] != p)
                    minFactorPower[i] = 1;
                else
                    minFactorPower[i] = minFactorPower[j] + 1;
                int a = minFactorPower[i];
                factorCount[i] = (factorCount[j] / a) * (a + 1);
            }
        }

        return factorCount;
    }
};

// prime factors of a integer number
struct PrimeFactors {
    vector<pair<int, int>>  primeFactors;

    PrimeFactors() {
    }

    explicit PrimeFactors(int x) {
        build(x);
    }

    PrimeFactors(const MinFactors& minFactors, int x) {
        build(minFactors, x);
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

    void build(const MinFactors& minFactors, int x) {
        primeFactors.clear();

        while (x > 1) {
            if (!primeFactors.empty() && primeFactors.back().first == minFactors.minFactors[x])
                primeFactors.back().second++;
            else
                primeFactors.push_back(make_pair(minFactors.minFactors[x], 1));
            x /= minFactors.minFactors[x];
        }
    }

    // find all prime numbers and prime factors from 1 to n
    static void buildAll(int n, vector<int>& primes, vector<PrimeFactors>& primeFactors) {
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
