#include <vector>
#include <algorithm>

using namespace std;

#include "convolution.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#define MOD     1000000007

void testConvolution() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Convolution ------------------------------" << endl;

    vector<int> x{ 1, 2, 3 };
    vector<int> h{ 1, 2 };

    vector<int> v = Convolution::multiply(x, h);
    cout << x << " * " << h << " : ";
    cout << v << endl;
    assert(operator ==(v, vector<int>{ 1, 4, 7, 6 }));

    vector<int> v2 = Convolution::multiplyFFT(x, h);
    cout << x << " * " << h << " : ";
    cout << v2 << endl;
    assert(v == v2);

    v = Convolution::multiply(x, h, false);
    cout << x << " * " << h << " : ";
    cout << v << endl;
    assert(operator ==(v, vector<int>{ 2, 5, 8, 3 }));

    v2 = Convolution::multiplyFFT(x, h, false);
    cout << x << " * " << h << " : ";
    cout << v2 << endl;
    assert(v == v2);

    v = Convolution::multiplyMod(x, h, MOD);
    cout << x << " * " << h << " : ";
    cout << v << endl;
    assert(operator ==(v, vector<int>{ 1, 4, 7, 6 }));

    v2 = Convolution::multiplyFFTMod(x, h, MOD);
    cout << x << " * " << h << " : ";
    cout << v2 << endl;
    assert(v == v2);

    v = Convolution::multiplyMod(x, h, MOD, false);
    cout << x << " * " << h << " : ";
    cout << v << endl;
    assert(operator ==(v, vector<int>{ 2, 5, 8, 3 }));

    v2 = Convolution::multiplyFFTMod(x, h, MOD, false);
    cout << x << " * " << h << " : ";
    cout << v2 << endl;
    assert(v == v2);

    cout << "*** Speed test ***" << endl;
    for (int n = 32; n <= 2048; n <<= 1) {
        vector<int> in1(n);
        vector<int> in2(n);
        vector<int> out;
        for (int i = 0; i < n; i++) {
            in1[i] = RandInt32().get() % 1024;
            in2[i] = RandInt32().get() % 1024;
        }

        cout << "N = " << n << endl;
        cout << "  convolution() : ";
        PROFILE_START(0);
        for (int i = 0; i < 1000; i++) {
            out = Convolution::multiply(in1, in2);
            if (out.empty())
                cerr << "It'll never be shwon!" << endl;
        }
        PROFILE_STOP(0);
        cout << "  convolutionFFT() : ";
        PROFILE_START(1);
        for (int i = 0; i < 1000; i++) {
            out = Convolution::multiplyFFT(in1, in2);
            if (out.empty())
                cerr << "It'll never be shwon!" << endl;
        }
        PROFILE_STOP(1);

        cout << "  convolutionMod() : ";
        PROFILE_START(2);
        for (int i = 0; i < 1000; i++) {
            out = Convolution::multiplyMod(in1, in2, MOD);
            if (out.empty())
                cerr << "It'll never be shwon!" << endl;
        }
        PROFILE_STOP(2);

        cout << "  convolutionFFTMod() : ";
        PROFILE_START(3);
        for (int i = 0; i < 1000; i++) {
            out = Convolution::multiplyFFTMod(in1, in2, MOD);
            if (out.empty())
                cerr << "It'll never be shwon!" << endl;
        }
        PROFILE_STOP(3);
    }

    cout << "OK!" << endl;
}
