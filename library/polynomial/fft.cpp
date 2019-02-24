using namespace std;

#include "fft.h"
#include "fftSimple.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#define EPSILON 1e-13

bool check(const vector<Complex<double>>& l, const vector<Complex<double>>& r) {
    for (int i = 0; i < int(l.size()); i++) {
        if (fabs(l[i].first - r[i].first) >= EPSILON || fabs(l[i].second - r[i].second) >= EPSILON)
            return false;
    }
    return true;
}

void testFFT() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- FFT ------------------------" << endl;

    vector<Complex<double>> in1 = { { 4, 0 },{ 2, 0 },{ 1, 0 },{ 4, 0 }, { 6, 0 },{ 3, 0 },{ 5, 0 },{ 2, 0 } };
    vector<Complex<double>> in2 = { { 4, 0 },{ 2, 0 },{ 1, 0 },{ 4, 0 }, { 6, 0 },{ 3, 0 },{ 5, 0 },{ 2, 0 } };
    vector<Complex<double>> out1(8);
    vector<Complex<double>> out2(8);

    dft(in1, out1);
    fft(in1, out2);
    assert(check(out1, out2));

    dft(in2, out1);
    fft(in2, out2);
    assert(check(out1, out2));

    out1 = in2;
    fft(out1);
    assert(out1 == out2);

    cout << "*** Speed test ***" << endl;
    for (int n = 32; n <= 2048; n <<= 1) {
        vector<Complex<double>> in(n);
        for (int i = 0; i < n; i++) {
            in[i].first = RandInt32::get() % 1024;
            in[i].second = 0;
        }

        cout << "N = " << n << endl;
        cout << "  DFT : ";
        PROFILE_START(0);
        for (int i = 0; i < 100; i++) {
            vector<Complex<double>> out(n);
            dft(in, out);
        }
        PROFILE_STOP(0);

        cout << "  FFT : ";
        PROFILE_START(1);
        for (int i = 0; i < 100; i++) {
            vector<Complex<double>> out(n);
            fft(in, out);
        }
        PROFILE_STOP(1);
    }

    cout << "OK!" << endl;
}
