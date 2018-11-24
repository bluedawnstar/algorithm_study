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

// O(N logN)
// It's fast because it's very simple.
inline vector<int> getAllFactorCountSimple(int n) {
    vector<int> factorCount(n + 1, 1);
    factorCount[0] = 0;

    for (int div = 2; div <= n; div++) {
        for (int m = div; m <= n; m += div)
            factorCount[m]++;
    }

    return factorCount;
}


#include "primalityTest.h"

// https://codeforces.com/blog/entry/22317
struct DivisorCounter {
    const int TABLE_SIZE = 1000000;

    vector<bool> primeTest;
    vector<int> primes;

    DivisorCounter() {
        eratosthenes();
        eratosthenes2();
    }

    // O(N^(1/3))
    template <typename T>
    int count(T n) const {
        int ans = 1;

        for (auto p : primes) {
            T ppp = T(p) * p * p;
            if (ppp > n)
                break;

            int count = 1;
            while (n % p == 0) {
                n /= p;
                count++;
            }
            ans *= count;
        }

        if (n > 1) {
            if (isPrime(n))
                ans *= 2;
            else {
                T sqrtN = T(sqrt(n));
                if (sqrtN * sqrtN == n)
                    ans *= 3;
                else
                    ans *= 4;
            }
        }

        return ans;
    }

private:
    template <typename T>
    bool isPrime(T n) const {
        if (n <= TABLE_SIZE)
            return primeTest[int(n)];
        
        //TODO: optimize

        return PrimalityTest<T>::isPrimeNumber(n);
    }

    // O(N*loglogN)
    void eratosthenes() {
        primeTest = vector<bool>(TABLE_SIZE + 1, true);
        primeTest[0] = false;
        primeTest[1] = false;

        if (TABLE_SIZE >= 4) {
            for (int j = 2 * 2; j <= TABLE_SIZE; j += 2)
                primeTest[j] = false;
        }

        int root = int(sqrt(TABLE_SIZE));
        for (int i = 3; i <= root; i += 2) {
            if (primeTest[i]) {
                for (int j = i * i; j >= 0 && j <= TABLE_SIZE; j += i)
                    primeTest[j] = false;
            }
        }
    }

    // O(N)
    void eratosthenes2() {
        primes.clear();

        if (TABLE_SIZE >= 2)
            primes.push_back(2);

        for (int i = 3; i <= TABLE_SIZE; i += 2)
            if (primeTest[i])
                primes.push_back(i);
    }
};
