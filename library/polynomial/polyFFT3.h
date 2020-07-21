#pragma once

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

template <int MaxN = (1 << 20)>
struct PolyFFT3 {
    static pair<double, double> w[MaxN];
    static bool initialized;

    static void init() {
        if (!initialized) {
            for (int i = 1; i < MaxN; i <<= 1) {
                for (int j = 0; j < i; j++) {
                    w[i + j].first = cos(M_PI * j / i);
                    w[i + j].second = sin(M_PI * j / i);
                }
            }
            initialized = true;
        }
    }

    static void fft(const pair<double, double>* in, pair<double, double>* out, int n, int k = 1) {
        if (n == 1) {
            *out = *in;
        } else {
            n >>= 1;
            fft(in, out, n, k << 1);
            fft(in + k, out + n, n, k << 1);
            for (int i = 0; i < n; i++) {
                pair<double, double> t(out[i + n].first * w[i + n].first - out[i + n].second * w[i + n].second,
                    out[i + n].second * w[i + n].first + out[i + n].first * w[i + n].second);

                out[i + n].first = out[i].first - t.first;
                out[i + n].second = out[i].second - t.second;
                out[i].first += t.first;
                out[i].second += t.second;
            }
        }
    }

    static void fftInv(const pair<double, double>* in, pair<double, double>* out, int n, int k = 1) {
        if (n == 1) {
            *out = *in;
        } else {
            n >>= 1;
            fftInv(in, out, n, k << 1);
            fftInv(in + k, out + n, n, k << 1);
            for (int i = 0; i < n; i++) {
                pair<double, double> t(out[i + n].first * w[i + n].first + out[i + n].second * w[i + n].second,
                    out[i + n].second * w[i + n].first - out[i + n].first * w[i + n].second);

                out[i + n].first = out[i].first - t.first;
                out[i + n].second = out[i].second - t.second;
                out[i].first += t.first;
                out[i].second += t.second;
            }
        }

        if (k == 1) {
            for (int i = 0; i < n; i++) {
                out[i].first /= n;
                out[i].second /= n;
            }
        }
    }

    //---

    static pair<double, double> A[MaxN], B[MaxN];
    static pair<double, double> C[MaxN], D[MaxN];

    // low order first
    static vector<int> multiplySlow(const vector<int>& a, const vector<int>& b) {
        vector<int> res(a.size() + b.size() - 1);
        for (int i = 0; i < int(a.size()); i++) {
            for (int j = 0; j < int(b.size()); j++) {
                res[i + j] = res[i + j] + a[i] * b[j];
            }
        }
        return res;
    }

    // low order first
    static vector<int> multiply(const vector<int>& left, const vector<int>& right) {
        int sizeL = int(left.size());
        int sizeR = int(right.size());
        int sizeDst = sizeL + sizeR - 1;

        if (min(sizeL, sizeR) < 256)
            return multiplySlow(left, right);

        init();

        static const int shift = 15;
        static const int mask = (1 << shift) - 1;

        int size = 1;
        while (size < sizeDst)
            size <<= 1;
        int sizeMask = size - 1;

        for (int i = 0; i < size; i++) {
            if (i < sizeL) {
                A[i].first = left[i];
                A[i].second = 0;
            } else {
                A[i].first = 0;
                A[i].second = 0;
            }
            if (i < sizeR) {
                B[i].first = right[i];
                B[i].second = 0;
            } else {
                B[i].first = 0;
                B[i].second = 0;
            }
        }

        fft(A, C, size);
        fft(B, D, size);
        for (int i = 0; i < size; i++) {
            //C[i] = A[i] * B[i];
            A[i].first = C[i].first * D[i].first - C[i].second * D[i].second;
            A[i].second = C[i].first * D[i].second + C[i].second * D[i].first;
        }

        fftInv(A, C, size);

        vector<int> res(sizeDst);
        for (int i = 0; i < sizeDst; i++)
            res[i] = int(C[i].first + 0.5);

        return res;
    }
};

template <int MaxN>
pair<double, double> PolyFFT3<MaxN>::w[MaxN];

template <int MaxN>
bool PolyFFT3<MaxN>::initialized = false;

template <int MaxN>
pair<double, double> PolyFFT3<MaxN>::A[MaxN];

template <int MaxN>
pair<double, double> PolyFFT3<MaxN>::B[MaxN];

template <int MaxN>
pair<double, double> PolyFFT3<MaxN>::C[MaxN];

template <int MaxN>
pair<double, double> PolyFFT3<MaxN>::D[MaxN];
