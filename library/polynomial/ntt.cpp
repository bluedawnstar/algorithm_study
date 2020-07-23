#include <vector>
#include <algorithm>

using namespace std;

#include "ntt.h"
#include "ntt_fast.h"
#include "polyNTT.h"
#include "factorialNTT.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "polyFFTMod.h"

#define MOD     998244353
#define ROOT    3

// https://www.codechef.com/problems/TREDEG

struct Solver {
    vector<int> fact;
    vector<int> factInv;
    vector<int> inv;

    // O(N)
    void build(int N) {
        fact.resize(N + 1);
        factInv.resize(N + 1);
        inv.resize(N + 1);

        inv[0] = 0;
        inv[1] = 1;
        for (int i = 2; i <= N; i++)
            inv[i] = int((MOD - 1ll * (MOD / i) * inv[MOD % i] % MOD) % MOD);

        fact[0] = 1;
        factInv[0] = 1;

        fact[1] = 1;
        factInv[1] = 1;

        for (int i = 2; i <= N; i++) {
            fact[i] = int(1ll * fact[i - 1] * i % MOD);
            factInv[i] = int(1ll * factInv[i - 1] * inv[i] % MOD);
        }
    }

    int solve(int N, int K) {
        if (K == 1) {
            long long ans = 0;
            for (int i = 0; i <= N - 2; i++)
                ans = (ans + 1ll * fact[i] * comb(N, i) % MOD * comb(N - 2, i) % MOD * modPow(N, N - 2 - i)) % MOD;
            return ans * modPow(N, MOD - 1 - (N - 2)) % MOD;
        }

        vector<int> A(N);
        for (int i = 0; i < N; i++)
            A[i] = 1ll * NTT<MOD,3>::modPow(i + 1, K) * factInv[i] % MOD;

        // poly = (1^k * x^0/0! + 2^k * x^1/1!*x + 3^k * x^2/2! + ... + n^k * x^(n-1)/(n-1)!)^n
        auto poly = NTT<MOD,3>::powFast(A, N);

        // res = poly[n-2] * (n - 2)! / n^(n-2)
        return int(1ll * poly[N - 2] * fact[N - 2] % MOD * NTT<MOD,3>::modInv(NTT<MOD,3>::modPow(N, N - 2)) % MOD);
    }

    int solveWithFFT(int N, int K) {
        if (K == 1) {
            long long ans = 0;
            for (int i = 0; i <= N - 2; i++)
                ans = (ans + 1ll * fact[i] * comb(N, i) % MOD * comb(N - 2, i) % MOD * modPow(N, N - 2 - i)) % MOD;
            return ans * modPow(N, MOD - 1 - (N - 2)) % MOD;
        }

        vector<int> A(N);
        for (int i = 0; i < N; i++)
            A[i] = 1ll * PolyFFTMod<MOD>::modPow(i + 1, K) * factInv[i] % MOD;

        // poly = (1^k * x^0/0! + 2^k * x^1/1!*x + 3^k * x^2/2! + ... + n^k * x^(n-1)/(n-1)!)^n
        auto poly =PolyFFTMod<MOD>::powFast(A, N);

        // res = poly[n-2] * (n - 2)! / n^(n-2)
        return int(1ll * poly[N - 2] * fact[N - 2] % MOD * PolyFFTMod<MOD>::modInv(PolyFFTMod<MOD>::modPow(N, N - 2)) % MOD);
    }

    int solveWithPolyNTT(int N, int K) {
        if (K == 1) {
            long long ans = 0;
            for (int i = 0; i <= N - 2; i++)
                ans = (ans + 1ll * fact[i] * comb(N, i) % MOD * comb(N - 2, i) % MOD * modPow(N, N - 2 - i)) % MOD;
            return ans * modPow(N, MOD - 1 - (N - 2)) % MOD;
        }

        vector<int> A(N);
        for (int i = 0; i < N; i++)
            A[i] = 1ll * PolyNTT<MOD,3>::modPow(i + 1, K) * factInv[i] % MOD;

        // poly = (1^k * x^0/0! + 2^k * x^1/1!*x + 3^k * x^2/2! + ... + n^k * x^(n-1)/(n-1)!)^n
        auto poly = PolyNTT<MOD,3>::powFast(A, N);

        // res = poly[n-2] * (n - 2)! / n^(n-2)
        return int(1ll * poly[N - 2] * fact[N - 2] % MOD * PolyNTT<MOD,3>::modInv(PolyNTT<MOD,3>::modPow(N, N - 2)) % MOD);
    }

private:
    int perm(int n, int r) {
        return int(1ll * fact[n] * factInv[n - r] % MOD);
    }

    int permInv(int n, int r) {
        return int(1ll * factInv[n] * fact[n - r] % MOD);
    }

    int comb(int n, int r) {
        if (n < r)
            return 0;

        if (n == 0 || r == 0 || n == r)
            return 1;

        if (n - r < r)
            r = n - r;

        return int(1ll * fact[n] * factInv[n - r] % MOD * factInv[r] % MOD);
    }

    int combInv(int n, int r) {
        if (n < r)
            return 0;

        if (n == 0 || r == 0 || n == r)
            return 1;

        if (n - r < r)
            r = n - r;

        return int(1ll * factInv[n] * fact[n - r] % MOD * fact[r] % MOD);
    }

    int modPow(int x, int n) {
        if (n == 0)
            return 1;

        long long t = x;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % MOD;
            t = t * t % MOD;
        }
        return int(res);
    }

    int modInv(int x) {
        return modPow(x, MOD - 2);
    }
};


