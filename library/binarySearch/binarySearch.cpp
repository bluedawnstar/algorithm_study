#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

#include "binarySearch.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <numeric>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testBinarySearch() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Binary Search -------------------------------" << endl;
    {
        int N = 100000;
        int T = 100;
#ifdef _DEBUG
        N = 1000;
#endif

        while (T-- > 0) {
            vector<int> v(N);
            for (int i = 0; i < N; i++)
                v[i] = RandInt32::get();
            sort(v.begin(), v.end());

            int x = RandInt32::get();

            int gt = int(lower_bound(v.begin(), v.end(), x) - v.begin());

            int ans1 = lowerBound<int>(0, N - 1, [x, &v](int mid) {
                return x <= v[mid];
            });
            int ans11 = int(lowerBound<int>(&v[0], &v[N - 1], x) - &v[0]);

            int ans2 = lowerBound2<int>(0, N - 1, [x, &v](int mid) {
                return x <= v[mid];
            });
            int ans21 = int(lowerBound2<int>(&v[0], &v[N - 1], x) - &v[0]);

            int ans3 = lowerBound3<int>(0, N - 1, [x, &v](int mid) {
                return x <= v[mid];
            });
            int ans31 = int(lowerBound3<int>(&v[0], &v[N - 1], x) - &v[0]);

            if (gt != ans1 || gt != ans2 || gt != ans3)
                cout << "Mismatched1 : " << gt << ", " << ans1 << ", " << ans2 << ", " << ans3 << endl;
            if (gt != ans11 || gt != ans21 || gt != ans31)
                cout << "Mismatched2 : " << gt << ", " << ans11 << ", " << ans21 << ", " << ans31 << endl;

            assert(gt == ans1); assert(gt == ans11);
            assert(gt == ans2); assert(gt == ans21);
            assert(gt == ans3); assert(gt == ans31);
        }
    }
    {
        int N = 100000;
        int T = 100;
#ifdef _DEBUG
        N = 1000;
#endif

        while (T-- > 0) {
            vector<int> v(N);
            for (int i = 0; i < N; i++)
                v[i] = RandInt32::get();
            sort(v.begin(), v.end());

            int x = v[RandInt32::get() % N];

            int gt = int(lower_bound(v.begin(), v.end(), x) - v.begin());

            int ans1 = lowerBound<int>(0, N - 1, [x, &v](int mid) {
                return x <= v[mid];
            });
            int ans11 = int(lowerBound<int>(&v[0], &v[N - 1], x) - &v[0]);

            int ans2 = lowerBound2<int>(0, N - 1, [x, &v](int mid) {
                return x <= v[mid];
            });
            int ans21 = int(lowerBound2<int>(&v[0], &v[N - 1], x) - &v[0]);

            int ans3 = lowerBound3<int>(0, N - 1, [x, &v](int mid) {
                return x <= v[mid];
            });
            int ans31 = int(lowerBound3<int>(&v[0], &v[N - 1], x) - &v[0]);

            if (gt != ans1 || gt != ans2 || gt != ans3)
                cout << "Mismatched : " << gt << ", " << ans1 << ", " << ans2 << ", " << ans3 << endl;
            if (gt != ans11 || gt != ans21 || gt != ans31)
                cout << "Mismatched2 : " << gt << ", " << ans11 << ", " << ans21 << ", " << ans31 << endl;

            assert(gt == ans1); assert(gt == ans11);
            assert(gt == ans2); assert(gt == ans21);
            assert(gt == ans3); assert(gt == ans31);
        }
    }
    {
        int T = 1'000'000;
        const int RANGE = 100;
#ifndef _DEBUG
        for (int N = 10'000'000; N <= 100'000'000; N += 10'000'000) {
#else
        T = 100'000;
        for (int N = 10'000'000; N <= 10'000'000; N += 10'000'000) {
#endif
            cout << "*** N = " << N << endl;

            vector<int> v(N);
            iota(v.begin(), v.end(), 0);

            for (int i = RANGE; i <= N; i *= 10) {
                cout << "-- range = (" << i - RANGE << ", " << i << ")" << endl;

                int ans1 = 0;
                {
                    vector<int> v1 = v;
                    v1[N - 1] = v1[N - 2];

                    PROFILE_HI_START(0);
                    for (int j = 0, start = i - RANGE; j < T; j++) {
                        ans1 += int(lowerBound<int>(&v1[0], &v1[N - 1], start + j % RANGE) - &v1[0]);
                    }
                    PROFILE_HI_STOP(0);
                }

                int ans2 = 0;
                {
                    vector<int> v1 = v;
                    v1[N - 1] = v1[N - 2];

                    PROFILE_HI_START(1);
                    for (int j = 0, start = i - RANGE; j < T; j++) {
                        ans2 += int(lowerBound2<int>(&v1[0], &v1[N - 1], start + j % RANGE) - &v1[0]);
                    }
                    PROFILE_HI_STOP(1);
                }

                int ans3 = 0;
                {
                    vector<int> v1 = v;
                    v1[N - 1] = v1[N - 2];

                    PROFILE_HI_START(2);
                    for (int j = 0, start = i - RANGE; j < T; j++) {
                        ans3 += int(lowerBound3<int>(&v1[0], &v1[N - 1], start + j % RANGE) - &v1[0]);
                    }
                    PROFILE_HI_STOP(2);
                }

                if (ans1 != ans2 || ans1 != ans3) {
                    cout << "ERROR: " << ans1 << ", " << ans2 << ", " << ans3 << endl;
                }

                assert(ans1 == ans2);
                assert(ans1 == ans3);
            }
        }
    }

    cout << "OK!" << endl;
}
