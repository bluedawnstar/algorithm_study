#include <vector>
#include <queue>
#include <functional>

using namespace std;

#include "medianFinder.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <queue>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// https://leetcode.com/problems/minimum-operations-to-make-elements-within-k-subarrays-equal/description/
static long long minOperations(const vector<int>& nums, int x, int k) {
    const long long INF = 0x3f3f3f3f3f3f3f3fll;

    int N = int(nums.size());
    vector<vector<long long>> dp(N, vector<long long>(k + 1, INF));

    MedianFinder<int> mf;
    for (int i = 0; i < x; i++) {
        mf.push(nums[i]);
        dp[i][0] = 0;
    }
    int med = mf.median();
    auto lt = mf.getLower();
    auto rt = mf.getUpper();
    dp[x - 1][1] = (rt.second - 1ll * med * rt.first) + (1ll * med * lt.first - lt.second);

    for (int i = x; i < N; i++) {
        mf.push(nums[i]);
        mf.pop(nums[i - x]);

        int med = mf.median();
        auto lt = mf.getLower();
        auto rt = mf.getUpper();
        auto t = (rt.second - 1ll * med * rt.first) + (1ll * med * lt.first - lt.second);
        for (int j = min(k, (i + 1) / x); j > 0; j--) {
            dp[i][j] = min(dp[i - 1][j], dp[i - x][j - 1] + t);
        }
        dp[i][0] = 0;
    }

    return dp[N - 1][k];
}

static int median(const vector<int>& v, int L, int R) {
    vector<int> sub(v.begin() + L, v.begin() + R + 1);
    sort(sub.begin(), sub.end());
    return sub[(R - L + 1) / 2];
}

void testMedianFinder() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- median finder ------------------------" << endl;
    {
        vector<int> in{ 5, -2, 1, 3, 7, 3, 6, 4, -1 };
        const int x = 3;
        const int k = 2;
        auto ans = minOperations(in, x, k);
        auto gt = 8ll;
        if (ans != gt) {
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        }
        assert(ans == gt);
    }
    {
        vector<int> in{ 9,-2,-2,-2,1,5 };
        const int x = 2;
        const int k = 2;
        auto ans = minOperations(in, x, k);
        auto gt = 3ll;
        if (ans != gt) {
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        }
        assert(ans == gt);
    }
    cout << "OK!" << endl;
    {
        const int T = 1000;
        const int N = 1000;
        const int W = 20;

        vector<int> in(N);
        for (int t = 0; t < T; t++) {
            for (int i = 0; i < N; i++)
                in[i] = RandInt32::get();

            MedianFinder<int> mf;
            for (int i = 0; i < W; i++)
                mf.push(in[i]);
            int ans = mf.median();
            int gt = median(in, 0, W - 1);
            if (ans != gt) {
                cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);

            for (int i = W; i < N; i++) {
                mf.push(in[i]);
                mf.pop(in[i - W]);
                int ans = mf.median();
                int gt = median(in, i - W + 1, i);
                if (ans != gt) {
                    cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
                }
                assert(ans == gt);
            }
        }
    }
    cout << "OK!" << endl;
}
