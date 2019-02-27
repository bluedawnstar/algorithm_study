#pragma once

#include "fft.h"

struct PolyFFT {
    // High order first
    static vector<int> multiplySlow(const vector<int>& left, const vector<int>& right) {
        vector<int> res(left.size() + right.size() - 1);

        for (int i = 0; i < int(right.size()); i++) {
            for (int j = 0; j < int(left.size()); j++) {
                res[i + j] += left[j] * right[i];
            }
        }

        return res;
    }

    // It's better performance than multiplySlow() when N >= 128
    // High order first
    static vector<int> multiply(const vector<int>& left, const vector<int>& right) {
        int sizeL = int(left.size());
        int sizeR = int(right.size());
        int sizeDst = sizeL + sizeR - 1;

        int size = 1;
        while (size < sizeDst)
            size <<= 1;

        vector<pair<double,double>> A(size);
        vector<pair<double,double>> B(size);
        for (int i = 0; i < sizeL; i++)
            A[i].first = left[i];
        for (int i = 0; i < sizeR; i++)
            B[i].first = right[i];

        FFT::fft(A);
        FFT::fft(B);

        vector<pair<double,double>> C(size);
        for (int i = 0; i < size; i++) {
            //C[i] = A[i] * B[i];
            C[i].first = A[i].first * B[i].first - A[i].second * B[i].second;
            C[i].second = A[i].first * B[i].second + A[i].second * B[i].first;
        }

        FFT::fft(C, true);

        vector<int> res(sizeDst);
        for (int i = 0; i < sizeDst; i++)
            res[i] = int(C[i].first + 0.5);

        return res;
    }
};
