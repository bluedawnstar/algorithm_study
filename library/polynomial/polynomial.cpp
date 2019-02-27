#include <vector>
#include <algorithm>

using namespace std;

#include "polynomial.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testPolynomial() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Polynomial Multiplication ----------------" << endl;

    vector<int> outMult1 = PolyFFT::multiplySlow(vector<int>{1, 2, 1}, vector<int>{1, 2, 1});
    vector<int> outMult2 = PolyFFT::multiply(vector<int>{1, 2, 1}, vector<int>{1, 2, 1});
    assert(outMult1 == outMult2);

    vector<int> outMult3 = PolyFFT::multiply(vector<int>{1, 2}, vector<int>{1, 2});
    cout << "(x + 2) * (x + 2) = " << outMult3 << endl;

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
        cout << "  PolyFFT::multiplySlow() : ";
        PROFILE_START(0);
        for (int i = 0; i < 1000; i++) {
            out = PolyFFT::multiplySlow(in1, in2);
            if (out.empty())
                cerr << "It'll never be shwon!" << endl;
        }
        PROFILE_STOP(0);

        cout << "  PolyFFT::multiply() : ";
        PROFILE_START(1);
        for (int i = 0; i < 1000; i++) {
            out = PolyFFT::multiply(in1, in2);
            if (out.empty())
                cerr << "It'll never be shwon!" << endl;
        }
        PROFILE_STOP(1);
    }

    cout << "OK!" << endl;
}
