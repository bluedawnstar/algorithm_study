#pragma once

/*
  "Number of integer-sided right triangles with n as a hypotenuse or leg."
  - http://oeis.org/A046081 = http://oeis.org/A046079 + http://oeis.org/A046080

  "ON PYTHAGOREAN TRIPLES CONTAINING A FIXED INTEGER"
  - https://www.fq.math.ca/Papers1/46_47-4/Tripathi.pdf


  1. the number of pythagorean triples with interget-sides
     - n is any integer-side of a triple
     - n1 denote the largest odd divisor of n each of whose prime divisors is of the form 4k + 1
       (n1 = 1 if no such prime divisor exists)
     - d(n) denotes the number of positive divisors of n

     1) n is odd
                { d(n^2) + d(n1^2) }
         P(n) = --------------------  -  1           (n >= 3)
                         2

     1) n is even
                { d((n/2)^2) + d(n1^2) }
         P(n) = ------------------------  -  1       (n >= 3)
                         2
*/
struct IntegerSidedPythagoreanTriples {
    vector<int> primes;

    IntegerSidedPythagoreanTriples() {
    }

    explicit IntegerSidedPythagoreanTriples(long long maxN) {
        prepare(maxN);
    }

    void prepare(long long maxN) {
        primes = eratosthenes2(int(sqrt(double(maxN))) + 1);
    }

    // n is any integer-side of a triple
    long long count(long long n) {
        if (n < 3)
            return 0ll;

        long long d = 1;
        long long d1 = 1;

        auto factors = getPrimeFactors(n);
        if (n & 1) { // odd number
            for (auto& it : factors) {
                d *= it.second + 1;
                if ((it.first & 3) == 1)    // 4*k + 1
                    d1 *= it.second + 1;
            }
        } else {
            auto factors2 = getPrimeFactors(n >> 1);
            for (auto& it : factors) {
                if ((it.first & 3) == 1)    // 4*k + 1
                    d1 *= it.second + 1;
            }
            for (auto& it : factors2) {
                d *= it.second + 1;
            }
        }
        return (d + d1) / 2 - 1;
    }

private:
    vector<bool> eratosthenes(int n) {
        vector<bool> res(n + 1, true);
        res[0] = false;
        res[1] = false;

        if (n >= 4) {
            for (int j = 2 * 2; j <= n; j += 2)
                res[j] = false;
        }

        int root = int(sqrt(n));
        for (int i = 3; i <= root; i += 2) {
            if (res[i]) {
                for (int j = i * i; j >= 0 && j <= n; j += i)
                    res[j] = false;
            }
        }

        return res;
    }

    // get all prime numbers in [0, n], inclusive
    // O(N loglogN)
    vector<int> eratosthenes2(int n) {
        vector<bool> p = eratosthenes(n);

        vector<int> res;
        if (n >= 2)
            res.push_back(2);
        for (int i = 3; i <= n; i += 2)
            if (p[i])
                res.push_back(i);

        return res;
    }

    vector<pair<long long, int>> getPrimeFactors(long long X) {
        vector<pair<long long, int>> res;
        for (int i = 0; i < int(primes.size()) && primes[i] <= X; i++) {
            int n = 0;
            while (X % primes[i] == 0) {
                n++;
                X /= primes[i];
            }
            res.emplace_back(primes[i], n * 2);
        }
        if (X > 1)
            res.emplace_back(X, 2);

        return res;
    }

    long long countDivisors(const vector<pair<long long, int>>& primeFactors) {
        long long res = 1;
        for (auto& it : primeFactors) {
            res *= it.second + 1;
        }
        return res;
    }
};
