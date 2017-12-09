#pragma once

#include "fft.h"

// High order first
template <typename T = int>
vector<T> multPoly(const vector<T>& left, const vector<T>& right) {
    vector<T> res(left.size() + right.size() - 1);

    for (int i = 0; i < (int)right.size(); i++) {
        for (int j = 0; j < (int)left.size(); j++) {
            res[i + j] += left[j] * right[i];
        }
    }

    return res;
}

// It's better performance than multPoly() when N >= 256
// High order first
template <typename T = int>
vector<T> multPolyFFT(const vector<T>& left, const vector<T>& right) {
    int sizeL = (int)left.size();
    int sizeR = (int)right.size();
    int sizeDst = sizeL + sizeR - 1;

    int size = 1;
    while (size < sizeDst)
        size <<= 1;

    vector<Complex<double>> A(size);
    vector<Complex<double>> B(size);
    for (int i = 0; i < sizeL; i++)
        A[i].first = left[i];
    for (int i = 0; i < sizeR; i++)
        B[i].first = right[i];

    fft(A);
    fft(B);

    vector<Complex<double>> C(size);
    for (int i = 0; i < size; i++)
        C[i] = A[i] * B[i];

    fft(C, true);

    vector<T> res(sizeDst);
    for (int i = 0; i < sizeDst; i++)
        res[i] = T(C[i].first + 0.5);

    return res;
}
