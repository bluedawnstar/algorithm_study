#pragma once

#include "ntt.h"

// It's slower than PolyFFTMod
struct PolyNTT {
    static vector<int> multiplySlow(const vector<int>& left, const vector<int>& right, int mod) {
        vector<int> res(left.size() + right.size() - 1);

        for (int i = 0; i < int(right.size()); i++) {
            for (int j = 0; j < int(left.size()); j++) {
                res[i + j] = int((res[i + j] + 1ll * left[j] * right[i]) % mod);
            }
        }

        return res;
    }

    static vector<int> multiply(const vector<int>& a, const vector<int>& b, int mod) {
        if (int(a.size() + b.size()) <= 256)
            return multiplySlow(a, b, mod);

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
        if (int(a.size() + b.size()) <= 256)
            return multiplySlow(a, b, mod);

        static NTT ntt1(167772161, 3);
        static NTT ntt2(469762049, 3);
        static NTT ntt3(998244353, 3);

        auto x = ntt1.multiply(a, b);
        auto y = ntt2.multiply(a, b);
        auto z = ntt3.multiply(a, b);

        const int m1 = ntt1.mod;
        const int m2 = ntt2.mod;
        const int m3 = ntt3.mod;

        const int m1InvM2 = modInv(m1, m2);
        const int m12InvM3 = modInv(1ll * m1 * m2 % m3, m3);
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

    //--- extended operations

    static vector<int> square(const vector<int>& a, int mod) {
        return multiplyFast(a, a, mod);
    }


    // low order first
    static vector<int> inverse(vector<int> a, int mod) {
        //assert(!a.empty());
        int n = int(a.size());
        vector<int> b = { modInv(a[0], mod) };
        while (int(b.size()) < n) {
            vector<int> a_cut(a.begin(), a.begin() + min(a.size(), b.size() << 1));
            vector<int> x = multiply(square(b, mod), a_cut, mod);
            b.resize(b.size() << 1);
            for (int i = int(b.size()) >> 1; i < int(min(x.size(), b.size())); i++)
                b[i] = mod - x[i];
        }
        b.resize(n);
        return b;
    }

    // low order first
    static vector<int> differentiate(vector<int> a, int mod) {
        a.back() = 0;
        for(int i = 1; i < int(a.size()); i++)
            a[i - 1] = int(1ll * a[i] * i % mod);  
        return a;
    }

    // low order first
    static vector<int> integrate(vector<int> a, int mod) {
        for(int i = int(a.size()) - 1; i > 0; i--)
            a[i] = int(1ll * a[i - 1] * modInv(i, mod) % mod);
        a[0] = 0;  
        return a;
    }

    // ln f(x) = INTEGRAL f'(x) / f(x)
    // low order first
    static vector<int> ln(vector<int> a, int mod) {
        auto A = inverse(a, mod);
        auto B = differentiate(a, mod);
        A = multiply(A, B, mod);
        A.resize(a.size());
        return integrate(A, mod);  
    }

    // low order first
    static vector<int> exp(vector<int> a, int mod) {
        int size = 1;
        while (size < int(a.size()))
            size <<= 1;

        if(size == 1)
            return{ 1 };

        a.resize(size);

        vector<int> dd(a.begin(), a.begin() + (size >> 1));

        vector<int> b = exp(dd, mod);
        b.resize(size);

        vector<int> c = ln(b, mod);
        for (int i = 0; i < size; i++)
            c[i] = int((a[i] - c[i] + mod) % mod);
        c[0]++;

        b = multiply(b, c, mod);
        b.resize(size);

        return b;
    }

    // a[0] != 0
    static vector<int> powFast(const vector<int>& a, int n, int mod) { // n >= 0
        auto b = ln(a, mod);
        for (int i = 0; i < int(b.size()); i++)
            b[i]= int(1ll * b[i] * n % mod);
        return exp(b, mod);
    }

    static vector<int> pow(const vector<int>& p, int n, int maxDegree, int mod) {
        if (n == 0)
            return{ 1 };

        auto poly = pow(p, n / 2, maxDegree, mod);
        poly = square(poly, mod);
        if (int(poly.size()) > maxDegree + 1)
            poly.resize(maxDegree + 1);

        if (n & 1) {
            poly = multiply(poly, p, mod);
            if (int(poly.size()) > maxDegree + 1)
                poly.resize(maxDegree + 1);
        }

        return poly;
    }

//private:
    static int garner(vector<pair<int, int>> p, int mod) {
        int n = int(p.size());

        p.emplace_back(mod, 0);

        // a * x + b == p[i].second (mod p[i].first)

        vector<int> a(p.size(), 1);
        vector<int> b(p.size(), 0);
        for (int i = 0; i < n; i++) {
            int x = int(1ll * (p[i].second - b[i]) * modInv(a[i], p[i].first) % p[i].first);
            if (x < 0)
                x += p[i].first;

            for (int j = i + 1; j < int(p.size()); j++) {
                b[j] = int((1ll * a[j] * x + b[j]) % p[j].first);
                a[j] = int((1ll * a[j] * p[i].first) % p[j].first);
            }
        }

        return b[n];
    }

    static int modPow(int x, int n, int mod) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return int(res);
    }

    // mod is a prime number.
    static int modInv(int a, int mod) {
        return modPow(a, mod - 2, mod);
    }
};
