#include <vector>

using namespace std;

#include "ntt.h"
#include "polynomialNTT.h"
#include "factorialNTT.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "polynomialMod.h"

#define MOD     998244353

void testNTT() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- NTT ------------------------" << endl;
    {
        NTT ntt(MOD, 3);
        int TESTN = 1000;
        for (int i = 0; i < 10; i++) {
            vector<int> A(TESTN);
            vector<int> B(TESTN);

            for (int i = 0; i < int(A.size()); i++)
                A[i] = RandInt32::get() % MOD;
            for (int i = 0; i < int(B.size()); i++)
                B[i] = RandInt32::get() % MOD;

            vector<int> out1 = PolyFFTMod::multiply(A, B, MOD);
            vector<int> out2 = ntt.multiply(A, B);
            if (out1 != out2) {
                cout << "Mismatched : " << endl;
                cout << out1 << endl;
                cout << out2 << endl;
            }
            assert(out1 == out2);
        }
    }
    {
        NTT ntt(MOD, 3);
        int TESTN = 1000;
        for (int i = 0; i < 10; i++) {
            vector<int> A(TESTN);
            vector<int> B(TESTN);

            for (int i = 0; i < int(A.size()); i++)
                A[i] = RandInt32::get() % MOD;
            for (int i = 0; i < int(B.size()); i++)
                B[i] = RandInt32::get() % MOD;

            vector<int> out1 = PolyFFTMod::multiply(A, B, MOD);
            vector<int> out2 = ntt.multiply(A, B);
            if (out1 != out2) {
                cout << "Mismatched : " << endl;
                cout << out1 << endl;
                cout << out2 << endl;
            }
            assert(out1 == out2);
        }
    }
    {
        static const int M = 1000000007;
        int TESTN = 1000;
        for (int i = 0; i < 10; i++) {
            vector<int> A(TESTN);
            vector<int> B(TESTN);

            for (int i = 0; i < int(A.size()); i++)
                A[i] = RandInt32::get() % M;
            for (int i = 0; i < int(B.size()); i++)
                B[i] = RandInt32::get() % M;

            vector<int> out1 = PolyFFTMod::multiply(A, B, M);
            vector<int> out2 = PolyNTT::multiply(A, B, M);
            if (out1 != out2) {
                cout << "Mismatched : " << endl;
                cout << out1 << endl;
                cout << out2 << endl;
            }
            assert(out1 == out2);
        }
    }
    {
        static const int M = 1000000007;
        int TESTN = 1000;
        for (int i = 0; i < 10; i++) {
            vector<int> A(TESTN);
            vector<int> B(TESTN);

            for (int i = 0; i < int(A.size()); i++)
                A[i] = RandInt32::get() % M;
            for (int i = 0; i < int(B.size()); i++)
                B[i] = RandInt32::get() % M;

            vector<int> out1 = PolyFFTMod::multiply(A, B, M);
            vector<int> out2 = PolyNTT::multiplyFast(A, B, M);
            if (out1 != out2) {
                cout << "Mismatched : " << endl;
                cout << out1 << endl;
                cout << out2 << endl;
            }
            assert(out1 == out2);
        }
    }
    {
        //static const int M = 1000000007;
        static const int M = MOD;

        cout << "*** Speed test ***" << endl;
        for (int n = 32; n <= 2048; n <<= 1) {
            vector<int> in1(n);
            vector<int> in2(n);
            vector<int> out;
            for (int i = 0; i < n; i++) {
                in1[i] = RandInt32::get() % M;
                in2[i] = RandInt32::get() % M;
            }

            cout << "N = " << n << endl;
            cout << "  PolyFFTMod::multiplySlow() : ";
            PROFILE_START(0);
            for (int i = 0; i < 1000; i++) {
                out = PolyFFTMod::multiplySlow(in1, in2, M);
                if (out.empty())
                    cerr << "It'll never be shwon!" << endl;
            }
            PROFILE_STOP(0);

            cout << "  PolyFFTMod::multiply() : ";
            PROFILE_START(1);
            for (int i = 0; i < 1000; i++) {
                out = PolyFFTMod::multiply(in1, in2, M);
                if (out.empty())
                    cerr << "It'll never be shwon!" << endl;
            }
            PROFILE_STOP(1);

            cout << "  NTT::multiply() : ";
            NTT ntt(MOD, 3);
            PROFILE_START(2);
            for (int i = 0; i < 1000; i++) {
                out = ntt.multiply(in1, in2);
                if (out.empty())
                    cerr << "It'll never be shwon!" << endl;
            }
            PROFILE_STOP(2);

            cout << "  PolyNTT::multiply() : ";
            PROFILE_START(3);
            for (int i = 0; i < 1000; i++) {
                out = PolyNTT::multiply(in1, in2, M);
                if (out.empty())
                    cerr << "It'll never be shwon!" << endl;
            }
            PROFILE_STOP(3);

            cout << "  PolyNTT::multiplyFast() : ";
            PROFILE_START(4);
            for (int i = 0; i < 1000; i++) {
                out = PolyNTT::multiplyFast(in1, in2, M);
                if (out.empty())
                    cerr << "It'll never be shwon!" << endl;
            }
            PROFILE_STOP(4);
        }
    }

    cout << "OK!" << endl;
}
