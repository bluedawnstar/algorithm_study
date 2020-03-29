#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreeSubsetXor.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <tuple>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

const int MOD = 1000000007;

// https://codeforces.com/contest/959/problem/F
// https://codeforces.com/problemset/problem/1100/F

#if 1
// https://www.hackerearth.com/challenges/competitive/march-circuits-20/algorithm/chef-f59c8115/
void solve(SubsetXorSegmentTree<26, MOD>& sxor, const int A[], int N, const vector<tuple<int,int,int>>& query, vector<int>& outAns) {
    sxor.build(A, N);

    int type, L, R;

    outAns.clear();
    for (int i = 0; i < int(query.size()); i++) {
        tie(type, L, R) = query[i];
        if (type == 1) {
            int x = R;
            sxor.update(L, x);
        } else {
            int ans = sxor.countSubsetXor(L, R, 0);
            for (int x = 1 << 25; x; x >>= 1) {
                ans += sxor.countSubsetXor(L, R, x);
                if (ans >= MOD)
                    ans -= MOD;
            }
            if (--ans < 0)
                ans += MOD;
            outAns.push_back(ans);
        }
    }
}
#endif

void testSubsetXorSegmentTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Segment Tree for Subset XOR -----------------------------" << endl;

    // ...

    cout << "OK!" << endl;
}