void testNTT() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- NTT ------------------------" << endl;
    {
        Solver solver;
        solver.build(2000000);
        int ans1 = solver.solve(3, 1);
        int ans2 = solver.solve(4, 2);
        int ans3 = solver.solveWithFFT(3, 1);
        int ans4 = solver.solveWithFFT(4, 2);
        int ans5 = solver.solveWithPolyNTT(3, 1);
        int ans6 = solver.solveWithPolyNTT(4, 2);

        if (ans1 != 2 || ans2 != 748683279)
            cout << "Invalid answer : " << ans1 << ", " << ans2 << endl;
        if (ans3 != 2 || ans4 != 748683279)
            cout << "Invalid answer with FFT : " << ans3 << ", " << ans4 << endl;
        if (ans5 != 2 || ans6 != 748683279)
            cout << "Invalid answer with NTT : " << ans5 << ", " << ans6 << endl;
        assert(ans1 == 2);
        assert(ans2 == 748683279);
        assert(ans3 == 2);
        assert(ans4 == 748683279);
        assert(ans5 == 2);
        assert(ans6 == 748683279);
    }
    {
        int TESTN = 1000;
        for (int i = 0; i < 10; i++) {
            vector<int> A(TESTN);
            vector<int> B(TESTN);

            for (int i = 0; i < int(A.size()); i++)
                A[i] = RandInt32::get() % MOD;
            for (int i = 0; i < int(B.size()); i++)
                B[i] = RandInt32::get() % MOD;

            vector<int> out1 = PolyFFTMod<MOD>::multiply(A, B);
            vector<int> out2 = NTT<MOD,3>::multiply(A, B);
            if (out1 != out2) {
                cout << "Mismatched : " << endl;
                cout << out1 << endl;
                cout << out2 << endl;
            }
            assert(out1 == out2);
        }
    }
    {
        int TESTN = 1000;
        for (int i = 0; i < 10; i++) {
            vector<int> A(TESTN);
            vector<int> B(TESTN);

            for (int i = 0; i < int(A.size()); i++)
                A[i] = RandInt32::get() % MOD;
            for (int i = 0; i < int(B.size()); i++)
                B[i] = RandInt32::get() % MOD;

            vector<int> out1 = PolyFFTMod<MOD>::multiply(A, B);
            vector<int> out2 = NTT<MOD,3>::multiply(A, B);
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
        static const int R = 5;
        int TESTN = 1000;
        for (int i = 0; i < 10; i++) {
            vector<int> A(TESTN);
            vector<int> B(TESTN);

            for (int i = 0; i < int(A.size()); i++)
                A[i] = RandInt32::get() % M;
            for (int i = 0; i < int(B.size()); i++)
                B[i] = RandInt32::get() % M;

            vector<int> out1 = PolyFFTMod<M>::multiply(A, B);
            vector<int> out2 = PolyNTT<M,R>::multiply(A, B);
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
        static const int R = 5;
        int TESTN = 1000;
        for (int i = 0; i < 10; i++) {
            vector<int> A(TESTN);
            vector<int> B(TESTN);

            for (int i = 0; i < int(A.size()); i++)
                A[i] = RandInt32::get() % M;
            for (int i = 0; i < int(B.size()); i++)
                B[i] = RandInt32::get() % M;

            vector<int> out1 = PolyFFTMod<M>::multiply(A, B);
            vector<int> out2 = PolyNTT<M,R>::multiplyFast(A, B);
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
        static const int R = ROOT;

        cout << "*** Speed test ***" << endl;
        for (int n = 32; n <= 2048; n <<= 1) {
            vector<int> in1(n);
            vector<int> in2(n);
            for (int i = 0; i < n; i++) {
                in1[i] = RandInt32::get() % M;
                in2[i] = RandInt32::get() % M;
            }

            vector<int> out1, out2, out3, out4, out5;

            cout << "N = " << n << endl;

            cout << "  PolyFFTMod::multiplySlow() : ";
            PROFILE_START(0);
            for (int i = 0; i < 1000; i++)
                out1 = PolyFFTMod<M>::multiplySlow(in1, in2);
            PROFILE_STOP(0);

            cout << "  PolyFFTMod::multiply() : ";
            PROFILE_START(1);
            for (int i = 0; i < 1000; i++)
                out2 = PolyFFTMod<M>::multiply(in1, in2);
            PROFILE_STOP(1);

            cout << "  NTT::multiply() : ";
            PROFILE_START(2);
            for (int i = 0; i < 1000; i++)
                out3 = NTT<MOD,3>::multiply(in1, in2);
            PROFILE_STOP(2);

            cout << "  PolyNTT::multiply() : ";
            PROFILE_START(3);
            for (int i = 0; i < 1000; i++)
                out4 = PolyNTT<M,R>::multiply(in1, in2);
            PROFILE_STOP(3);

            cout << "  PolyNTT::multiplyFast() : ";
            PROFILE_START(4);
            for (int i = 0; i < 1000; i++)
                out5 = PolyNTT<M,R>::multiplyFast(in1, in2);
            PROFILE_STOP(4);

            if (out1 != out2 || out1 != out3 || out1 != out4 || out1 != out5)
                cout << "ERROR at " << __LINE__ << endl;
            assert(out1 == out2 && out1 == out3 && out1 == out4 && out1 == out5);
        }
    }

    cout << "OK!" << endl;
}
