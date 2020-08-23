#include <memory.h>
#include <functional>
#include <limits>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "segmentTreeBeatsMin.h"
#include "segmentTreeBeatsMax.h"
#include "segmentTreeBeatsMinMax.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// <Related problems>
// https://www.acmicpc.net/problem/17474   (DONE)
// https://www.acmicpc.net/problem/17473   (TODO)

template <typename T>
static T sumSlow(vector<T>& v, int L, int R) {
    T res = 0;
    while (L <= R)
        res += v[L++];
    return res;
}

template <typename T>
static pair<T,T> minmaxSlow(vector<T>& v, int L, int R) {
    pair<T,T> res(numeric_limits<T>::max(), numeric_limits<T>::min());
    for (; L <= R; L++) {
        res.first = min(res.first, v[L]);
        res.second = max(res.second, v[L]);
    }
    return res;
}

template <typename T>
static void updateMinSlow(vector<T>& v, int L, int R, T x) {
    for (; L <= R; L++)
        v[L] = min(v[L], x);
}

template <typename T>
static void updateMaxSlow(vector<T>& v, int L, int R, T x) {
    for (; L <= R; L++)
        v[L] = max(v[L], x);
}


void testSegmentTreeBeats() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Segment Tree Beats -----------------------------------" << endl;
    {
        const int MAXX = 100'000'000;
        int N = 1000;
        int T = 10;

        for (int tt = 0; tt < T; tt++) {
            vector<long long> A(N);
            for (int i = 0; i < N; i++)
                A[i] = RandInt32::get() % MAXX + 1;
            
            SegmentTreeBeatsMin<long long> tree;
            tree.build(A);

            for (int i = 0; i < N; i++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;
                if (L > R)
                    swap(L, R);
                long long x = RandInt32::get() % MAXX + 1;

                updateMinSlow(A, L, R, x);
                tree.updateMin(L, R, x);
            }

            for (int i = 0; i < N; i++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;
                if (L > R)
                    swap(L, R);

                auto sum1 = sumSlow(A, L, R);
                auto sum2 = tree.querySum(L, R);
                if (sum1 != sum2) {
                    cout << "Mismatched : " << sum1 << ", " << sum2 << endl;
                }
                assert(sum1 == sum2);

                auto mx1 = minmaxSlow(A, L, R);
                auto mx2 = tree.queryMinMax(L, R);
                if (mx1 != mx2) {
                    cout << "Mismatched : " << mx1 << ", " << mx2 << endl;
                }
                assert(mx1 == mx2);
            }
        }
    }
    {
        const int MAXX = 100'000'000;
        int N = 1000;
        int T = 10;

        for (int tt = 0; tt < T; tt++) {
            vector<long long> A(N);
            for (int i = 0; i < N; i++)
                A[i] = RandInt32::get() % MAXX + 1;

            SegmentTreeBeatsMax<long long> tree;
            tree.build(A);

            for (int i = 0; i < N; i++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;
                if (L > R)
                    swap(L, R);
                long long x = RandInt32::get() % MAXX + 1;

                updateMaxSlow(A, L, R, x);
                tree.updateMax(L, R, x);
            }

            for (int i = 0; i < N; i++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;
                if (L > R)
                    swap(L, R);

                auto sum1 = sumSlow(A, L, R);
                auto sum2 = tree.querySum(L, R);
                if (sum1 != sum2) {
                    cout << "Mismatched : " << sum1 << ", " << sum2 << endl;
                }
                assert(sum1 == sum2);

                auto mx1 = minmaxSlow(A, L, R);
                auto mx2 = tree.queryMinMax(L, R);
                if (mx1 != mx2) {
                    cout << "Mismatched : " << mx1 << ", " << mx2 << endl;
                }
                assert(mx1 == mx2);
            }
        }
    }
    {
        const int MAXX = 100'000'000;
        int N = 1000;
        int T = 10;

        for (int tt = 0; tt < T; tt++) {
            vector<long long> A(N);
            for (int i = 0; i < N; i++)
                A[i] = RandInt32::get() % MAXX + 1;

            SegmentTreeBeatsMinMax<long long> tree;
            tree.build(A);

            for (int i = 0; i < N; i++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;
                if (L > R)
                    swap(L, R);
                long long x = RandInt32::get() % MAXX + 1;

                if (RandInt32::get() & 1) {
                    updateMinSlow(A, L, R, x);
                    tree.updateMin(L, R, x);
                } else {
                    updateMaxSlow(A, L, R, x);
                    tree.updateMax(L, R, x);
                }
            }

            for (int i = 0; i < N; i++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;
                if (L > R)
                    swap(L, R);

                auto sum1 = sumSlow(A, L, R);
                auto sum2 = tree.querySum(L, R);
                if (sum1 != sum2) {
                    cout << "Mismatched : " << sum1 << ", " << sum2 << endl;
                }
                assert(sum1 == sum2);

                auto mx1 = minmaxSlow(A, L, R);
                auto mx2 = tree.queryMinMax(L, R);
                if (mx1 != mx2) {
                    cout << "Mismatched : " << mx1 << ", " << mx2 << endl;
                }
                assert(mx1 == mx2);
            }
        }
    }

    cout << "OK!" << endl;
}
