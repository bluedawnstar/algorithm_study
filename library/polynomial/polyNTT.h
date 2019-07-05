#pragma once

#include "ntt.h"

struct PolyNTT {
    static vector<int> multiply(const vector<int>& a, const vector<int>& b, int mod) {
        static NTT ntt1(167772161, 3);
        static NTT ntt2(469762049, 3);
        static NTT ntt3(998244353, 3);

        auto x = ntt1.multiply(a, b);
        auto y = ntt2.multiply(a, b);
        auto z = ntt3.multiply(a, b);

        vector<int> res(x.size());
        vector<pair<int, int>> p(3);
        for (int i = 0; i < int(x.size()); i++) {
            p[0].first = ntt1.mod;
            p[0].second = x[i];

            p[1].first = ntt2.mod;
            p[1].second = y[i];

            p[2].first = ntt3.mod;
            p[2].second = z[i];

            res[i] = garner(p, mod);
        }

        return res;
    }

    static vector<int> multiplyFast(const vector<int>& a, const vector<int>& b, int mod) {
        static NTT ntt1(167772161, 3);
        static NTT ntt2(469762049, 3);
        static NTT ntt3(998244353, 3);

        auto x = ntt1.multiply(a, b);
        auto y = ntt2.multiply(a, b);
        auto z = ntt3.multiply(a, b);

        const int m1 = ntt1.mod;
        const int m2 = ntt2.mod;
        const int m3 = ntt3.mod;

        const int m1InvM2 = modInvIter(m1, m2);
        const int m12InvM3 = modInvIter(1ll * m1 * m2 % m3, m3);
        const int m12Mod = 1ll * m1 * m2 % mod;

        vector<int> res(x.size());
        for (int i = 0; i < int(x.size()); i++) {
            int v1 = int(1ll * (y[i] - x[i]) *  m1InvM2 % m2);
            if (v1 < 0)
                v1 += m2;

            int v2 = int(1ll * (z[i] - (x[i] + 1ll * m1 * v1) % m3) * m12InvM3 % m3);
            if (v2 < 0)
                v2 += m3;

            int b3 = (x[i] + 1ll * m1 * v1 + 1ll * m12Mod * v2) % mod;
            if (b3 < 0)
                b3 += mod;

            res[i] = b3;
        }

        return res;
    }


    static vector<int> multiply(const vector<int>& a, const vector<int>& b, int mod, bool reverseB) {
        if (!reverseB)
            return multiply(a, b, mod);

        vector<int> revB = b;
        reverse(revB.begin(), revB.end());
        return multiply(a, revB, mod);
    }

    static vector<int> convolute(const vector<int>& x, const vector<int>& h, bool reverseH = true) {
        return multiply(x, h, reverseH);
    }

private:
    static int garner(vector<pair<int, int>> p, int mod) {
        int n = int(p.size());

        p.emplace_back(mod, 0);

        // a * x + b == p[i].second (mod p[i].first)

        vector<int> a(p.size(), 1);
        vector<int> b(p.size(), 0);
        for (int i = 0; i < n; i++) {
            int x = int(1ll * (p[i].second - b[i]) * modInvPrime(a[i], p[i].first) % p[i].first);
            if (x < 0)
                x += p[i].first;

            for (int j = i + 1; j < int(p.size()); j++) {
                b[j] = int((1ll * a[j] * x + b[j]) % p[j].first);
                a[j] = int((1ll * a[j] * p[i].first) % p[j].first);
            }
        }

        return b[n];
    }

    static int modPow(int x, int n, int M) {
        if (n == 0)
            return 1;

        long long t = x % M;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % M;
            t = t * t % M;
        }
        return int(res);
    }

    // M is a prime number.
    static int modInvPrime(int a, int M) {
        return modPow(a, M - 2, M);
    }

    // a and M are coprime
    static int modInvIter(int a, int M) {
        int b = M;
        int y = 0, x = 1;

        if (M == 1)
            return 0;

        while (a > 1 && b != 0) {
            int q = a / b;

            int t = b;
            b = a % b;
            a = t;

            t = y;
            y = x - q * y;
            x = t;
        }

        return (x % M + M) % M;
    }
};
