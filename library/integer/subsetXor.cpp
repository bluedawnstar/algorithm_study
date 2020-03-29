#include <vector>
#include <algorithm>

using namespace std;

#include "subsetXor.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

const int MOD = 1000000007;

#if 1
// https://codeforces.com/contest/959/problem/F
// https://www.hackerearth.com/challenges/competitive/march-circuits-20/algorithm/chef-f59c8115/
// query = { (L, (R[0], ans_index[0])), (L, (R[1], ans_index[1])), ..., (L, (R[n - 1], ans_index[n - 1])) }
void solve(SubsetXor<MOD>& sxor, const int A[], int L, const vector<pair<int, int>>& R, vector<int>& ans) {
    if (R.empty())
        return;

    sxor.clear();

    for (int i = L - 1, j = 0; j < int(R.size()); j++) {
        while (i < R[j].first)
            sxor.insert(A[++i]);

        int s = sxor.countSubsetXor(0);
        for (int x = (1 << 25); x; x >>= 1) {
            s += sxor.countSubsetXor(x);
            if (s >= MOD)
                s -= MOD;
        }
        ans[R[j].second] = s;
    }
}
#endif

void testSubsetXOR() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Subset XOR -----------------------------" << endl;
    {
        SubsetXor<MOD> sxor;
        sxor.init(7, 3);
        for (int i = 1; i <= 7; i++)
            sxor.insert(i);

        int distinctXor = sxor.countDistinctXorIntegers();
        if (distinctXor != 8)
            cout << "Invalid the number of distinct XORs : " << distinctXor << endl;
        assert(distinctXor == 8);

        int maxXor = sxor.getMaxSubsetXor();
        if (maxXor != 7)
            cout << "Invalid max XOR : " << maxXor << endl;
        assert(maxXor == 7);

        for (int i = 1; i <= 8; i++) {
            int kth = sxor.kthSubsetXor(i);
            if (kth != (i - 1))
                cout << "Invalid " << i << "th XOR : " << kth << endl;
            assert(kth == (i - 1));
        }
    }
    cout << "OK!" << endl;
}
