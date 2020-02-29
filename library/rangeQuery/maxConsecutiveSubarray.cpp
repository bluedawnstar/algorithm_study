#include <vector>
#include <algorithm>

using namespace std;

#include "maxConsecutiveSubarray.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

static int solveSlow(const vector<int>& P, int L, int R) {
    int res = 1;
    for (int i = L; i <= R; i++) {
        for (int j = i; j <= R; j++) {
            auto it = minmax_element(P.begin() + i, P.begin() + j + 1);
            if (abs(*it.second - *it.first) == j - i)
                res = max(res, j - i + 1);
        }
    }
    return res;
}

void testMaxConsecutiveSubarray() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Max Consecutive Subarray ----------------------------------------------" << endl;
    {
        vector<int> P{ 1, 4, 3, 5, 2, 6, 0 };

        /*
            6 |                     x
            5 |             x
            4 |     x
            3 |         x
            2 |                 x
            1 | x
            0 |                         x
              +---+---+---+---+---+---+---
                0   1   2   3   4   5   6
        */

        MaxConsecutiveSubarray mcs;
        mcs.build(P);

        int N = int(P.size());
        for (int L = 0; L < N; L++) {
            for (int R = L; R < N; R++) {
                int gt = solveSlow(P, L, R);
                int ans = mcs.query(L, R);
                if (gt != ans)
                    cout << "Mismatch : (" << L << ", " << R << ") = " << ans << ", " << gt << endl;
                assert(gt == ans);
            }
        }
    }

    cout << "OK!" << endl;
}
