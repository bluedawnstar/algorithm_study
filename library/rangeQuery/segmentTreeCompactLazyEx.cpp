#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreeCompactLazyEx.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

template <typename T>
static void update(vector<T>& A, int L, int R, T x) {
    for (int i = L; i <= R; i++)
        A[i] = x;
}

template <typename T>
static void add(vector<T>& A, int L, int R, T x) {
    for (int i = L; i <= R; i++)
        A[i] += x;
}

template <typename T>
static long long sum(vector<T>& A, int L, int R) {
    long long res = 0;
    for (int i = L; i <= R; i++)
        res += A[i];
    return res;
}


template <typename T>
static int findNext(const vector<T>& A, int start, T x) {
    while (start < int(A.size())) {
        if (A[start] <= x)
            return start;
        start++;
    }
    return -1;
}

template <typename T>
static int findPrev(const vector<T>& A, int start, T x) {
    while (start >= 0) {
        if (A[start] <= x)
            return start;
        start--;
    }
    return -1;
}

void testSegmentTreeCompactLazyEx() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Compact Segment Tree with Lazy Operations -----------------------" << endl;
    {
#ifdef _DEBUG
        const int T = 100;
        const int N = 100;
#else
        const int T = 1000;
        const int N = 1000;
#endif
        const int MAXX = 1000;
        vector<long long> A(N);

        for (int i = 0; i < N; i++)
            A[i] = RandInt32::get() % MAXX;

        auto segTree = makeCompactSegmentTreeLazyEx(A, [](long long a, long long b) { return a + b; },
                                                       [](long long x, long long k) { return x * k; });
        for (int tt = 0; tt < T; tt++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);
            long long X = RandInt32::get() % MAXX;

            switch (RandInt32::get() & 1) {
            case 0:
                update(A, L, R, X);
                segTree.update(L, R, X);
                break;
            case 1:
                add(A, L, R, X);
                segTree.add(L, R, X);
                break;
            }
        }

        for (int L = 0; L < N; L++) {
            for (int R = L; R < N; R++) {
                auto gt = sum(A, L, R);
                auto ans = segTree.query(L, R);
                if (ans != gt) {
                    cout << "Mismatched : " << ans << ", " << gt << endl;
                }
                assert(ans == gt);
            }
        }
    }
    cout << "OK!" << endl;
}
