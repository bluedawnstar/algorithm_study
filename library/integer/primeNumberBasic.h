#pragma once

// check if x is a prime number
inline bool isPrimeNumber(int x) {
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

//--- Fetmat's little theorem

inline static int _power(int a, int n, int p) {
    int res = 1;

    a = a % p;
    while (n > 0) {
        if (n & 1)
            res = int(((long long)res * a) % p);

        n = n >> 1;
        a = int(((long long)a * a) % p);
    }

    return res;
}

// If n is prime, then always returns true,
// If n is composite then returns false with high probability
// k=try count, O(KlogX)
inline bool isPrimeNumberWithFetmat(int x, int k) {
    if (x <= 1 || x == 4)
        return false;
    if (x <= 3)
        return true;

    // try k times
    while (k > 0) {
        int a = 2 + int(((long long)rand() * rand()) % (x - 4));

        // Fermat's little theorem
        if (_power(a, x - 1, x) != 1)
            return false;

        k--;
    }

    return true;
}

//--- Miller-Rabin method

// It returns false if n is composite and returns false if n is probably prime.
// d is an odd number such that  d*2^r = n-1 for some r >= 1
inline static bool _millerTest(int d, int n) {
    int a = 2 + int(((long long)rand() * rand()) % (n - 4));

    // compute a^d % n
    int x = _power(a, d, n);

    if (x == 1 || x == n - 1)
        return true;

    while (d != n - 1) {
        x = (x * x) % n;
        d *= 2;

        if (x == 1)
            return false;
        if (x == n - 1)
            return true;
    }

    return false;
}

// It returns false if n is composite and returns true if n is probably prime.
inline bool isPrimeNumberWithMiller(int n, int k) {
    if (n <= 1 || n == 4)
        return false;
    if (n <= 3)
        return true;

    // find r such that n = 2^d * r + 1 for some r >= 1
    int d = n - 1;
    while ((d & 1) == 0)
        d /= 2;

    for (int i = 0; i < k; i++)
        if (_millerTest(d, n) == false)
            return false;

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
