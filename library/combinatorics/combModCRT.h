#pragma once

// Chinese Remainder Theorem

// M = p^1 * q^1 * r^1 ... (p, q, r are prime numbers)
struct CombModCRT {
    const long long M;                  // (mod M)
    vector<int> P;                      // prime factors

    vector<vector<int>> fact;           // factorial
    vector<vector<int>> factInverse;    // inverse factorial

    CombModCRT(int maxN, long long M) : M(M) {
        P = CombModCRT::getPrimeFactors(M);
        const int PN = int(P.size());   // the number of prime factors

        fact.resize(PN);
        factInverse.resize(PN);

        for (int j = 0; j < PN; j++) {
            const int mod = P[j];
            const int n = min(mod - 1, maxN);

            vector<int> inverse(n + 1);
            fact[j].resize(n + 1);
            factInverse[j].resize(n + 1);

            inverse[1] = 1;
            fact[j][0] = 1;
            fact[j][1] = 1;
            factInverse[j][0] = 1;
            factInverse[j][1] = 1;
            for (int x = 2; x <= n; x++) {
                inverse[x] = int((mod - 1ll * (mod / x) * inverse[mod % x] % mod) % mod);
                fact[j][x] = int(1ll * fact[j][x - 1] * x % mod);
                factInverse[j][x] = int(1ll * factInverse[j][x - 1] * inverse[x] % mod);
            }
        }
    }

    //---

    long long combLucas(int n, int r) {
        if (n < r)
            return 0;
        if (n == 0 || r == 0 || n == r)
            return 1;

        if (n - r < r)
            r = n - r;

        const int PN = int(P.size());

        vector<int> R(PN);
        for (int i = 0; i < PN; i++) {
            const int mod = P[i];
            int C = 1;
#if 1
            for (int nn = n, rr = r; nn != 0 && rr != 0 && nn != rr; nn /= mod, rr /= mod) {
                // C = C * comb(nn % mod, rr % mod) % mod
                const int nnR = nn % mod;
                const int rrR = rr % mod;
                if (nnR < rrR) {
                    C = 0;
                    break;
                }
                C = int(1ll * C * fact[i][nnR] % mod * factInverse[i][nnR - rrR] % mod * factInverse[i][rrR] % mod);
            }
#else
            for (int nn = n, rr = r; nn != 0 && rr != 0 && nn != rr; nn /= mod, rr /= mod) {
                // C = C * comb(nn % mod, rr % mod) % mod
                const int nnR = nn % mod;
                const int rrR = rr % mod;
                C = int(1ll * C * modComb(nnR, rrR, mod) % mod);
            }
#endif
            R[i] = C;
        }

        return solveCRT(P, R, M);
        //return solveCRTSimple(P, R, M);
    }

    static long long combLucas(int n, int r, const vector<int>& P) {
        if (n < r)
            return 0;
        if (n == 0 || r == 0 || n == r)
            return 1;

        if (n - r < r)
            r = n - r;

        const int PN = int(P.size());

        long long M = 1;
        vector<int> R(PN);
        for (int i = 0; i < PN; i++) {
            R[i] = modCombLucas(n, r, P[i]);
            M *= P[i];
        }

        return solveCRT(P, R, M);
    }

private:
    static long long solveCRTSimple(const vector<int>& pf, const vector<int>& R, long long M) {
        const int n = int(pf.size());

        for (int x = 0; x < M; x++) {
            bool ok = true;
            for (int j = 0; j < n && ok; j++) {
                ok = (x % pf[j] == R[j]);
            }
            if (ok)
                return x;
        }
        return -1;
    }

    static long long solveCRT(const vector<int>& pf, const vector<int>& R, long long M) {
        int n = int(pf.size());

        long long res = 0;
        for (int i = 0; i < n; i++) {
            long long nk = M / pf[i];
            res = (res + 1ll * R[i] * nk % M * modInv(nk, pf[i])) % M;
        }

        return res;
    }

    static int modComb(int n, int r, int M) {
        if (n < r)
            return 0;

        if (n == 0 || r == 0 || n == r)
            return 1;

        if (n - r < r)
            r = n - r;

        long long p = 1;
        long long q = 1;
        for (int i = 1; i <= r; i++) {
            p = p * n-- % M;
            q = q * i % M;
        }

        return int(p * modInv(q, M) % M);
    }

    // Lucas' Theorem
    static int modCombLucas(int n, int r, int M) {
        //if (n < r)
        //    return 0;
        if (n == 0 || r == 0 || n == r)
            return 1;

        return int(1ll * modCombLucas(n / M, r / M, M) * modComb(n % M, r % M, M) % M);
    }

    static vector<int> getPrimeFactors(long long x) {
        vector<int> res;

        int root = int(sqrt(x));
        if (root >= 2) {
            if (x % 2 == 0) {
                res.emplace_back(2);
                x /= 2;
            }
        }

        for (int i = 3; i <= root; i += 2) {
            if (x % i == 0) {
                res.emplace_back(i);
                x /= i;
            }
        }

        if (x > 1)
            res.emplace_back(int(x));

        return res;
    }

    static int modPow(long long x, long long n, int M) {
        if (x == 0 && n > 0)
            return 0;

        n %= M - 1;

        long long res = 1;
        long long t = x % M;
        while (n) {
            if (n & 1)
                res = res * t % M;
            t = t * t % M;
            n >>= 1;
        }
        return int(res);
    }

    static int modInv(long long x, int M) {
        return modPow(x, M - 2, M);
    }
};
