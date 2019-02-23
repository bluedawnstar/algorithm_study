#pragma once

// Number Theoretic Transforms
// M = 998244353 (119 * 2^23 + 1), primitive root = 3
//
// [CAUTION]
// It's not working for below M's
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
                    int x = int((0ll + a[j] - a[k] + M) % M);
                    a[j] = (0ll + a[j] + a[k]) % M;
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