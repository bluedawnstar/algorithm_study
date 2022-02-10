#include <vector>
#include <algorithm>

using namespace std;


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../../common/iostreamhelper.h"
#include "../../common/profile.h"

// A good subset
// https://www.hackerearth.com/practice/algorithms/dynamic-programming/2-dimensional/practice-problems/algorithm/good-subset-88fda603/

struct KnapsackAndBinarySearchExample1 {
    static bool decide(const vector<int>& A, int K, int x) {
        int N = int(A.size());
        vector<char> dp(K + 1);

        dp[0] = 1;
        for (int i = 0; i < N; i++) {
            if (i == x)
                continue;
            for (int j = K - A[i]; j >= 0; j--)
                dp[j + A[i]] |= dp[j];
        }
        for (int i = max(0, K - A[x]); i < K; i++) {
            if (dp[i])
                return false;
        }
        return true;
    }

    /*
       If A[i] < A[j] and A[i] is necessary, then A[j] is also necessary.

       O(N*K*logN)
    */
    static int solve(const vector<int>& in, int K) {
        int N = int(in.size());

        int necessary = 0;

        vector<int> A;
        A.reserve(in.size());
        for (auto x : in) {
            if (x >= K)
                necessary++;
            else
                A.push_back(x);
        }
        if (A.empty())
            return 0;

        sort(A.begin(), A.end(), greater<int>());

        int lo = 0, hi = int(A.size()) - 1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (decide(A, K, mid))
                hi = mid - 1;
            else
                lo = mid + 1;
        }
        necessary += lo;

        return N - necessary;
    }
};

void testKnapsackAndBinarySearchExample1() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Knapsack and Binary Search Example #1 --------------------" << endl;
    {
        // ...
    }

    cout << "OK!" << endl;
}
