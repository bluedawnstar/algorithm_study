#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "nextCombination.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "../integer/bit.h"

static vector<vector<int>> getCombinationsSlow(int n, int r) {
    vector<vector<int>> res;
    for (int i = 1; i < (1 << n); i++) {
        if (popcount((unsigned int)i) == r) {
            res.push_back({});
            for (int j = 0, t = i; t; j++, t >>= 1) {
                if (t & 1)
                    res.back().push_back(j);
            }
        }
    }
    sort(res.begin(), res.end());

    return res;
}

void testNextCombination() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Next Combination --------------------------------------" << endl;
    {
        int N = 5, R = 3;
        vector<int> c;
        vector<vector<int>> ans;

        init_combination(c, N, R);
        do {
            //cout << c << endl;
            ans.push_back(c);
        } while (next_combination(c, N));

        auto gt = getCombinationsSlow(N, R);
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
