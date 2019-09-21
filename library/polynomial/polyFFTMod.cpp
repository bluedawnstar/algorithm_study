#include <vector>
#include <algorithm>

using namespace std;

#include "polyFFTMod.h"
#include "polyFFTMod2.h"
#include "polyNTT.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#define MOD     1000000007

void testPolyFFTMod() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Modular Polynomial Multiplication ----------------" << endl;

    PolyFFTMod2<MOD> fft2;
    for (int i = 0; i < 10; i++) {
        vector<int> A(1000);
        vector<int> B(1000);

        for (int i = 0; i < int(A.size()); i++)
            A[i] = RandInt32::get() % MOD;
        for (int i = 0; i < int(B.size()); i++)
            B[i] = RandInt32::get() % MOD;

        vector<int> out1 = PolyFFTMod<MOD>::multiplySlow(A, B);
        vector<int> out2 = PolyFFTMod<MOD>::multiply(A, B);
        vector<int> out3 = fft2.multiply(A, B);
        vector<int> out4 = PolyNTT<MOD,3>::multiply(A, B);

        if (out1 != out2 || out1 != out3 || out1 != out4)
            cout << "ERROR at " << __LINE__ << endl;

        assert(out1 == out2);
        assert(out1 == out3);
        assert(out1 == out4);
    }

    cout << "*** Speed test ***" << endl;
    for (int n = 32; n <= 2048; n <<= 1) {
        vector<int> in1(n);
        vector<int> in2(n);
        for (int i = 0; i < n; i++) {
            in1[i] = RandInt32::get() % 1024;
            in2[i] = RandInt32::get() % 1024;
        }

        vector<int> out1, out2, out3, out4;

        cout << "N = " << n << endl;

        cout << "  PolyFFTMod::multiplySlow() : ";
        PROFILE_START(0);
        for (int i = 0; i < 1000; i++)
            out1 = PolyFFTMod<MOD>::multiplySlow(in1, in2);
        PROFILE_STOP(0);

        cout << "  PolyFFTMod::multiply() : ";
        PROFILE_START(1);
        for (int i = 0; i < 1000; i++)
            out2 = PolyFFTMod<MOD>::multiply(in1, in2);
        PROFILE_STOP(1);

        cout << "  PolyFFTMod2::multiply() : ";
        PROFILE_START(2);
        for (int i = 0; i < 1000; i++)
            out3 = fft2.multiply(in1, in2);
        PROFILE_STOP(2);

        cout << "  PolyNTT::multiply() : ";
        PROFILE_START(3);
        for (int i = 0; i < 1000; i++)
            out4 = PolyNTT<MOD,3>::multiply(in1, in2);
        PROFILE_STOP(3);

        if (out1 != out2 || out1 != out3 || out1 != out4)
            cout << "ERROR at " << __LINE__ << endl;

        assert(out1 == out2);
        assert(out1 == out3);
        assert(out1 == out4);
    }

    cout << "OK!" << endl;
}
