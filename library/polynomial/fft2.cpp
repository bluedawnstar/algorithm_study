#include <vector>
#include <algorithm>

using namespace std;

#include "dft.h"
#include "fft.h"
#include "fft2.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#define EPSILON 1e-13

static bool check(const vector<pair<double,double>>& l, const vector<pair<double,double>>& r) {
    for (int i = 0; i < int(l.size()); i++) {
        if (fabs(l[i].first - r[i].first) >= EPSILON || fabs(l[i].second - r[i].second) >= EPSILON)
            return false;
    }
    return true;
}

#define EPSILON2 1e-6

static bool check2(const vector<pair<double,double>>& l, const vector<pair<double,double>>& r) {
    for (int i = 0; i < int(l.size()); i++) {
        if (fabs(l[i].first - r[i].first) >= EPSILON2 || fabs(l[i].second - r[i].second) >= EPSILON2)
            return false;
    }
    return true;
}

void testFFT2() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Fast FFT ------------------------" << endl;

    vector<pair<double,double>> in1 = { { 4, 0 },{ 2, 0 },{ 1, 0 },{ 4, 0 }, { 6, 0 },{ 3, 0 },{ 5, 0 },{ 2, 0 } };
    vector<pair<double,double>> in2 = { { 4, 0 },{ 2, 0 },{ 1, 0 },{ 4, 0 }, { 6, 0 },{ 3, 0 },{ 5, 0 },{ 2, 0 } };
    vector<pair<double,double>> out1(8);
    vector<pair<double,double>> out2(8);

    FFT2 fft;

    DFT::dft(in1, out1);
    fft.fft(in1, out2);
    if (!check(out1, out2))
        cout << "ERROR at " << __LINE__ << endl;
    assert(check(out1, out2));

    DFT::dft(in2, out1);
    fft.fft(in2, out2);
    if (!check(out1, out2))
        cout << "ERROR at " << __LINE__ << endl;
    assert(check(out1, out2));

    out1 = in2;
    fft.fft(out1);
    if (!check(out1, out2))
        cout << "ERROR at " << __LINE__ << endl;
    assert(out1 == out2);

    cout << "*** Speed test ***" << endl;

    for (int n = 32; n <= 2048; n <<= 1) {
        vector<pair<double,double>> in(n);
        for (int i = 0; i < n; i++) {
            in[i].first = RandInt32::get() % 1024;
            in[i].second = 0;
        }

        vector<pair<double,double>> out1(n), out2(n), out3(n);

        cout << "N = " << n << endl;

        cout << "  DFT : ";
        PROFILE_START(0);
        for (int i = 0; i < 100; i++) {
            DFT::dft(in, out1);
        }
        PROFILE_STOP(0);

        cout << "  FFT : ";
        PROFILE_START(1);
        for (int i = 0; i < 100; i++) {
            FFT::fft(in, out2);
        }
        PROFILE_STOP(1);

        cout << "  FastFFT : ";
        PROFILE_START(2);
        for (int i = 0; i < 100; i++)
            fft.fft(in, out3);
        PROFILE_STOP(2);

        if (!check2(out1, out2))
            cout << "ERROR at " << __LINE__ << endl;
        if (!check2(out1, out3))
            cout << "ERROR at " << __LINE__ << endl;
        assert(check2(out1, out2) && check2(out1, out3));
    }

    cout << "OK!" << endl;
}
