#include <vector>

using namespace std;

#include "walshHadamard.h"
#include "walshHadamardMod.h"
#include "walshHadamardMod3xor.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

/*
  <related problem>
    https://www.hackerearth.com/challenges/competitive/june-circuits-19/algorithm/xor-paths-dd39904a/

    1) make a tree

        dfsInit(0, -1);

    2) calculate XOR values from the root to each node except the root (paths from root to each node)

        vector<ll> A(POW2Z);    // POW2Z is power of 2 greater than max Z
        dfsXor(0, -1, A, 0);

    3) calculate XOR values of all paths

        auto C = FWHT<long long>::fastXor(A, A);// C[k] = SUM_{k=i^j} A[i]*A[j]
        for (int i = 0; i < int(C.size()); i++)
            pathXor[i] += C[i];                 

        for (int i = 0; i < POW2Z; i++)
            pathXor[i] += 2ll * A[i];           // add each A[i]

        pathXor[0] += N - (N - 1);              // + |nodes| - |edges|

    4) calculate combinations that XOR values of two paths have hamming distance 0 or 1

        long long res = 0;
        for (int i = 0; i < POW2Z; i++) {
            if (!pathXor[i])
                continue;
            pathXor[i] %= MOD;

            res = (res + 1ll * pathXor[i] * pathXor[i] % MOD) % MOD;    // hamming distance 0
            for (auto t = i; t; t &= t - 1) {
                int mask = t & -t;
                if (pathXor[i & ~mask])                                 // hamming distance 1
                    res = (res + 2ll * pathXor[i] * pathXor[i & ~mask] % MOD) % MOD;
            }
        }
*/


template <typename T>
static vector<T> slowXor(const vector<T>& A, const vector<T>& B) {
    int size = 1;
    while (size < int(A.size()) || size < int(B.size()))
        size <<= 1;

    vector<T> C(size);
    for (int i = 0; i < int(A.size()); i++) {
        for (int j = 0; j < int(B.size()); j++)
            C[i ^ j] += A[i] * B[j];
    }

    return C;
}

template <typename T>
static vector<T> slowOr(const vector<T>& A, const vector<T>& B) {
    int size = 1;
    while (size < int(A.size()) || size < int(B.size()))
        size <<= 1;

    vector<T> C(size);
    for (int i = 0; i < int(A.size()); i++) {
        for (int j = 0; j < int(B.size()); j++)
            C[i | j] += A[i] * B[j];
    }

    return C;
}

template <typename T>
static vector<T> slowAnd(const vector<T>& A, const vector<T>& B) {
    int size = 1;
    while (size < int(A.size()) || size < int(B.size()))
        size <<= 1;

    vector<T> C(size);
    for (int i = 0; i < int(A.size()); i++) {
        for (int j = 0; j < int(B.size()); j++)
            C[i & j] += A[i] * B[j];
    }

    return C;
}

//---

const int MOD = 1000000007;

template <typename T>
static vector<T> slowXorMod(const vector<T>& A, const vector<T>& B) {
    int size = 1;
    while (size < int(A.size()) || size < int(B.size()))
        size <<= 1;

    vector<T> C(size);
    for (int i = 0; i < int(A.size()); i++) {
        for (int j = 0; j < int(B.size()); j++)
            C[i ^ j] = T((C[i ^ j] + 1ll * A[i] * B[j]) % MOD);
    }

    return C;
}

template <typename T>
static vector<T> slowOrMod(const vector<T>& A, const vector<T>& B) {
    int size = 1;
    while (size < int(A.size()) || size < int(B.size()))
        size <<= 1;

    vector<T> C(size);
    for (int i = 0; i < int(A.size()); i++) {
        for (int j = 0; j < int(B.size()); j++)
            C[i | j] = T((C[i | j] + 1ll * A[i] * B[j]) % MOD);
    }

    return C;
}

