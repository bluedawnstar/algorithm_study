#pragma once

// check if x is a prime number (slow!)
template <typename T>
inline bool isPrimeNumber(T x) {
    if (x < 2)
        return false;
    else if (x == 2)
        return true;
    else if ((x & 1) == 0)
        return false;

    int root = int(sqrt(x));
    for (int i = 3; i <= root; i += 2) {
        if (x % i == 0)
            return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

// return all prime numbers from 1 to n
inline vector<int> findPrimeNumbers(int n) {
    vector<int> res;
    if (n < 2)
        return res;

    if (n >= 2)
        res.push_back(2);

    for (int i = 3; i <= n; i += 2) {
        bool isPrime = true;
        int root = int(sqrt(i));
        for (int j = 0; res[j] <= root; j++) {
            if (i % res[j] == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime)
            res.push_back(i);
    }

    return res;
}

// return prime factors of x
inline vector<int> getPrimeFactors(int x) {
    vector<int> res;

    int root = int(sqrt(x));
    if (root >= 2) {
        while (x % 2 == 0) {
            x /= 2;
            res.push_back(2);
        }
    }

    for (int i = 3; i <= root; i += 2) {
        while (x % i == 0) {
            x /= i;
            res.push_back(i);
        }
    }

    if (x > 1)
        res.push_back(x);

    return res;
}

// return prime factors of x
inline vector<int> getPrimeFactorsFast(int x) {
    vector<int> res;

    while (x % 2 == 0) {
        x /= 2;
        res.push_back(2);
    }

    for (int i = 3; i * i <= x; i += 2) {
        while (x % i == 0) {
            x /= i;
            res.push_back(i);
        }
    }

    if (x > 1)
        res.push_back(x);

    return res;
}

// <PRECONDITION>
//   - 'primes' has all primes in [0, sqrt(N)]
// return prime factors of x, O(sqrtN / log(sqrtN))
template <typename T>
inline vector<T> getPrimeFactorsFast(T x, const vector<int>& primes) {
    vector<T> res;

    int idx = 0;
    T pf = primes[0];

    while (pf * pf <= x) {
        while (x % pf == 0) {
            x /= pf;
            res.push_back(pf);
        }
        pf = primes[++idx];
    }

    if (x > 1)
        res.push_back(x);

    return res;
}
