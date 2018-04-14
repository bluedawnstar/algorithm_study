#include <cmath>
#include <tuple>
#include <vector>
#include <queue>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "subsequenceCount.h"
#include "subsequenceCheck.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static ostream& operator <<(ostream& os, const unordered_set<string>& rhs) {
    for (auto it : rhs)
        os << it << ", ";
    os << endl;
    return os;
}

static string makeRandomString(int len) {
    string s;
    for (int i = 0; i < len; i++) {
        s.push_back('a' + RandInt32::get() % 26);
    }
    return s;
}

void testSubsequence() {
    return; //TODO: if you want to test, make this line a comment.

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
    {
        int T = 100;

        string s = makeRandomString(10000);

        NextPositionTable npt1(s);
        CompactNextPositionTable npt2(s);
        for (int i = 0; i < T; i++) {
            string t = makeRandomString(385);

            bool gt = isSubsequence(s, t);
            bool ans1 = npt1.isSubsequence(t);
            bool ans2 = npt2.isSubsequence(t);
            assert(gt == ans1);
            assert(gt == ans2);
        }
    }

    cout << "OK!" << endl;
}
