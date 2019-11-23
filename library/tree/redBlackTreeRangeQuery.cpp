#include <cassert>
#include <functional>
#include <algorithm>

using namespace std;

#include "redBlackTreeRangeQuery.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <numeric>
#include <set>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testRBTreePrefixQuery() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Simple Red-Black Tree with Prefix Query -------------------------" << endl;
    {
        RBTreeRangeQuery<int> tree([](int a, int b) { return a + b; }, 0);

        int N = 100;
        vector<int> v;
        for (int i = 0; i < N; i++)
            v.push_back(RandInt32::get() % 10000);
        sort(v.begin(), v.end());

        for (int i = 0; i < N; i++)
            tree.insert(v[i]);

        for (int i = 0; i < N; i++) {
            int gt = accumulate(v.begin(), v.begin() + i + 1, 0);
            int ans = tree.queryPrefix(i);
            if (gt != ans)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    cout << "OK!" << endl;
}
