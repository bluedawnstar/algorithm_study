#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "dynamicSegmentTreeForest.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int sumSlow(const vector<int>& v, int L, int R) {
    int res = 0;
    while (L <= R)
        res += v[L++];
    return res;
}

void testDynamicSegmentTreeForest() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Dynamic Segment Tree Forest ----------------------------------------" << endl;
    {
#ifdef _DEBUG
        const int N = 100;
#else
        const int N = 1000;
#endif
        const int X = 10000;
        vector<int> v1(N), v2(N);
        for (int i = 0; i < N; i++) {
            v1[i] = RandInt32::get() % X;
            v2[i] = RandInt32::get() % X;
        }

        DynamicSegmentTreeForest<int> forest([](int a, int b) { return a + b; }, 0);
        forest.init(N);

        int tree1 = -1;
        int tree2 = -1;
        for (int i = 0; i < N; i++) {
            tree1 = forest.update(tree1, i, v1[i]);
            tree2 = forest.update(tree2, i, v2[i]);
        }

        for (int L = 0; L < N; L++) {
            for (int R = L; R < N; R++) {
                int gt1 = sumSlow(v1, L, R);
                int gt2 = sumSlow(v2, L, R);
                int ans1 = forest.query(tree1, L, R);
                int ans2 = forest.query(tree2, L, R);
                if (ans1 != gt1)
                    cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << gt1 << endl;
                if (ans2 != gt2)
                    cout << "Mismatched at " << __LINE__ << " : " << ans2 << ", " << gt2 << endl;
                assert(ans1 == gt1);
                assert(ans2 == gt2);
            }
        }
    }

    cout << "OK!" << endl;
}
