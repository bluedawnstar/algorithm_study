#include <vector>
#include <algorithm>

using namespace std;

#include "convolution.h"
#include "convolutionMod.h"
#include "convolution2.h"
#include "convolutionMod2.h"

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
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Convolution ------------------------------" << endl;

    Convolution conv;
    ConvolutionMod<MOD> convMod;

    {
        vector<int> x{ 1, 2, 3 };
        vector<int> h{ 1, 2 };

        vector<int> v = Convolution::convoluteSlow(x, h);
        cout << x << " * " << h << " : ";
        cout << v << endl;
        if (!operator ==(v, vector<int>{ 1, 4, 7, 6 }))
            cout << "ERROR at " << __LINE__ << endl;
        assert(operator ==(v, vector<int>{ 1, 4, 7, 6 }));

        vector<int> v2 = Convolution::convolute(x, h);
        cout << x << " * " << h << " : ";
        cout << v2 << endl;
        if (!(v == v2))
            cout << "ERROR at " << __LINE__ << endl;
        assert(v == v2);

        v = Convolution::convoluteSlow(x, h, false);
        cout << x << " * " << h << " : ";
        cout << v << endl;
        if (!operator ==(v, vector<int>{ 2, 5, 8, 3 }))
            cout << "ERROR at " << __LINE__ << endl;
        assert(operator ==(v, vector<int>{ 2, 5, 8, 3 }));

        v2 = Convolution::convolute(x, h, false);
        cout << x << " * " << h << " : ";
        cout << v2 << endl;
        if (v != v2)
            cout << "ERROR at " << __LINE__ << endl;
        assert(v == v2);

        v = ConvolutionMod<MOD>::convoluteSlow(x, h);
        cout << x << " * " << h << " : ";
        cout << v << endl;
        if (!operator ==(v, vector<int>{ 1, 4, 7, 6 }))
            cout << "ERROR at " << __LINE__ << endl;
        assert(operator ==(v, vector<int>{ 1, 4, 7, 6 }));

        v2 = ConvolutionMod<MOD>::convolute(x, h);
        cout << x << " * " << h << " : ";
        cout << v2 << endl;
        if (!(v == v2))
            cout << "ERROR at " << __LINE__ << endl;
        assert(v == v2);

        v = ConvolutionMod<MOD>::convoluteSlow(x, h, false);
        cout << x << " * " << h << " : ";
        cout << v << endl;
        if (!operator ==(v, vector<int>{ 2, 5, 8, 3 }))
            cout << "ERROR at " << __LINE__ << endl;
        assert(operator ==(v, vector<int>{ 2, 5, 8, 3 }));

        v2 = ConvolutionMod<MOD>::convolute(x, h, false);
        cout << x << " * " << h << " : ";
        cout << v2 << endl;
        if (!(v == v2))
            cout << "ERROR at " << __LINE__ << endl;
        assert(v == v2);
    }
    {
        vector<int> x{ 1, 2, 3 };
        vector<int> h{ 1, 2 };

        vector<int> v = conv.convoluteSlow(x, h);
        cout << x << " * " << h << " : ";
        cout << v << endl;
        if (!operator ==(v, vector<int>{ 1, 4, 7, 6 }))
            cout << "ERROR at " << __LINE__ << endl;
        assert(operator ==(v, vector<int>{ 1, 4, 7, 6 }));

        vector<int> v2 = conv.convolute(x, h);
        cout << x << " * " << h << " : ";
        cout << v2 << endl;
        if (!(v == v2))
            cout << "ERROR at " << __LINE__ << endl;
        assert(v == v2);

        v = conv.convoluteSlow(x, h, false);
        cout << x << " * " << h << " : ";
        cout << v << endl;
        if (!operator ==(v, vector<int>{ 2, 5, 8, 3 }))
            cout << "ERROR at " << __LINE__ << endl;
        assert(operator ==(v, vector<int>{ 2, 5, 8, 3 }));

        v2 = conv.convolute(x, h, false);
        cout << x << " * " << h << " : ";
        cout << v2 << endl;
        if (v != v2)
            cout << "ERROR at " << __LINE__ << endl;
        assert(v == v2);

        v = convMod.convoluteSlow(x, h);
        cout << x << " * " << h << " : ";
        cout << v << endl;
        if (!operator ==(v, vector<int>{ 1, 4, 7, 6 }))
            cout << "ERROR at " << __LINE__ << endl;
        assert(operator ==(v, vector<int>{ 1, 4, 7, 6 }));

        v2 = convMod.convolute(x, h);
        cout << x << " * " << h << " : ";
        cout << v2 << endl;
        if (!(v == v2))
            cout << "ERROR at " << __LINE__ << endl;
        assert(v == v2);

        v = convMod.convoluteSlow(x, h, false);
        cout << x << " * " << h << " : ";
        cout << v << endl;
        if (!operator ==(v, vector<int>{ 2, 5, 8, 3 }))
            cout << "ERROR at " << __LINE__ << endl;
        assert(operator ==(v, vector<int>{ 2, 5, 8, 3 }));

        v2 = convMod.convolute(x, h, false);
        cout << x << " * " << h << " : ";
        cout << v2 << endl;
        if (!(v == v2))
            cout << "ERROR at " << __LINE__ << endl;
        assert(v == v2);
    }
    cout << "*** Speed test ***" << endl;
    {
        for (int n = 32; n <= 2048; n <<= 1) {
            vector<int> in1(n);
            vector<int> in2(n);
            for (int i = 0; i < n; i++) {
                in1[i] = RandInt32().get() % 1024;
                in2[i] = RandInt32().get() % 1024;
            }

            vector<int> out1, out2, out3, out4, out5, out6;

            cout << "N = " << n << endl;

            cout << "  convolution::convoluteSlow() : ";
            PROFILE_START(0);
            for (int i = 0; i < 1000; i++)
                out1 = Convolution::convoluteSlow(in1, in2);
            PROFILE_STOP(0);

            cout << "  convolution::convolute() : ";
            PROFILE_START(1);
            for (int i = 0; i < 1000; i++)
                out2 = Convolution::convolute(in1, in2);
            PROFILE_STOP(1);

            cout << "  convolution2::convolute() : ";
            PROFILE_START(2);
            for (int i = 0; i < 1000; i++)
                out3 = conv.convolute(in1, in2);
            PROFILE_STOP(2);

            cout << "  convolutionMod::convoluteSlow() : ";
            PROFILE_START(3);
            for (int i = 0; i < 1000; i++)
                out4 = ConvolutionMod<MOD>::convoluteSlow(in1, in2);
            PROFILE_STOP(3);

            cout << "  convolutionMod::convolute() : ";
            PROFILE_START(4);
            for (int i = 0; i < 1000; i++)
                out5 = ConvolutionMod<MOD>::convolute(in1, in2);
            PROFILE_STOP(4);

            cout << "  convolutionMod2::convolute() : ";
            PROFILE_START(5);
            for (int i = 0; i < 1000; i++)
                out6 = convMod.convolute(in1, in2);
            PROFILE_STOP(5);

            if (out1 != out2 || out1 != out3)
                cout << "ERROR at " << __LINE__ << endl;
            if (out4 != out5 || out4 != out6)
                cout << "ERROR at " << __LINE__ << endl;
            assert(out1 == out2 && out1 == out3);
            assert(out4 == out5 && out4 == out6);
        }
    }

    cout << "OK!" << endl;
}
