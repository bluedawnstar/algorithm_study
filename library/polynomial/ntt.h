#pragma once

// Number Theoretic Transforms
// M = 998244353 (119 * 2^23 + 1), primitive root = 3
//
// [CAUTION]
// It's not working for below 'M's. Use PolyNTT for these
//    - 10^9 + 7
//    - 10^9 + 9
struct NTT {
    int M, root;

    NTT(int mod, int root) {
        this->M = mod;
        this->root = root;
    }

    void ntt(vector<int>& a, bool inverse = false) {
        int n = int(a.size());

        int base = modPow(root, (M - 1) / n);
        if (inverse)
            base = modInv(base);

        for (int m = n; m >= 2; m >>= 1) {
            int mh = m >> 1;
            int w = 1;
            for (int i = 0; i < mh; i++) {
                for (int j = i; j < n; j += m) {
                    int k = j + mh;
                    int x = int((a[j] - a[k] + M) % M);
                    a[j] = (a[j] + a[k]) % M;
                    a[k] = int(1ll * w * x % M);
                }
                w = int(1ll * w * base % M);
            }
            base = int(1ll * base * base % M);
        }

        int i = 0;
        for (int j = 1; j < n - 1; j++) {
            for (int k = n >> 1; k > (i ^= k); k >>= 1)
                ;
            if (j > i)
                swap(a[i], a[j]);
        }

        if (inverse) {
            int inv = modInv(n);
            for (int j = 0; j < n; j++)
                a[j] = int(1ll * a[j] * inv % M);
        }
    }

    vector<int> multiply(const vector<int>& a, const vector<int>& b, bool reverseB = false) {
        int n = int(a.size()) + int(b.size()) - 1;

        int size = 1;
        while (size < n)
            size <<= 1;

        vector<int> A(size);
        vector<int> B(size);
        copy(a.begin(), a.end(), A.begin());
        if (!reverseB)
            copy(b.begin(), b.end(), B.begin());
        else
            copy(b.begin(), b.end(), B.rbegin());
        
        ntt(A);
        ntt(B);

        vector<int> C(size);
        for (int i = 0; i < size; i++)
            C[i] = int(1ll * A[i] * B[i] % M);
        ntt(C, true);
        
        C.resize(n);

        return C;
    }

    vector<int> convolute(const vector<int>& x, const vector<int>& h, bool reverseH = true) {
        return multiply(x, h, reverseH);
    }

private:
    int modPow(int x, int n) {
        if (n == 0)
            return 1;

        int p = modPow(x, n / 2) % M;
        p = int(1ll * p * p % M);

        return ((n & 1) == 0) ? p : int(1ll * p * x % M);
    }

    int modInv(int a) {
        return modPow(a, M - 2);
    }
};

struct PolyNTT {
    static vector<int> multiply(const vector<int>& a, const vector<int>& b, int MOD) {
        static NTT ntt1(167772161, 3);
        static NTT ntt2(469762049, 3);
        static NTT ntt3(998244353, 3);

        auto x = ntt1.multiply(a, b);
        auto y = ntt2.multiply(a, b);
        auto z = ntt3.multiply(a, b);

        vector<int> res(x.size());
        vector<pair<int, int>> p(3);
        for (int i = 0; i < int(x.size()); i++) {
            p[0].first = ntt1.M;
            p[0].second = x[i];

            p[1].first = ntt2.M;
            p[1].second = y[i];

            p[2].first = ntt3.M;
            p[2].second = z[i];

            res[i] = garner(p, MOD);
        }

        return res;
    }

    static vector<int> multiplyFast(const vector<int>& a, const vector<int>& b, int MOD) {
        static NTT ntt1(167772161, 3);
        static NTT ntt2(469762049, 3);
        static NTT ntt3(998244353, 3);

        auto x = ntt1.multiply(a, b);
        auto y = ntt2.multiply(a, b);
        auto z = ntt3.multiply(a, b);

        const int m1 = ntt1.M;
        const int m2 = ntt2.M;
        const int m3 = ntt3.M;

        const int m1InvM2 = modInvIter(m1, m2);
        const int m12InvM3 = modInvIter(1ll * m1 * m2 % m3, m3);
        const int m12Mod = 1ll * m1 * m2 % MOD;

        vector<int> res(x.size());
        for (int i = 0; i < int(x.size()); i++) {
            int v1 = int(1ll * (y[i] - x[i]) *  m1InvM2 % m2);
            if (v1 < 0)
                v1 += m2;

            int v2 = int(1ll * (z[i] - (x[i] + 1ll * m1 * v1) % m3) * m12InvM3 % m3);
            if (v2 < 0)
                v2 += m3;

            int b3 = (x[i] + 1ll * m1 * v1 + 1ll * m12Mod * v2) % MOD;
            if (b3 < 0)
                b3 += MOD;

            res[i] = b3;
        }

        return res;
    }


    static vector<int> multiply(const vector<int>& a, const vector<int>& b, int MOD, bool reverseB) {
        if (!reverseB)
            return multiply(a, b, MOD);

        vector<int> revB = b;
        reverse(revB.begin(), revB.end());
        return multiply(a, revB, MOD);
    }

    static vector<int> convolute(const vector<int>& x, const vector<int>& h, bool reverseH = true) {
        return multiply(x, h, reverseH);
    }

private:
    static int garner(vector<pair<int, int>> p, int MOD) {
        int n = int(p.size());

        p.emplace_back(MOD, 0);

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
        int res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = int(res * t % M);
            t = t * t % M;
        }
        return res;
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
