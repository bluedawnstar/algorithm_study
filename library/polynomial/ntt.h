#pragma once

// Number Theoretic Transforms
// M = 998244353 (119 * 2^23 + 1), primitive root = 3
//
// [CAUTION]
// It's not working for below 'M's. Use PolyNTT for these
//    - 10^9 + 7
//    - 10^9 + 9
struct NTT {
    int mod, root;

    NTT(int mod, int root) {
        this->mod = mod;
        this->root = root;
    }

    void ntt(vector<int>& a, bool inverse = false) {
        int n = int(a.size());

        int base = modPow(root, (mod - 1) / n);
        if (inverse)
            base = modInv(base);

        for (int m = n; m >= 2; m >>= 1) {
            int mh = m >> 1;
            int w = 1;
            for (int i = 0; i < mh; i++) {
                for (int j = i; j < n; j += m) {
                    int k = j + mh;
                    int x = int((a[j] - a[k] + mod) % mod);
                    a[j] = (a[j] + a[k]) % mod;
                    a[k] = int(1ll * w * x % mod);
                }
                w = int(1ll * w * base % mod);
            }
            base = int(1ll * base * base % mod);
        }

        int i = 0;
        for (int j = 1; j < n - 1; j++) {
            for (int k = n >> 1; k > (i ^= k); k >>= 1)
                ;
            if (j > i)
                swap(a[i], a[j]);
        }

        if (inverse) {
            int inverse = modInv(n);
            for (int j = 0; j < n; j++)
                a[j] = int(1ll * a[j] * inverse % mod);
        }
    }


    vector<int> multiplySlow(const vector<int>& left, const vector<int>& right) {
        vector<int> res(left.size() + right.size() - 1);

        for (int i = 0; i < int(right.size()); i++) {
            for (int j = 0; j < int(left.size()); j++) {
                res[i + j] = int((res[i + j] + 1ll * left[j] * right[i]) % mod);
            }
        }

        return res;
    }

    vector<int> multiply(const vector<int>& a, const vector<int>& b, bool reverseB = false) {
        int n = int(a.size()) + int(b.size()) - 1;
        if (n <= 128)
            return multiplySlow(a, b);

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
            C[i] = int(1ll * A[i] * B[i] % mod);
        ntt(C, true);

        C.resize(n);

        return C;
    }

    vector<int> convolute(const vector<int>& x, const vector<int>& h, bool reverseH = true) {
        return multiply(x, h, reverseH);
    }

    //--- extended operations

    vector<int> square(const vector<int>& a) {
        int n = int(a.size()) * 2 - 1;
        if (n < 128)
            return multiplySlow(a, a);

        int size = 1;
        while (size < n)
            size <<= 1;

        vector<int> A(size);
        copy(a.begin(), a.end(), A.begin());

        ntt(A);

        vector<int> C(size);
        for (int i = 0; i < size; i++)
            C[i] = int(1ll * A[i] * A[i] % mod);
        ntt(C, true);

        C.resize(n);

        return C;
    }

    // low order first
    vector<int> inverse(vector<int> a) {
        int size = 1;
        while (size < int(a.size()))
            size <<= 1;

        if (int(a.size()) != size)
            a.resize(size);

        if (size == 1)
            return{ modInv(a[0]) }; // 1/a[0]

        vector<int> b(a.begin(), a.begin() + (size >> 1));
        b = inverse(b);

        a.resize(size << 1);
        b.resize(size << 1);

        ntt(a);
        ntt(b);
        for(int i = 0; i < (size << 1); i++)
            b[i] = int(((b[i] * 2ll - 1ll * a[i] * b[i] % mod * b[i]) % mod + mod) % mod);
        // b[i] = b[i] * 2 - a[i] * b[i]^2
        ntt(b, true);
        b.resize(size); 

        return b;
    }

    // low order first
    vector<int> differentiate(vector<int> a) {
        a.back() = 0;
        for(int i = 1; i < int(a.size()); i++)
            a[i - 1] = int(1ll * a[i] * i % mod);  
        return a;
    }

    // low order first
    vector<int> integrate(vector<int> a) {
        for(int i = int(a.size()) - 1; i > 0; i--)
            a[i] = int(1ll * a[i - 1] * modInv(i) % mod);
        a[0] = 0;  
        return a;
    }

    // ln f(x) = INTEGRAL f'(x) / f(x)
    // low order first
    vector<int> ln(vector<int> a) {
        auto A = inverse(a);
        auto B = differentiate(a);
        A = multiply(A, B);
        A.resize(a.size());
        return integrate(A);  
    }

    // low order first
    vector<int> exp(vector<int> a) {
        int size = 1;
        while (size < int(a.size()))
            size <<= 1;

        if(size == 1)
            return{ 1 };

        a.resize(size);

        vector<int> dd(a.begin(), a.begin() + (size >> 1));

        vector<int> b = exp(dd);
        b.resize(size);

        vector<int> c = ln(b);
        for (int i = 0; i < size; i++)
            c[i] = int((a[i] - c[i] + mod) % mod);
        c[0]++;

        b = multiply(b, c);
        b.resize(size);

        return b;
    }

    // a[0] != 0
    vector<int> powFast(const vector<int>& a, int n) { // n >= 0
        auto b = ln(a);
        for (int i = 0; i < int(b.size()); i++)
            b[i]= int(1ll * b[i] * n % mod);
        return exp(b);
    }

    vector<int> pow(const vector<int>& p, int n, int maxDegree) {
        if (n == 0)
            return{ 1 };

        auto poly = pow(p, n / 2, maxDegree);
        poly = square(poly);
        if (int(poly.size()) > maxDegree + 1)
            poly.resize(maxDegree + 1);

        if (n & 1) {
            poly = multiply(poly, p);
            if (int(poly.size()) > maxDegree + 1)
                poly.resize(maxDegree + 1);
        }

        return poly;
    }

//private:
    int modPow(int x, int n) {
        if (n == 0)
            return 1;

        int p = modPow(x, n / 2) % mod;
        p = int(1ll * p * p % mod);

        return ((n & 1) == 0) ? p : int(1ll * p * x % mod);
    }

    int modInv(int a) {
        return modPow(a, mod - 2);
    }
};
