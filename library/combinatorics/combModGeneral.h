#pragma once

// M is normal integer (neither prime number nor coprime number)
// M = p^a * q^b * r^c ... (p, q, r are prime numbers)
struct CombModGeneral {
    const long long M;              // (mod M)
    vector<pair<int, int>> P;       // prime factors

    vector<vector<int>> cntSum;     // cntSum[j][x] = SUM{log_P[j](i)}
                                    //                1<=i<=x
    vector<long long> g;            // g[x] = PRODUCT{i / gcd(g[i], M)}
                                    //        1<=i<=x

    // O(N * |PF_M| * logN), |PF_M| = the number of prime factors of M
    CombModGeneral(int maxN, long long M) : M(M) {
        P = CombModGeneral::getPrimeFactors(M);
        cntSum.assign(P.size(), vector<int>(maxN + 2, 0));
        g.assign(maxN + 2, 0);

        const int PN = int(P.size());

        g[0] = 1;
        for (int x = 1; x <= maxN; x++) {
            int x2 = x;
            for (int j = 0; j < PN; j++) {
                cntSum[j][x] = cntSum[j][x - 1];
                for (; x2 % P[j].first == 0; x2 /= P[j].first)
                    cntSum[j][x]++;
            }
            g[x] = g[x - 1] * x2 % M;
        }
    }

    // O(|PF_M| * logn + (logM)^2)
    long long comb(int n, int r) {
        if (n < r)
            return 0;
        if (n == 0 || r == 0 || n == r)
            return 1;

        const int PN = int(P.size());   // the number of prime factors

        long long mulPF = 1ll;
        bool multiple = true;
        for (int j = 0; j < PN; j++) {
            int cnt = (cntSum[j][n] - cntSum[j][r]) - cntSum[j][n - r];
            if (cnt < P[j].second)
                multiple = false;
            mulPF = mulPF * modPow(P[j].first, cnt, M) % M;
        }

        if (multiple)
            return 0;

        return mulPF * g[n] % M * modInv(g[r], M) % M * modInv(g[n - r], M) % M;
    }

    // O(n * (|PF_M| * logn + (logM)^2))
    vector<long long> buildCombTable(int n) {
        vector<long long> res(n + 1);
        res[0] = res[n] = 1;

        const int PN = int(P.size());   // the number of prime factors

        for (int r = 1; r <= n / 2; r++) {
            long long mulPF = 1ll;
            bool multiple = true;
            for (int j = 0; j < PN; j++) {
                int cnt = (cntSum[j][n] - cntSum[j][n - r]) - cntSum[j][r];
                if (cnt < P[j].second)
                    multiple = false;
                mulPF = mulPF * modPow(P[j].first, cnt, M) % M;
            }

            auto y = multiple ? 0 : mulPF * g[n] % M * modInv(g[n - r], M) % M * modInv(g[r], M) % M;
            res[r] = res[n - r] = y;
        }

        return res;
    }

    // O(n * (|PF_M| * logn + logM))
    static vector<long long> buildCombTable(int n, int M) {
        vector<long long> res(n + 1);
        res[0] = res[n] = 1;

        auto P = CombModGeneral::getPrimeFactors(M);
        const int PN = int(P.size());   // the number of prime factors

        long long p = 1;
        long long q = 1;
        vector<int> cnt(PN);
        for (int r = 1; r <= n / 2; r++) {
            int a = n - r + 1;
            int b = r;

            long long mulPF = 1ll;
            for (int j = 0; j < PN; j++) {
                for (; a % P[j].first == 0; a /= P[j].first)
                    cnt[j]++;
                for (; b % P[j].first == 0; b /= P[j].first)
                    cnt[j]--;

                mulPF = mulPF * modPow(P[j].first, cnt[j], M) % M;
            }

            p = p * a % M;
            q = q * b % M;

            auto y = p * modInv(q, M) % M * mulPF % M;
            res[r] = res[n - r] = y;
        }

        return res;
    }

private:
    static vector<pair<int,int>> getPrimeFactors(long long x) {
        vector<pair<int,int>> res;

        int root = int(sqrt(x));
        if (root >= 2) {
            if (x % 2 == 0) {
                res.emplace_back(2, 1);
                while ((x /= 2) % 2 == 0)
                    ++res.back().second;
            }
        }

        for (int i = 3; i <= root; i += 2) {
            if (x % i == 0) {
                res.emplace_back(i, 1);
                while ((x /= i) % i == 0)
                    ++res.back().second;
            }
        }

        if (x > 1)
            res.emplace_back(int(x), 1);

        return res;
    }

    static long long modPow(int x, int n, long long M) {
        long long res = 1;
        long long t = x % M;
        while (n) {
            if (n & 1)
                res = 1ll * res * t % M;
            t = 1ll * t * t % M;
            n >>= 1;
        }
        return res;
    }

#if 1
    // a and M are coprime
    static long long modInv(long long a, long long M) {
        long long b = M;
        long long y = 0, x = 1;

        if (M == 1)
            return 0;

        while (a > 1 && b != 0) {
            long long q = a / b;

            long long t = b;
            b = a % b;
            a = t;

            t = y;
            y = x - q * y;
            x = t;
        }

        x %= M;
        if (x < 0)
            x += M;

        return x;
    }
#else
    // a * x + b * y = gcd(a, b)
    static long long extGcd(long long a, long long b, long long& x, long long& y) {
        if (b == 0) {
            x = 1;
            y = 0;
            return a;
        }

        long long x1, y1;
        long long g = extGcd(b, a % b, x1, y1);

        x = y1;
        y = x1 - (a / b) * y1;
        return g;
    }

    // a and mod are coprime.
    static long long modInv(long long a, long long M) {
        long long x, y;
        extGcd(a, M, x, y);
        //if (extGcd(a, M, x, y) > 1)
        //    return -1;
        return (x % M + M) % M;
    }
#endif
};
