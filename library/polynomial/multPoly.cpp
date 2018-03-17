using namespace std;

#include "multPoly.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testMultPoly() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Polynomial Multiplication ----------------" << endl;

    vector<int> outMult1 = multPoly(vector<int>{1, 2, 1}, vector<int>{1, 2, 1});
    vector<int> outMult2 = multPolyFFT(vector<int>{1, 2, 1}, vector<int>{1, 2, 1});
    assert(outMult1 == outMult2);

    vector<int> outMult3 = multPolyFFT(vector<int>{1, 2}, vector<int>{1, 2});
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
        cout << "  multPoly() : ";
        PROFILE_START(0);
        for (int i = 0; i < 1000; i++) {
            out = multPoly(in1, in2);
            if (out.empty())
                cerr << "It'll never be shwon!" << endl;
        }
        PROFILE_STOP(0);

        cout << "  multPolyFFT() : ";
        PROFILE_START(1);
        for (int i = 0; i < 1000; i++) {
            out = multPolyFFT(in1, in2);
            if (out.empty())
                cerr << "It'll never be shwon!" << endl;
        }
        PROFILE_STOP(1);
    }

    cout << "OK!" << endl;
}
