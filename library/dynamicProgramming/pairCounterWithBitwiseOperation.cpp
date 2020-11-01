#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "pairCounterWithBitwiseOperation.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static long long countPairsWithAndZeroNaive(const vector<int>& A) {
    long long res = 0;

    int N = int(A.size());
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if ((A[i] & A[j]) == 0) {
                res++;
            }
        }
    }
    return res;
}

static long long countPairsWithAndNaive(const vector<int>& A, int target) {
    long long res = 0;

    int N = int(A.size());
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if ((A[i] & A[j]) == target) {
                res++;
            }
        }
    }
    return res;
}

static long long countPairsWithOrNaive(const vector<int>& A, int target) {
    long long res = 0;

    int N = int(A.size());
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if ((A[i] | A[j]) == target) {
                res++;
            }
        }
    }
    return res;
}

static long long countPairsWithXorNaive(const vector<int>& A, int target) {
    long long res = 0;

    int N = int(A.size());
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if ((A[i] ^ A[j]) == target) {
                res++;
            }
        }
    }
    return res;
}

void testPairCounterWithBitwiseOperation() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Pair Counter with Bitwise Operation ------------------------" << endl;
    // Bitwise AND
    {
#ifndef _DEBUG
        const int N = 1'000;
        const int MAXX = 1'000'000;
        const int T = 10;
#else
        const int N = 100;
        const int MAXX = 100'000;
        const int T = 3;
#endif
        for (int tt = 0; tt < T; tt++) {
            vector<int> A(N);

            for (int i = 0; i < N; i++)
                A[i] = RandInt32::get() % MAXX;

            auto gt = countPairsWithAndZeroNaive(A);
            auto ans1 = PairCounterWithBitwiseOperation::countPairsWithAndZeroDP(A);
            auto ans2 = PairCounterWithBitwiseOperation::countPairsWithAndDP(A, 0);

            if (gt != ans1 || gt != ans2) {
                cout << "Mismatch : " << ans1 << ", " << ans2 << ", " << gt << endl;
                cout << "           A = " << A << endl;
            }
            assert(ans1 == gt && ans2 == gt);
        }
    }
    {
#ifndef _DEBUG
        const int N = 1'000;
        const int MAXX = 1'000;
        const int T = 10;
#else
        const int N = 100;
        const int MAXX = 100;
        const int T = 3;
#endif
        for (int tt = 0; tt < T; tt++) {
            vector<int> A(N);
            int target = RandInt32::get() % MAXX;

            for (int i = 0; i < N; i++)
                A[i] = RandInt32::get() % MAXX;

            auto gt = countPairsWithAndNaive(A, target);
            auto ans = PairCounterWithBitwiseOperation::countPairsWithAndDP(A, target);

            if (gt != ans) {
                cout << "Mismatch : " << ans << ", " << gt << endl;
                cout << "           A = " << A << ", target = " << target << endl;
            }
            assert(ans == gt);
        }
    }
    // Bitwise OR
    {
#ifndef _DEBUG
        const int N = 1'000;
        const int MAXX = 1'000;
        const int T = 10;
#else
        const int N = 100;
        const int MAXX = 100;
        const int T = 3;
#endif
        for (int tt = 0; tt < T; tt++) {
            vector<int> A(N);
            int target = RandInt32::get() % MAXX;

            for (int i = 0; i < N; i++)
                A[i] = RandInt32::get() % MAXX;

            auto gt = countPairsWithOrNaive(A, target);
            auto ans = PairCounterWithBitwiseOperation::countPairsWithOrDP(A, target);

            if (gt != ans) {
                cout << "Mismatch : " << ans << ", " << gt << endl;
                cout << "           A = " << A << ", target = " << target << endl;
            }
            assert(ans == gt);
        }
    }
    // Bitwise XOR
    {
#ifndef _DEBUG
        const int N = 1'000;
        const int MAXX = 1'000;
        const int T = 10;
#else
        const int N = 100;
        const int MAXX = 100;
        const int T = 3;
#endif
        for (int tt = 0; tt < T; tt++) {
            vector<int> A(N);
            int target = RandInt32::get() % MAXX;

            for (int i = 0; i < N; i++)
                A[i] = RandInt32::get() % MAXX;

            auto gt = countPairsWithXorNaive(A, target);
            auto ans = PairCounterWithBitwiseOperation::countPairsWithXOR(A, target);

            if (gt != ans) {
                cout << "Mismatch : " << ans << ", " << gt << endl;
                cout << "           A = " << A << ", target = " << target << endl;
            }
            assert(ans == gt);
        }
        {
            vector<int> A(N);
            int target = 0;

            for (int i = 0; i < N; i++)
                A[i] = RandInt32::get() % MAXX;

            auto gt = countPairsWithXorNaive(A, target);
            auto ans = PairCounterWithBitwiseOperation::countPairsWithXOR(A, target);

            if (gt != ans) {
                cout << "Mismatch : " << ans << ", " << gt << endl;
                cout << "           A = " << A << ", target = " << target << endl;
            }
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
