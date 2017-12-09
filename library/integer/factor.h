#pragma once


// return factors of a number x
// O(sqrt(N))
template <typename T>
vector<T> getFactors(T x) {
    vector<T> res;
    if (x <= 0)
        return res;

    int i;
    for (i = 1; T(i) * i < x; i++) {
        if (x % i == 0) {
            res.push_back(T(i));
            res.push_back(x / i);
        }
    }

    if (T(i) * i == x)
        res.push_back(i);

    sort(res.begin(), res.end());

    return res;
}


// input : pairs of (a prime number, a prime number's power)
// return : all factors
inline vector<int> getFactors(const vector<pair<int, int>>& primeFactors) {
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
    for (int i = 1; i < (int)primeFactors.size(); i++) {
        int prevN = (int)res.size();

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

//--------- Factor Count ------------------------------------------------------

// a^メ * b^モ * c^ャ  ==>  (メ + 1) * (モ + 1) * (ャ + 1)
// input : pairs of (a prime number, a prime number's power)
inline int getFactorCount(const vector<pair<int, int>>& primeFactors) {
    int res = 1;
    for (auto& it : primeFactors)
        res *= it.second + 1;
    return res;
}

// O(N) (the time complexity to get min prime factors is not included)
// 
// input : minFactors - min prime factors from 0 to n (see to primeNumberEratosthenes.cpp)
// return : factorCount - the number of factors of each number from 0 to n
inline vector<int> getAllFactorCount(const vector<int>& minFactors) {
    int n = (int)minFactors.size();

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

// O(N logN)
// It's faster than getAllFactorCount() because it's very simple.
inline vector<int> getAllFactorCountSimple(int n) {
    vector<int> factorCount(n + 1, 1);
    factorCount[0] = 0;

    for (int div = 2; div <= n; div++) {
        for (int m = div; m <= n; m += div)
            factorCount[m]++;
    }

    return factorCount;
}
