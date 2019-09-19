#include <vector>
#include <algorithm>

using namespace std;

#include "polyFFTMod.h"
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

    for (int i = 0; i < 10; i++) {
        vector<int> A(1000);
        vector<int> B(1000);

        for (int i = 0; i < int(A.size()); i++)
            A[i] = RandInt32::get() % MOD;
        for (int i = 0; i < int(B.size()); i++)
            B[i] = RandInt32::get() % MOD;

        vector<int> out1 = PolyFFTMod<MOD>::multiplySlow(A, B);
        vector<int> out2 = PolyFFTMod<MOD>::multiply(A, B);
        vector<int> out3 = PolyNTT<MOD,3>::multiply(A, B);
        assert(out1 == out2);
        assert(out1 == out3);
    }

    cout << "*** Speed test ***" << endl;
    for (int n = 32; n <= 2048; n <<= 1) {
        vector<int> in1(n);
        vector<int> in2(n);
        vector<int> out;
        for (int i = 0; i < n; i++) {
            in1[i] = RandInt32::get() % 1024;
            in2[i] = RandInt32::get() % 1024;
        }

        cout << "N = " << n << endl;
        cout << "  PolyFFTMod::multiplySlow() : ";
        PROFILE_START(0);
        for (int i = 0; i < 1000; i++) {
            out = PolyFFTMod<MOD>::multiplySlow(in1, in2);
            if (out.empty())
                cerr << "It'll never be shwon!" << endl;
        }
        PROFILE_STOP(0);

        cout << "  PolyFFTMod::multiply() : ";
        PROFILE_START(1);
        for (int i = 0; i < 1000; i++) {
            out = PolyFFTMod<MOD>::multiply(in1, in2);
            if (out.empty())
                cerr << "It'll never be shwon!" << endl;
        }
        PROFILE_STOP(1);

        cout << "  PolyNTT::multiply() : ";
        PROFILE_START(2);
        for (int i = 0; i < 1000; i++) {
            out = PolyNTT<MOD,3>::multiply(in1, in2);
            if (out.empty())
                cerr << "It'll never be shwon!" << endl;
        }
        PROFILE_STOP(2);
    }

    cout << "OK!" << endl;
}
