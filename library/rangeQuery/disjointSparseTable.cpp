#include <memory.h>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "disjointSparseTable.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

static const int MOD = 1000000007;

static long long sum(const vector<int>& A, int L, int R) {
    long long res = 0;
    while (L <= R)
        res += A[L++];
    return res;
}

static int mult(const vector<int>& A, int L, int R, int mod) {
    long long res = 1;
    while (L <= R)
        res = res * A[L++] % mod;
    return int(res);
}

void testDisjointSparseTable() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Disjoint Sparse Table ----------------------------------------" << endl;
    cout << "*** Sum ***" << endl;
    {
        int N = 10000;
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 100;

        vector<int> prefixSum(N + 1);
        prefixSum[0] = 0;
        for (int i = 1; i <= N; i++) {
            prefixSum[i] = prefixSum[i - 1] + in[i - 1];
        }

        auto dst = makeDisjointSparseTable<int>(in, [](int a, int b) { return a + b; }, 0);

        for (int i = 0; i < 1000; i++) {
            int left = RandInt32::get() % N;
            int right = RandInt32::get() % N;
            if (left > right)
                swap(left, right);

            int ans1 = prefixSum[right + 1] - prefixSum[left];
            int ans2 = dst.query(left, right);
            assert(ans1 == ans2);
        }
    }
    cout << "*** Multiplication ***" << endl;
    {
        int N = 10000;
        vector<int> in(N);
        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get();

        auto dst = makeDisjointSparseTable<int>(in, [](int a, int b) { return int(1ll * a * b % MOD); }, 1);

        for (int i = 0; i < 1000; i++) {
            int left = RandInt32::get() % N;
            int right = RandInt32::get() % N;
            if (left > right)
                swap(left, right);

            int ans1 = mult(in, left, right, MOD);
            int ans2 = dst.query(left, right);
            assert(ans1 == ans2);
        }
    }

    cout << "OK!" << endl;
}
