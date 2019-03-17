#include <string>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "patternSubsequence.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <vector>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

// https://www.codechef.com/MARCH19A/problems/MATPER

void testPatternSubsequence() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Pattern Subsequence -------------------------" << endl;
    {
        string S("abbabacbac");
        vector<string> P{ "ab", "ac", "b" };
        assert(countPatternSubsequenceInString(S, P) == 3);
    }
    {
        string S("aaa");
        vector<string> P{ "aa", "aa" };
        assert(countPatternSubsequenceInString(S, P) == 0);
    }
    {
        string S("aaaa");
        vector<string> P{ "aa", "aa" };
        assert(countPatternSubsequenceInString(S, P) == 2);
    }

    cout << "OK!" << endl;
}
