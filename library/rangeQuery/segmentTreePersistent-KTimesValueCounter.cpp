#include <functional>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreePersistent-KTimesValueCounter.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <unordered_map>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// https://www.hackerearth.com/challenges/competitive/april-circuits-19/algorithm/adadnahsioo-2804289e/

static int countNaive(const vector<int>& v, int L, int R, int K) {
    unordered_map<int, int> M;

    for (int i = L; i <= R; i++)
        ++M[v[i]];

    int res = 0;
    for (auto it : M) {
        if (it.second == K)
            ++res;
    }

    return res;
}

void testKTimesValueCounter() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- The number of values which appear K times in a range ----------------------------" << endl;
    {
        int N = 100000;
        int MaxValue = N;
        int T = 30;

#ifdef _DEBUG
        N = 1000;
        T = 10;
#endif

        for (int i = 0; i < T; i++) {
            vector<int> v(N);
            KTimesValueCounter<int> tree;

            for (int j = 0; j < N; j++)
                v[j] = RandInt32::get() % MaxValue;

            tree.build(v, 3);
            for (int t = 0; t < T; t++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;
                if (L > R)
                    swap(L, R);

                int gt = countNaive(v, L, R, 3);
                int ans = tree.query(L, R);
                if (ans != gt)
                    cout << "Mismatched : " << ans << ", " << gt << endl;
                assert(ans == gt);
            }
        }
    }

    cout << "OK!" << endl;
}
