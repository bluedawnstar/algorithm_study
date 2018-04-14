#include <cmath>
#include <tuple>
#include <vector>
#include <queue>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "subsequence.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static ostream& operator <<(ostream& os, const unordered_set<string>& rhs) {
    for (auto it : rhs)
        os << it << ", ";
    os << endl;
    return os;
}

void testSubsequence() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Subsequence ------------------------" << endl;
    {
        assert(countDistinctSubsequence("aabab") == 17);
        // 0: ""
        // 1: "a", "b"
        // 2: "aa", "ab", "ba", "bb"
        // 3: "aaa", "aab", "aba", "abb", "bab"
        // 4: "aaab", "aaba", "aabb", "abab"
        // 5: "aabab"
        assert(countSubsequence("aabab", "ab") == 5);
    }

    cout << "OK!" << endl;
}
