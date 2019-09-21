#pragma once

#include "fft2.h"

template <int mod>
struct ConvolutionMod2 : public FFT2 {
    static const int SCALE = 32768;

    vector<int> convoluteSlow(const vector<int>& x, const vector<int>& h, bool reverseH = true) {
        int xN = int(x.size());
        int hN = int(h.size());

        vector<int> y(xN + hN - 1);

        int n = min(xN, hN);
        if (reverseH) {
            // y(n) = SUM x(t) * h(n - t)
            for (int i = 0; i < int(y.size()); i++) {
                for (int L = i - hN + 1, R = hN - 1; R >= 0; L++, R--) {
                    if (L >= 0 && L < xN)
                        y[i] = ((long long)y[i] + (long long)x[L] * h[R]) % mod;
                }
            }
        } else {
            // y(n) = SUM x(t) * h(t)
            for (int i = 0; i < int(y.size()); i++) {
                for (int L = i - hN + 1, R = 0; R < hN; L++, R++) {
                    if (L >= 0 && L < xN)
                        y[i] = ((long long)y[i] + (long long)x[L] * h[R]) % mod;
                }
            }
        }

        return y;
    }

    // It's better performance than multiplySlow() when N >= 64
    vector<int> convolute(const vector<int>& x, const vector<int>& h, bool reverseH = true) {
        int sizeL = int(x.size());
        int sizeR = int(h.size());
        int sizeDst = sizeL + sizeR - 1;

        int size = 1;
        while (size < sizeDst)
            size <<= 1;

        vector<pair<double,double>> A1(size), A2(size);
        vector<pair<double,double>> B1(size), B2(size);
        for (int i = 0; i < sizeL; i++) {
            A1[i].first = x[i] / SCALE;
            A2[i].first = x[i] % SCALE;
        }
        if (reverseH) {
            for (int i = 0; i < sizeR; i++) {
                B1[i].first = h[i] / SCALE;
                B2[i].first = h[i] % SCALE;
            }
        } else {
            for (int i = 0, j = sizeR - 1; j >= 0; i++, j--) {
                B1[i].first = h[j] / SCALE;
                B2[i].first = h[j] % SCALE;
            }
        }

        fft(A1); fft(A2);
        fft(B1); fft(B2);

        vector<int> res(sizeDst);
        vector<pair<double,double>> C(size);

        for (int i = 0; i < size; i++) {
            //C[i] = A1[i] * B1[i];
            C[i].first = A1[i].first * B1[i].first - A1[i].second * B1[i].second;
            C[i].second = A1[i].first * B1[i].second + A1[i].second * B1[i].first;
        }
        fft(C, true);
        for (int i = 0; i < sizeDst; i++)
            res[i] = ((long long)fmod(C[i].first + 0.5, mod) * SCALE * SCALE) % mod;

        for (int i = 0; i < size; i++) {
            //C[i] = A1[i] * B2[i] + A2[i] * B1[i];
            C[i].first = A1[i].first * B2[i].first - A1[i].second * B2[i].second
                + A2[i].first * B1[i].first - A2[i].second * B1[i].second;
            C[i].second = A1[i].first * B2[i].second + A1[i].second * B2[i].first
                + A2[i].first * B1[i].second + A2[i].second * B1[i].first;
        }
        fft(C, true);
        for (int i = 0; i < sizeDst; i++)
            res[i] = (res[i] + (long long)fmod(C[i].first + 0.5, mod) * SCALE) % mod;

        for (int i = 0; i < size; i++) {
            //C[i] = A2[i] * B2[i];
            C[i].first = A2[i].first * B2[i].first - A2[i].second * B2[i].second;
            C[i].second = A2[i].first * B2[i].second + A2[i].second * B2[i].first;
        }
        fft(C, true);
        for (int i = 0; i < sizeDst; i++)
            res[i] = (res[i] + (long long)fmod(C[i].first + 0.5, mod)) % mod;

        return res;
    }
};
