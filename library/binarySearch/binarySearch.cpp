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
            int ans2 = lowerBound2<int>(0, N - 1, [x, &v](int mid) {
                return x <= v[mid];
            });

            if (gt != ans1 || gt != ans2)
                cout << "Mismatched : " << gt << ", " << ans1 << ", " << ans2 << endl;

            assert(gt == ans1);
            assert(gt == ans2);
        }
    }
    {
        int N = 100000;
        int T = 100;

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
            int ans2 = lowerBound2<int>(0, N - 1, [x, &v](int mid) {
                return x <= v[mid];
            });

            if (gt != ans1 || gt != ans2)
                cout << "Mismatched : " << gt << ", " << ans1 << ", " << ans2 << endl;

            assert(gt == ans1);
            assert(gt == ans2);
        }
    }

    cout << "OK!" << endl;
}
