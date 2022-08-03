#include <vector>
#include <algorithm>

using namespace std;

#include "mergeSortTreeCompactWithLiChaoTreeMin.h"
#include "mergeSortTreeCompactWithLiChaoTreeMax.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <functional>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

void testCompactMergeSortTreeWithLiChaoTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Compact Merge Sort Tree with Li-Chao Segment Tree -----------------------------" << endl;
    {
        // https://www.hackerearth.com/problem/algorithm/query-in-hackland-53d71418/
        /*
            return the first point that is at least K distance from (X, 0)
            -------
            Distance between (x[i], y[i]) and (X, 0) is greater than or equal to K

                sqrt{ (X - x[i]) * (X - x[i]) + y[i] * y[i] } >= K
            <=> (X - x[i]) * (X - x[i]) + y[i]^2 >= K^2
            <=> X^2 - 2*X*x[i] + x[i]^2+ y[i]^2 >= K^2
            <=> X^2 - K^2 >= 2*x[i]*X - x[i]^2 - y[i]^2
            <=> 2*x[i]*X - x[i]^2 - y[i]^2 <= X^2 - K^2
            <=> m*X + b <= c

                m = 2*x[i]
                b = - x[i]^2 - y[i]^2
                c = X^2 - K^2

            O(N*(logN)^2)
        */
        vector<pair<int,int>> points{
            { 4, 5 }, { 1, 2 }, { 6, 2 }, { 5, 9 }, { 9, 5 }
        };
        vector<pair<long long, long long>> lines;
        for (auto& it : points)
            lines.emplace_back(2ll * it.first, -(1ll * it.first * it.first + 1ll * it.second * it.second));

        CompactMergeSortTreeWithLiChaoTreeMin tree;
        tree.build(lines);

        auto solver = [&tree](int left, int right, int X, int K) -> int {
            int lo = left, hi = right;
            while (lo <= hi) {
                int mid = (lo + hi) >> 1;
                long long y = tree.query(lo, mid, X);
                if (y <= 1ll * X * X - 1ll * K * K)
                    hi = mid - 1;
                else
                    lo = mid + 1;
            }

            return (lo <= right) ? lo : -1;
        };

        int X = 1, K = 9;

        int ans = solver(1, 4, X, K);
        int gt = 3;

        if (ans != gt)
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
