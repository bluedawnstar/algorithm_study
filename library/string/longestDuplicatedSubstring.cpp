#include <numeric>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

#include "longestDuplicatedSubstring.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <algorithm>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

static string makeRandomString(int n) {
    string s;
    s.reserve(n);
    for (int i = 0; i < n; i++)
        s.push_back(RandInt32::get() % 26 + 'a');
    return s;
}

void testLongestDuplicatedSubstring() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Longest Duplicated Substring --------------------------------" << endl;
    {
        assert(LongestDuplicatedSubstring::solve("banana") == "ana");
        assert(LongestDuplicatedSubstring::solveWithSA("banana") == "ana");
        assert(LongestDuplicatedSubstring::solve("abcd") == "");
        assert(LongestDuplicatedSubstring::solveWithSA("abcd") == "");
    }
    {
        int N = 10000;
        int T = 100;
#ifdef _DEBUG
        N = 1000;
        T = 20;
#endif
        for (int i = 0; i < T; i++) {
            auto s = makeRandomString(N);

            auto ans1 = LongestDuplicatedSubstring::solve(s);
            auto ans2 = LongestDuplicatedSubstring::solveWithSA(s);
            if (ans1.length() != ans2.length())
                cout << "Mismatched! : " << i << endl;
            assert(ans1.length() == ans2.length());
        }
    }

    cout << "OK!" << endl;
}
