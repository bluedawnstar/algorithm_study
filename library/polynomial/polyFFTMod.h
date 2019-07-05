#pragma once

#include "fft.h"

#ifndef SCALE
#define SCALE   32768
#endif

struct PolyFFTMod {
    static vector<int> multiplySlow(const vector<int>& left, const vector<int>& right, int mod) {
        vector<int> res(left.size() + right.size() - 1);

        for (int i = 0; i < int(right.size()); i++) {
            for (int j = 0; j < int(left.size()); j++) {
                res[i + j] = int((res[i + j] + 1ll * left[j] * right[i]) % mod);
            }
        }

        return res;
    }

    // It's better performance than multiplySlowMod() when N >= 128
    static vector<int> multiply(const vector<int>& left, const vector<int>& right, int mod) {
        int sizeL = int(left.size());
        int sizeR = int(right.size());
        int sizeDst = sizeL + sizeR - 1;

        int size = 1;
        while (size < sizeDst)
            size <<= 1;

        vector<pair<double,double>> A1(size), A2(size);
        vector<pair<double,double>> B1(size), B2(size);
        for (int i = 0; i < sizeL; i++) {
            A1[i].first = left[i] / SCALE;
            A2[i].first = left[i] % SCALE;
        }
        for (int i = 0; i < sizeR; i++) {
            B1[i].first = right[i] / SCALE;
            B2[i].first = right[i] % SCALE;
        }

        FFT::fft(A1); FFT::fft(A2);
        FFT::fft(B1); FFT::fft(B2);

        vector<int> res(sizeDst);
        return multiplyFT(A1, A2, B1, B2, res, mod);
    }

    //--- extended operations

    static vector<int> square(const vector<int>& poly, int mod) {
        int sizeDst = int(poly.size()) * 2 - 1;

        int size = 1;
        while (size < sizeDst)
            size <<= 1;

        vector<pair<double,double>> A1(size), A2(size);
        for (int i = 0; i < int(poly.size()); i++) {
            A1[i].first = poly[i] / SCALE;
            A2[i].first = poly[i] % SCALE;
        }
        FFT::fft(A1); FFT::fft(A2);

        vector<int> res(sizeDst);
        return multiplyFT(A1, A2, A1, A2, res, mod);
    }

private:
    static vector<int> multiplyFT(const vector<pair<double,double>>& A1, const vector<pair<double,double>>& A2,
        const vector<pair<double,double>>& B1, const vector<pair<double,double>>& B2,
        vector<int>& result, int mod) {
        int N = int(A1.size());
        vector<pair<double,double>> C(N);

        for (int i = 0; i < N; i++) {
            // C[i] = A1[i] * B1[i]
            C[i].first = A1[i].first * B1[i].first - A1[i].second * B1[i].second;
            C[i].second = A1[i].first * B1[i].second + A1[i].second * B1[i].first;
        }
        FFT::fft(C, true);
        for (int i = 0; i < int(result.size()); i++)
            result[i] = int(((long long)fmod(C[i].first + 0.5, mod) * SCALE * SCALE) % mod);

        for (int i = 0; i < N; i++) {
            //C[i] = A1[i] * B2[i] + A2[i] * B1[i];
            C[i].first = A1[i].first * B2[i].first - A1[i].second * B2[i].second
                + A2[i].first * B1[i].first - A2[i].second * B1[i].second;
            C[i].second = A1[i].first * B2[i].second + A1[i].second * B2[i].first
                + A2[i].first * B1[i].second + A2[i].second * B1[i].first;
        }
        FFT::fft(C, true);
        for (int i = 0; i < int(result.size()); i++)
            result[i] = int((result[i] + (long long)fmod(C[i].first + 0.5, mod) * SCALE) % mod);

        for (int i = 0; i < N; i++) {
            //C[i] = A2[i] * B2[i];
            C[i].first = A2[i].first * B2[i].first - A2[i].second * B2[i].second;
            C[i].second = A2[i].first * B2[i].second + A2[i].second * B2[i].first;
        }
        FFT::fft(C, true);
        for (int i = 0; i < int(result.size()); i++)
            result[i] = int((result[i] + (long long)fmod(C[i].first + 0.5, mod)) % mod);

        return result;
    }
};
