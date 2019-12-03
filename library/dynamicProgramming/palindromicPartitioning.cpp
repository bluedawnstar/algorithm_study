#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "palindromicPartitioning.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testPalindromicPartitioning() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Palindromic Partitioning -----------------------------" << endl;
    {
        vector<pair<string, int>> in{
            { "aab", 1 },
            { "aba", 0 },
            { "abc", 2 }
        };

        for (auto& it : in) {
            int ans = PalindromicPartitioning::minCut(it.first);
            if (ans != it.second)
                cout << "Mismatched : " << ans << ", " << it.second << endl;
            assert(ans == it.second);
        }
    }
    {
        vector<tuple<string, int, int>> in{
            { "abc"     , 2, 1 },
            { "aabbc"   , 3, 0 },
            { "abbcdefb", 8, 0 },
            { "abcdefa" , 4, 2 }
        };

        for (auto& it : in) {
            int ans1 = PalindromicPartitioning::palindromePartitionMemoization(get<0>(it), get<1>(it));
            int ans2 = PalindromicPartitioning::palindromePartitionDP(get<0>(it), get<1>(it));
            if (ans1 != get<2>(it) || ans2 != get<2>(it))
                cout << "Mismatched : " << ans1 << ", " << ans2 << ", " << get<2>(it) << endl;
            assert(ans1 == get<2>(it));
            assert(ans2 == get<2>(it));
        }
    }

    cout << "OK!" << endl;
}
