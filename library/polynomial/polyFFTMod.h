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

        if (sizeDst <= 256)
            return multiplySlow(left, right, mod);

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
        if (poly.size() <= 128)
            return multiplySlow(poly, poly, mod);

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

    // low order first
    static vector<int> inverse(vector<int> a, int mod) {
        //assert(!a.empty());
        int n = int(a.size());
        vector<int> b = { modInv(a[0], mod) };
        while (int(b.size()) < n) {
            vector<int> a_cut(a.begin(), a.begin() + min(a.size(), b.size() << 1));
            vector<int> x = multiply(square(b, mod), a_cut, mod);
            b.resize(b.size() << 1);
            for (int i = int(b.size()) >> 1; i < int(min(x.size(), b.size())); i++)
                b[i] = mod - x[i];
        }
        b.resize(n);
        return b;
    }

    // low order first
    static vector<int> differentiate(vector<int> a, int mod) {
        a.back() = 0;
        for(int i = 1; i < int(a.size()); i++)
            a[i - 1] = int(1ll * a[i] * i % mod);  
        return a;
    }

    // low order first
    static vector<int> integrate(vector<int> a, int mod) {
        for(int i = int(a.size()) - 1; i > 0; i--)
            a[i] = int(1ll * a[i - 1] * modInv(i, mod) % mod);
        a[0] = 0;  
        return a;
    }

    // ln f(x) = INTEGRAL f'(x) / f(x)
    // low order first
    static vector<int> ln(vector<int> a, int mod) {
        auto A = inverse(a, mod);
        auto B = differentiate(a, mod);
        A = multiply(A, B, mod);
        A.resize(a.size());
        return integrate(A, mod);  
    }

    // low order first
    static vector<int> exp(vector<int> a, int mod) {
        int size = 1;
        while (size < int(a.size()))
            size <<= 1;

        if(size == 1)
            return{ 1 };

        a.resize(size);

        vector<int> dd(a.begin(), a.begin() + (size >> 1));

        vector<int> b = exp(dd, mod);
        b.resize(size);

        vector<int> c = ln(b, mod);
        for (int i = 0; i < size; i++)
            c[i] = int((a[i] - c[i] + mod) % mod);
        c[0]++;

        b = multiply(b, c, mod);
        b.resize(size);

        return b;
    }

    // a[0] != 0
    static vector<int> powFast(const vector<int>& a, int n, int mod) { // n >= 0
        auto b = ln(a, mod);
        for (int i = 0; i < int(b.size()); i++)
            b[i]= int(1ll * b[i] * n % mod);
        return exp(b, mod);
    }

    static vector<int> pow(const vector<int>& p, int n, int maxDegree, int mod) {
        if (n == 0)
            return{ 1 };

        auto poly = pow(p, n / 2, maxDegree, mod);
        poly = square(poly, mod);
        if (int(poly.size()) > maxDegree + 1)
            poly.resize(maxDegree + 1);

        if (n & 1) {
            poly = multiply(poly, p, mod);
            if (int(poly.size()) > maxDegree + 1)
                poly.resize(maxDegree + 1);
        }

        return poly;
    }

//private:
    static int modPow(int x, int n, int mod) {
        if (n == 0)
            return 1;

        int p = modPow(x, n / 2, mod) % mod;
        p = int(1ll * p * p % mod);

        return ((n & 1) == 0) ? p : int(1ll * p * x % mod);
    }

    static int modInv(int a, int mod) {
        return modPow(a, mod - 2, mod);
    }

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
