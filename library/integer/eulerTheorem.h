#pragma once

/*
    1. Fermat's Little Theorem
        a^(p - 1) = 1 (mod p),  p is a prime number

    2. Euler's Theorem
        a^phi(m) = 1 (mod m),   a and m are coprime
    <=> a^n (mod m) = a^(n % phi(m)) (mod m)

        phi(n) = the number of coprimes to n in [1, n]
               = n * (1 - 1 / p1) * (1 - 1 / p2) * ...  (p1, p2... : prime factors of n)
*/

template <typename T>
T phi(T n) {
    T res = n;

    if (n < 2)
        return res;

    int root = int(sqrt(n));
    if (root >= 2 && n % 2 == 0) {
        do {
            n /= 2;
        } while (n % 2 == 0);
        res -= res / 2;
    }

    for (int i = 3; i <= root; i += 2) {
        if (n % i == 0) {
            do {
                n /= i;
            } while (n % i == 0);
            res -= res / i;
        }
    }

    if (n > 1)
        res -= res / n;

    return res;
}

template <typename T>
T phiFast(T n) {
    T res = n;

    if (n < 2)
        return res;

    if (n % 2 == 0) {
        do {
            n /= 2;
        } while (n % 2 == 0);
        res -= res / 2;
    }

    for (T i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            do {
                n /= i;
            } while (n % i == 0);
            res -= res / i;
        }
    }

    if (n > 1)
        res -= res / n;

    return res;
}

// 'primes' is prime numbers in [0, sqrt(n)]
template <typename T>
T phiFast(T n, const vector<T>& primes) {
    T res = n;

    if (n < 2)
        return res;

    int idx = 0;
    T pf = primes[0];

    while (pf * pf <= n) {
        if (n % pf == 0) {
            do {
                n /= pf;
            } while (n % pf == 0);
            res -= res / pf;
        }
        pf = primes[++idx];
    }

    if (n > 1)
        res -= res / n;

    return res;
}

// 'primeFactors' is prime factors of 'n'
template <typename T>
T phiFastest(T n, const vector<T>& primeFactors) {
    T res = n;

    for (auto p : primeFactors)
        res -= res / p;

    return res;
}

// all phi of [0, n]
template <typename T>
vector<T> phiAll(T n) {
    vector<T> res(n + 1);
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
