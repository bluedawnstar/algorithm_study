#include <vector>
#include <algorithm>

using namespace std;

#include "dynamicSegmentTreeMaxSubarray.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <tuple>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int getMaxSubarray(const vector<int>& v, int L, int R) {
    int ans = v[L];
    int sum = v[L];
    for (int i = L + 1; i <= R; i++) {
        sum = max(sum + v[i], v[i]);
        ans = max(ans, sum);
    }

    return ans;
}

// https://www.codechef.com/problems/CBFEAST

void testDynamicSegmentTreeMaxSubarray() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Dynamic Segment Tree for Max Subarray Sum ----------------------------------------" << endl;
    {
        vector<tuple<int, int, int>> cmd{
            { 0, 1, 1 },
            { 1, 3, 1 },
            { 0, 1, 2 },
        };
        DynamicSegmentTreeMaxSubarray<int> tree(0, 3);

        tree.addLeft(get<0>(cmd[0]), get<1>(cmd[0]), get<2>(cmd[0]));
        assert(tree.query(0) == 1);
        assert(tree.query(1) == 1);
        assert(tree.query(2) == 0);

        tree.addRight(get<0>(cmd[1]), get<1>(cmd[1]), get<2>(cmd[1]));
        assert(tree.query(0) == 1);
        assert(tree.query(1) == 2);
        assert(tree.query(2) == 1);

        tree.addLeft(get<0>(cmd[2]), get<1>(cmd[2]), get<2>(cmd[2]));
        assert(tree.query(0) == 3);
        assert(tree.query(1) == 4);
        assert(tree.query(2) == 1);
    }

    cout << "OK!" << endl;
}
