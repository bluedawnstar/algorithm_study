#include <functional>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreePersistent-UniqueValueCounter.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int countNaive(const vector<int>& v, int L, int R) {
    unordered_set<int> S;

    for (int i = L; i <= R; i++)
        S.insert(v[i]);

    return int(S.size());
}

void testUniqueValueCounter() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- The number of unique values in a range ----------------------------" << endl;
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
            UniqueValueCounter counter;

            for (int j = 0; j < N; j++)
                v[j] = RandInt32::get() % MaxValue;

            counter.buildWithCompactValues(v, MaxValue);
            for (int t = 0; t < T; t++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;
                if (L > R)
                    swap(L, R);

                int gt = countNaive(v, L, R);
                int ans = counter.query(L, R);
                if (ans != gt)
                    cout << "Mismatched : " << ans << ", " << gt << endl;
                assert(ans == gt);
            }
        }
    }

    cout << "OK!" << endl;
}
