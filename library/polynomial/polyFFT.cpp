#include <vector>
#include <algorithm>

using namespace std;

#include "polyFFT.h"
#include "polyFFT2.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testPolyFFT() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Polynomial Multiplication ----------------" << endl;

    PolyFFT2 fft2;

    vector<int> outMult1 = PolyFFT::multiplySlow(vector<int>{1, 2, 1}, vector<int>{1, 2, 1});
    vector<int> outMult2 = PolyFFT::multiply(vector<int>{1, 2, 1}, vector<int>{1, 2, 1});
    vector<int> outMult3 = fft2.multiply(vector<int>{1, 2, 1}, vector<int>{1, 2, 1});

    if (outMult1 != outMult2 || outMult1 != outMult3)
        cout << "ERROR at " << __LINE__ << endl;
    assert(outMult1 == outMult2);
    assert(outMult1 == outMult3);

    vector<int> outMult4 = PolyFFT::multiply(vector<int>{1, 2}, vector<int>{1, 2});
    cout << "(x + 2) * (x + 2) = " << outMult4 << endl;

    vector<int> outMult5 = fft2.multiply(vector<int>{1, 2}, vector<int>{1, 2});
    cout << "(x + 2) * (x + 2) = " << outMult5 << endl;

    cout << "*** Speed test ***" << endl;
    for (int n = 32; n <= 2048; n <<= 1) {
        vector<int> in1(n);
        vector<int> in2(n);
        for (int i = 0; i < n; i++) {
            in1[i] = RandInt32::get() % 1024;
            in2[i] = RandInt32::get() % 1024;
        }

        vector<int> out1, out2, out3;

        cout << "N = " << n << endl;

        cout << "  PolyFFT::multiplySlow() : ";
        PROFILE_START(0);
        for (int i = 0; i < 1000; i++)
            out1 = PolyFFT::multiplySlow(in1, in2);
        PROFILE_STOP(0);

        cout << "  PolyFFT::multiply() : ";
        PROFILE_START(1);
        for (int i = 0; i < 1000; i++)
            out2 = PolyFFT::multiply(in1, in2);
        PROFILE_STOP(1);

        cout << "  PolyFFT2::multiply() : ";
        PROFILE_START(2);
        for (int i = 0; i < 1000; i++)
            out3 = fft2.multiply(in1, in2);
        PROFILE_STOP(2);

        if (out1 != out2 || out1 != out3)
            cout << "ERROR at " << __LINE__ << endl;
        assert(out1 == out2 && out1 == out3);
    }

    cout << "OK!" << endl;
}
