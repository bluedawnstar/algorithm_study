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
T phi(T n, const vector<T>& primeFactors) {
    T res = n;

    for (auto p : primeFactors)
        res -= res / p;

    return res;
}
