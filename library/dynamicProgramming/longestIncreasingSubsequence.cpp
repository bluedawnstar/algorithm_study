#include <vector>
#include <algorithm>

using namespace std;

#include "longestIncreasingSubsequence.h"
#include "longestIncreasingSubsequenceDAG.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testLongestIncreasingSubsequence() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Longest Increasing Subsequence ------------------------" << endl;
    {
        vector<int> v{ 10, 22, 9, 33, 21, 50, 41, 60, 80 };
        int len = LIS::findLength(v);
        assert(len == 6);

        auto smallest = LIS::findFirst(v);
        auto biggest = LIS::findLast(v);

        cout << "smallest = " << smallest << endl;
        cout << "biggest = " << biggest << endl;
    }
    {
        vector<int> v{ 1, 3, 2, 7, 4, 11, 4, 12, 4, 2, 1, 9, 10 };

        LisDAG<int> tr(v);

        assert(tr.getTotalCount() == 12);

        auto list = tr.findAll();
        for (auto& v : list)
            cout << v << endl;

        for (int i = 0; i < int(list.size()); i++) {
            auto v = tr.getKth(i);
            cout << "lis[" << i << "] = " << v << endl;
            assert(v == list[i]);
        }
    }
    cout << "OK!" << endl;
}
