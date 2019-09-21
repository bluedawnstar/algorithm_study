#pragma once

#include "fft2.h"

struct Convolution2 : public FFT2 {
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
                        y[i] += x[L] * h[R];
                }
            }
        } else {
            // y(n) = SUM x(t) * h(t)
            for (int i = 0; i < int(y.size()); i++) {
                for (int L = i - hN + 1, R = 0; R < hN; L++, R++) {
                    if (L >= 0 && L < xN)
                        y[i] += x[L] * h[R];
                }
            }
        }

        return y;
    }

    // It's better performance than convoluteSlow() when N >= 128
    vector<int> convolute(const vector<int>& x, const vector<int>& h, bool reverseH = true) {
        int sizeL = int(x.size());
        int sizeR = int(h.size());
        int sizeDst = sizeL + sizeR - 1;

        int size = 1;
        while (size < sizeDst)
            size <<= 1;

        vector<pair<double,double>> A(size);
        vector<pair<double,double>> B(size);
        for (int i = 0; i < sizeL; i++)
            A[i].first = x[i];
        if (reverseH) {
            for (int i = 0; i < sizeR; i++)
                B[i].first = h[i];
        } else {
            for (int i = 0, j = sizeR - 1; j >= 0; i++, j--)
                B[i].first = h[j];
        }

        fft(A);
        fft(B);

        vector<pair<double,double>> C(size);
        for (int i = 0; i < size; i++) {
            //C[i] = A[i] * B[i];
            C[i].first = A[i].first * B[i].first - A[i].second * B[i].second;
            C[i].second = A[i].first * B[i].second + A[i].second * B[i].first;
        }

        fft(C, true);

        vector<int> res(sizeDst);
        for (int i = 0; i < sizeDst; i++)
            res[i] = int(C[i].first + 0.5);

        return res;
    }
};
