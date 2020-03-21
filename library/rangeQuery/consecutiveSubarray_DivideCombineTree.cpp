#include <vector>
#include <algorithm>
#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

using namespace std;

#include "consecutiveSubarray_DivideCombineTree.h"
#include "consecutiveSubarray_DivideCombineTreeWithLCA.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

// related problems
// https://www.codechef.com/problems/GOODSEGS
// -> https://discuss.codechef.com/t/goodsegs-editorial/56718
// https://codeforces.com/problemset/problem/526/F
// -> https://www.cnblogs.com/yyf0309/p/7498514.html

static pair<int,int> findSmallestSlow(const vector<int>& P, int L, int R) {
    pair<int, int> res(0, int(P.size()) - 1);
    for (int i = 0; i <= L; i++) {
        for (int j = R; j < int(P.size()); j++) {
            auto it = minmax_element(P.begin() + i, P.begin() + j + 1);
            if (abs(*it.second - *it.first) == j - i) {
                if (res.second - res.first > j - i) {
                    res.first = i;
                    res.second = j;
                }
            }
        }
    }
    return res;
}

void testDivideCombineTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Divide-Combine Tree ----------------------------------------------" << endl;
    {
        vector<int> P{ 0, 3, 1, 4, 2, 5 };

        /*
            5 |                     x
            4 |             x
            3 |     x
            2 |                 x
            1 |         x
            0 | x
              +---+---+---+---+---+---
                0   1   2   3   4   5
        */

        DivideCombineTree dct;
        dct.build(P.data(), int(P.size()));

        int ans = int(dct.countIntersectedPairs(2));
        int gt = 1;
        if (ans != gt)
            cout << "Mismatch : " << ans << ", " << gt << endl;
        assert(ans == 1);
    }
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

        DivideCombineTreeWithLCA dct;
        dct.build(P);

        {
            auto gt = findSmallestSlow(P, 4, 5);
            auto ans = dct.findSmallestConsecutiveSubarray(4, 5);   // ans = (1, 5)
            if (gt != ans)
                cout << "Mismatch : (4, 5) = " << ans << ", " << gt << endl;
            assert(gt == ans);
        }

        int N = int(P.size());
        for (int L = 0; L < N; L++) {
            for (int R = L; R < N; R++) {
                auto gt = findSmallestSlow(P, L, R);
                auto ans = dct.findSmallestConsecutiveSubarray(L, R);
                if (gt != ans)
                    cout << "Mismatch : (" << L << ", " << R << ") = " << ans << ", " << gt << endl;
                assert(gt == ans);
            }
        }
    }

    cout << "OK!" << endl;
}