template <typename T>
static vector<T> slowAndMod(const vector<T>& A, const vector<T>& B) {
    int size = 1;
    while (size < int(A.size()) || size < int(B.size()))
        size <<= 1;

    vector<T> C(size);
    for (int i = 0; i < int(A.size()); i++) {
        for (int j = 0; j < int(B.size()); j++)
            C[i & j] = T((C[i & j] + 1ll * A[i] * B[j]) % MOD);
    }

    return C;
}


//-----------------------------------------------------------------------------
// https://www.codechef.com/problems/MDSWIN
// -> https://discuss.codechef.com/t/mdswin-editorial/44120

const int MAXVALUE = 177147;    // 3^11

static long long solveMDSWIN(const vector<int>& A, int K) {
    int N = int(A.size());

    vector<int> cnt(MAXVALUE);
    for (int i = 0; i < N; i++) {
        ++cnt[FWHTMod3Xor<int,MOD>::binaryToBase3(A[i])];
    }

    auto P = FWHTMod3Xor<int,MOD>::pow(cnt, K);

    long long res = 0;
    for (int i = 1; i < int(P.size()); i++)
        res = (res + P[i]) % MOD;

    return res;
}


void testWalshHadamard() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Fast Walsh-Hadamard transform ------------------------" << endl;
    {
        int T = 100;
        int N = 1000;

#if _DEBUG
        T = 10;
        N = 100;
#endif

        while (T-- > 0) {
            vector<long long> A(N);
            vector<long long> B(N);
            for (int i = 0; i < N; i++) {
                A[i] = RandInt32::get() % 10000;
                B[i] = RandInt32::get() % 10000;
            }

            auto ansXor = FWHT<long long>::fastXor(A, B);
            auto ansOr  = FWHT<long long>::fastOr(A, B);
            auto ansAnd = FWHT<long long>::fastAnd(A, B);

            auto gtXor = slowXor(A, B);
            auto gtOr = slowOr(A, B);
            auto gtAnd = slowAnd(A, B);

            if (ansXor != gtXor)
                cout << "Mismatched XOR!" << endl;
            if (ansOr != gtOr)
                cout << "Mismatched OR!" << endl;
            if (ansAnd != gtAnd)
                cout << "Mismatched AND!" << endl;

            assert(ansXor == gtXor);
            assert(ansOr == gtOr);
            assert(ansAnd == gtAnd);
        }
    }
    {
        int T = 100;
        int N = 1000;

#if _DEBUG
        T = 10;
        N = 100;
#endif

        while (T-- > 0) {
            vector<int> A(N);
            vector<int> B(N);
            for (int i = 0; i < N; i++) {
                A[i] = RandInt32::get() % 10000;
                B[i] = RandInt32::get() % 10000;
            }

            auto ansXor = FWHTMod<int>::fastXor(A, B);
            auto ansOr  = FWHTMod<int>::fastOr(A, B);
            auto ansAnd = FWHTMod<int>::fastAnd(A, B);

            auto gtXor = slowXorMod(A, B);
            auto gtOr = slowOrMod(A, B);
            auto gtAnd = slowAndMod(A, B);

            if (ansXor != gtXor)
                cout << "Mismatched XOR!" << endl;
            if (ansOr != gtOr)
                cout << "Mismatched OR!" << endl;
            if (ansAnd != gtAnd)
                cout << "Mismatched AND!" << endl;

            assert(ansXor == gtXor);
            assert(ansOr == gtOr);
            assert(ansAnd == gtAnd);
        }
    }
    {
        auto ans1 = solveMDSWIN(vector<int>{1, 2}, 3);
        int gt1 = 6;
        if (ans1 != gt1)
            cout << "Mismatched : " << ans1 << ", " << gt1 << endl;
        assert(ans1 == gt1);

        auto ans2 = solveMDSWIN(vector<int>{1, 2}, 6);
        int gt2 = 42;
        if (ans2 != gt2)
            cout << "Mismatched : " << ans2 << ", " << gt2 << endl;
        assert(ans2 == gt2);
    }

    cout << "OK!" << endl;
}
