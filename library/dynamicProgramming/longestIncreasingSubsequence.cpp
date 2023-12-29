#include <tuple>
#include <limits>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "longestIncreasingSubsequence.h"
#include "longestIncreasingSubsequenceKth.h"
#include "longestIncreasingSubsequenceDAG.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static vector<int> generateVector(int maxX, int N) {
    vector<int> res(N);
    for (int i = 0; i < N; i++)
        res[i] = RandInt32::get() % maxX;
    return res;
}

static vector<int> generateUniqueVector(int maxX, int N) {
    unordered_set<int> S;
    vector<int> res(N);
    for (int i = 0; i < N; i++) {
        int x = RandInt32::get() % maxX;
        while (S.find(x) != S.end())
            x = RandInt32::get() % maxX;
        S.insert(x);
    }
    return res;
}

void testLongestIncreasingSubsequence() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Longest Increasing Subsequence ------------------------" << endl;
    {
        vector<int> v{ 10, 22, 9, 34, 33, 21, 50, 34, 60, 90,  80 };
        /*
                |  10  22   9  34  33  21  50  34  60  90  80  
         -------+--------------------------------------------------
             9  |           1
            10  |   1
            21  |                       2
            22  |       2 
            33  |                   3
            34  |               3
            34  |                               4
            50  |                           4
            60  |                                   5
            80  |                                           6
            90  |                                       6

            10 22 33 34 60 80
            10 22 33 34 60 90

            10 22 33 50 60 80
            10 22 33 50 60 90

            10 22 34 50 60 80
            10 22 34 50 60 90
        */
        auto smallestGT = vector<int>{ 10, 22, 33, 34, 60, 80 };
        auto biggestGT = vector<int>{ 10, 22, 34, 50, 60, 90 };

        int len = LIS::findLength(v);
        assert(len == 6);

        auto smallest = LIS::findFirst(v);
        auto biggest = LIS::findLast(v);

        assert(smallest == smallestGT);
        assert(biggest == biggestGT);

        auto listCount = KthLIS::countLIS(v);
        auto smallest2 = KthLIS::findKth(v, 1);
        auto biggest2 = KthLIS::findKth(v, listCount);
        assert(smallest2 == smallestGT);
        assert(biggest2 == biggestGT);
    }
    {
        vector<int> A{ 1, 3, 2, 7, 4, 11, 4, 12, 4, 2, 1, 9, 10 };

        LisDAG<int> tr(0x3f3f3f3f, A);
        assert(tr.getTotalCount() == KthLIS::countLIS(A));
        assert(tr.getTotalCount() == LIS::countLIS(A));

        auto list = tr.findAll();
        sort(list.begin(), list.end());
        for (int i = 0; i < int(list.size()); i++) {
            auto v1 = tr.findKth(i + 1);
            if (v1 != list[i])
                cout << "Mismatched at " << __LINE__ << " : lis[" << i << "] = " << v1 << endl;
            assert(v1 == list[i]);

            auto v2 = KthLIS::findKth(A, i + 1);
            if (v2 != list[i])
                cout << "Mismatched at " << __LINE__ << " : lis[" << i << "] = " << v2 << endl;
            assert(v2 == list[i]);
        }
    }
    // distinct values
    {
        const int T = 100;
        const int N = 50;
        const int MAXX = 1'000'000'000;
        for (int t = 0; t < T; t++) {
            auto A = generateUniqueVector(MAXX, N);
            
            int len = LIS::findLength(A);
            LisDAG<int> dag(0x3f3f3f3f, A);
            assert(len == dag.getLength());

            long long count1 = dag.getTotalCount();
            long long count2 = KthLIS::countLIS(A);
            long long count3 = LIS::countLIS(A);
            assert(count1 == count2);
            assert(count1 == count3);

            auto smallest = LIS::findFirst(A);
            auto biggest = LIS::findLast(A);
            assert(smallest == dag.findKthOfDistinctValues(1));
            assert(biggest == dag.findKthOfDistinctValues(count1));
            assert(smallest == KthLIS::findKthOfDistinctValues(A, 1));
            assert(biggest == KthLIS::findKthOfDistinctValues(A, count1));

            for (int i = 1, cnt = int(min(1'000'000ll, count1)); i <= cnt; i++) {
                auto lis1 = dag.findKthOfDistinctValues(i);
                auto lis2 = KthLIS::findKthOfDistinctValues(A, i);
                assert(lis1 == lis2);
            }
        }
    }
    {
        const int T = 100;
        const int N = 50;
        const int MAXX = 10;
        for (int t = 0; t < T; t++) {
            auto A = generateVector(MAXX, N);

            int len = LIS::findLength(A);
            LisDAG<int> dag(0x3f3f3f3f, A);
            assert(len == dag.getLength());

            long long count1 = dag.getTotalCount();
            long long count2 = KthLIS::countLIS(A);
            assert(count1 == count2);

            auto smallest = LIS::findFirst(A);
            auto biggest = LIS::findLast(A);
            assert(smallest == dag.findKth(1));
            assert(biggest == dag.findKth(count1));
            assert(smallest == KthLIS::findKth(A, 1));
            assert(biggest == KthLIS::findKth(A, count1));

            for (int i = 1, cnt = int(min(1'000'000ll, count1)); i <= cnt; i++) {
                auto lis1 = dag.findKth(i);
                auto lis2 = KthLIS::findKth(A, i);
                assert(lis1 == lis2);
            }
        }
    }
    cout << "OK!" << endl;
}
