#pragma once

// Fast Walsh-Hadamard transform
//  https://csacademy.com/blog/fast-fourier-transform-and-variations-of-it/
//  https://en.wikipedia.org/wiki/Fast_Walsh%E2%80%93Hadamard_transform

// Fast Walsh-Hadamard transform

template <typename T, int mod = 1000000007>
struct FWHTMod {
    // C = SUM SUM A[i] * B[j] * x^(i xor j)
    //      i   j
    static vector<T> fastXor(const vector<T>& A, const vector<T>& B) {
        int size = 1;
        while (size < int(A.size()) || size < int(B.size()))
            size <<= 1;

        vector<T> tA(size);
        vector<T> tB(size);
        for (int i = 0; i < int(A.size()); i++)
            tA[i] = A[i];
        for (int i = 0; i < int(B.size()); i++)
            tB[i] = B[i];

        transformXor(tA, false);
        transformXor(tB, false);

        for (int i = 0; i < size; i++)
            tA[i] = T(1ll * tA[i] * tB[i] % mod);

        transformXor(tA, true);

        return tA;
    }

    // C = SUM SUM A[i] * B[j] * x^(i or j)
    //      i   j
    static vector<T> fastOr(const vector<T>& A, const vector<T>& B) {
        int size = 1;
        while (size < int(A.size()) || size < int(B.size()))
            size <<= 1;

        vector<T> tA(size);
        vector<T> tB(size);
        for (int i = 0; i < int(A.size()); i++)
            tA[i] = A[i];
        for (int i = 0; i < int(B.size()); i++)
            tB[i] = B[i];

        transformOr(tA, false);
        transformOr(tB, false);

        for (int i = 0; i < size; i++)
            tA[i] = T(1ll * tA[i] * tB[i] % mod);

        transformOr(tA, true);

        return tA;
    }

    // C = SUM SUM A[i] * B[j] * x^(i and j)
    //      i   j
    static vector<T> fastAnd(const vector<T>& A, const vector<T>& B) {
        int size = 1;
        while (size < int(A.size()) || size < int(B.size()))
            size <<= 1;

        vector<T> tA(size);
        vector<T> tB(size);
        for (int i = 0; i < int(A.size()); i++)
            tA[i] = A[i];
        for (int i = 0; i < int(B.size()); i++)
            tB[i] = B[i];

        transformAnd(tA, false);
        transformAnd(tB, false);

        for (int i = 0; i < size; i++)
            tA[i] = T(1ll * tA[i] * tB[i] % mod);

        transformAnd(tA, true);

        return tA;
    }

private:
    // the size of P must be a power of 2
    static void transformXor(vector<T>& P, bool inverse) {
        int N = int(P.size());
        for (int step = 1; step < N; step <<= 1) {
            for (int i = 0; i < N; i += (step << 1)) {
                for (int j = 0; j < step; j++) {
                    T u = P[i + j];
                    T v = P[i + step + j];

                    auto a = u + v;
                    auto b = u - v;
                    if (a >= mod)
                        a -= mod;
                    if (b < 0)
                        b += mod;

                    P[i + j] = a;
                    P[i + step + j] = b;
                }
            }
        }

        if (inverse) {
            T invN = modInv(N);
            for (int i = 0; i < int(P.size()); i++)
                P[i] = T(1ll * P[i] * invN % mod);
        }
    }

    // the size of P must be a power of 2
    static void transformOr(vector<T>& P, bool inverse) {
        int N = int(P.size());
        for (int step = 1; step < N; step <<= 1) {
            for (int i = 0; i < N; i += (step << 1)) {
                for (int j = 0; j < step; j++) {
                    T u = P[i + j];
                    T v = P[i + step + j];
                    if (!inverse) {
                        P[i + j] = u + v;
                        if (P[i + j] >= mod)
                            P[i + j] -= mod;
                        P[i + step + j] = u;
                    } else {
                        P[i + j] = v;
                        P[i + step + j] = u - v;
                        if (P[i + step + j] < 0)
                            P[i + step + j] += mod;
                    }
                }
            }
        }
    }

    // the size of P must be a power of 2
    static void transformAnd(vector<T>& P, bool inverse) {
        int N = int(P.size());
        for (int step = 1; step < N; step <<= 1) {
            for (int i = 0; i < N; i += (step << 1)) {
                for (int j = 0; j < step; j++) {
                    T u = P[i + j];
                    T v = P[i + step + j];
                    if (!inverse) {
                        P[i + j] = v;
                        P[i + step + j] = v + u;
                        if (P[i + step + j] >= mod)
                            P[i + step + j] -= mod;
                    } else {
                        P[i + j] = v - u;
                        if (P[i + j] < 0)
                            P[i + j] += mod;
                        P[i + step + j] = u;
                    }
                }
            }
        }
    }

    //---

    template <typename U>
    static T modPow(T x, U n) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return T(res);
    }

    static T modInv(T a) {
        return modPow(a, mod - 2);
    }

    static T normalize(T x) {
        if (x < 0)
            x += mod;
        else if (x >= mod)
            x -= mod;
        return x;
    }

    template <typename U>
    static T normalizeL(U x) {
        T res = T(x % mod);
        if (res < 0)
            res += mod;
        else if (res >= mod)
            res -= mod;
        return res;
    }
};
