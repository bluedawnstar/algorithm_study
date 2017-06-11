using namespace std;

#include "fft.h"

template <typename T = int>
vector<T> convolution(const vector<T>& x, const vector<T>& h, bool reverseH = true) {
    int xN = (int)x.size();
    int hN = (int)h.size();

    vector<T> y(xN + hN - 1);

    int n = min(xN, hN);
    if (reverseH) {
        // y(n) = SUM x(t) * h(n - t)
        for (int i = 0; i < (int)y.size(); i++) {
            for (int L = i - hN + 1, R = hN - 1; R >= 0; L++, R--) {
                if (L >= 0 && L < xN)
                    y[i] += x[L] * h[R];
            }
        }
    } else {
        // y(n) = SUM x(t) * h(t)
        for (int i = 0; i < (int)y.size(); i++) {
            for (int L = i - hN + 1, R = 0; R < hN; L++, R++) {
                if (L >= 0 && L < xN)
                    y[i] += x[L] * h[R];
            }
        }
    }

    return y;
}

template <typename T = int>
vector<T> convolutionFFT(const vector<T>& x, const vector<T>& h, bool reverseH = true) {
    int sizeL = (int)x.size();
    int sizeR = (int)h.size();
    int sizeDst = sizeL + sizeR - 1;

    int size = 1;
    while (size < sizeDst)
        size <<= 1;

    vector<Complex<double>> A(size);
    vector<Complex<double>> B(size);
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

    vector<Complex<double>> C(size);
    for (int i = 0; i < size; i++)
        C[i] = A[i] * B[i];

    fft(C, true);

    vector<T> res(sizeDst);
    for (int i = 0; i < sizeDst; i++)
        res[i] = T(C[i].first + 0.5);

    return res;
}


//--- Modular Operations ------------------------------------------------------

#define SCALE   32768

vector<int> convolutionMod(const vector<int>& x, const vector<int>& h, int MOD, bool reverseH = true) {
    int xN = (int)x.size();
    int hN = (int)h.size();

    vector<int> y(xN + hN - 1);

    int n = min(xN, hN);
    if (reverseH) {
        // y(n) = SUM x(t) * h(n - t)
        for (int i = 0; i < (int)y.size(); i++) {
            for (int L = i - hN + 1, R = hN - 1; R >= 0; L++, R--) {
                if (L >= 0 && L < xN)
                    y[i] = ((long long)y[i] + (long long)x[L] * h[R]) % MOD;
            }
        }
    } else {
        // y(n) = SUM x(t) * h(t)
        for (int i = 0; i < (int)y.size(); i++) {
            for (int L = i - hN + 1, R = 0; R < hN; L++, R++) {
                if (L >= 0 && L < xN)
                    y[i] = ((long long)y[i] + (long long)x[L] * h[R]) % MOD;
            }
        }
    }

    return y;
}

vector<int> convolutionFFTMod(const vector<int>& x, const vector<int>& h, int MOD, bool reverseH = true) {
    int sizeL = (int)x.size();
    int sizeR = (int)h.size();
    int sizeDst = sizeL + sizeR - 1;

    int size = 1;
    while (size < sizeDst)
        size <<= 1;

    vector<Complex<double>> A1(size), A2(size);
    vector<Complex<double>> B1(size), B2(size);
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
    vector<Complex<double>> C(size);

    for (int i = 0; i < size; i++)
        C[i] = A1[i] * B1[i];
    fft(C, true);
    for (int i = 0; i < sizeDst; i++)
        res[i] = ((long long)fmod(C[i].first + 0.5, MOD) * SCALE * SCALE) % MOD;

    for (int i = 0; i < size; i++)
        C[i] = A1[i] * B2[i] + A2[i] * B1[i];
    fft(C, true);
    for (int i = 0; i < sizeDst; i++)
        res[i] = (res[i] + (long long)fmod(C[i].first + 0.5, MOD) * SCALE) % MOD;

    for (int i = 0; i < size; i++)
        C[i] = A2[i] * B2[i];
    fft(C, true);
    for (int i = 0; i < sizeDst; i++)
        res[i] = (res[i] + (long long)fmod(C[i].first + 0.5, MOD)) % MOD;

    return res;
}


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#define MOD     1000000007

void testConvolution() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Convolution ------------------------------" << endl;

    vector<int> x{ 1, 2, 3 };
    vector<int> h{ 1, 2 };

    vector<int> v = convolution(x, h);
    cout << x << " * " << h << " : ";
    cout << v << endl;
    assert(operator ==(v, vector<int>{ 1, 4, 7, 6 }));

    vector<int> v2 = convolutionFFT(x, h);
    cout << x << " * " << h << " : ";
    cout << v2 << endl;
    assert(v == v2);

    v = convolution(x, h, false);
    cout << x << " * " << h << " : ";
    cout << v << endl;
    assert(operator ==(v, vector<int>{ 2, 5, 8, 3 }));

    v2 = convolutionFFT(x, h, false);
    cout << x << " * " << h << " : ";
    cout << v2 << endl;
    assert(v == v2);

    v = convolutionMod(x, h, MOD);
    cout << x << " * " << h << " : ";
    cout << v << endl;
    assert(operator ==(v, vector<int>{ 1, 4, 7, 6 }));

    v2 = convolutionFFTMod(x, h, MOD);
    cout << x << " * " << h << " : ";
    cout << v2 << endl;
    assert(v == v2);

    v = convolutionMod(x, h, MOD, false);
    cout << x << " * " << h << " : ";
    cout << v << endl;
    assert(operator ==(v, vector<int>{ 2, 5, 8, 3 }));

    v2 = convolutionFFTMod(x, h, MOD, false);
    cout << x << " * " << h << " : ";
    cout << v2 << endl;
    assert(v == v2);

    cout << "OK!" << endl;
}
