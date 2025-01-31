#pragma once

/*
    phi(n) = the number of coprimes to n in [1, n]
            = n * (1 - 1 / p1) * (1 - 1 / p2) * ...  (p1, p2... : prime factors of n)
*/
struct EulerPhi {
    vector<int> primes;     // prime numbers in [0, sqrt(maxN)]
    vector<int> spf;        // minimum prime factors of [0, sqrt(maxN)]

    EulerPhi() {
    }

    explicit EulerPhi(int maxN) {
        build(maxN);
    }

    // O(sqrt(N))
    void build(int maxN) {
        int sqrtN = int(sqrt(maxN));

        // get all prime numbers in [0, n], inclusive, O(N)
        //-- linear sieve
        primes.clear();
        spf.resize(max(2, sqrtN + 1));
        spf[0] = -1;
        spf[1] = -1;

        if (maxN >= 2) {
            primes.push_back(2);
            for (int j = 2; j <= sqrtN; j += 2)
                spf[j] = 2;
        }
        for (int i = 3; i <= sqrtN; i += 2) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int j = 1, pn = int(primes.size()); j < pn && primes[j] <= spf[i] && (i * primes[j]) <= sqrtN; j++)
                spf[i * primes[j]] = primes[j];
        }
    }

    // O(PI(sqrt(N)) ~= O(sqrt(N) / log(N))
    int phiFast(int n) const {
        int res = n;

        if (n < 2)
            return res;

        int idx = 0;
        int p = primes[0];

        while (p * p <= n) {
            if (n % p == 0) {
                do {
                    n /= p;
                } while (n % p == 0);
                res -= res / p;
            }
            if (++idx >= int(primes.size()))
                break;
            p = primes[idx];
        }

        if (n > 1)
            res -= res / n;

        return res;
    }

    //---

    // O(sqrt(N))
    static int phi(int n) {
        int res = n;

        if (n < 2)
            return res;

        if (n % 2 == 0) {
            do {
                n /= 2;
            } while (n % 2 == 0);
            res -= res / 2;
        }

        for (int i = 3; i * i <= n; i += 2) {
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

    // all phi of [0, n]
    static vector<int> phiAll(int n) {
        vector<int> res(n + 1);
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
};
