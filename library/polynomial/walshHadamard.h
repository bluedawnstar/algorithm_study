#pragma once

// Fast Walsh-Hadamard transform
//  https://csacademy.com/blog/fast-fourier-transform-and-variations-of-it/
//  https://en.wikipedia.org/wiki/Fast_Walsh%E2%80%93Hadamard_transform

// Fast Walsh-Hadamard transform

template <typename T>
struct FWHT {
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
            tA[i] *= tB[i];

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
            tA[i] *= tB[i];

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
            tA[i] *= tB[i];

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
                    P[i + j] = u + v;
                    P[i + step + j] = u - v;
                }
            }
        }

        if (inverse) {
            for (int i = 0; i < int(P.size()); i++)
                P[i] /= N;
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
                        P[i + step + j] = u;
                    } else {
                        P[i + j] = v;
                        P[i + step + j] = u - v;
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
                    } else {
                        P[i + j] = v - u;
                        P[i + step + j] = u;
                    }
                }
            }
        }
    }
};
