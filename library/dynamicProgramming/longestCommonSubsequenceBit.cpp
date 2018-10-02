#include <cmath>
#include <tuple>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

#include "longestCommonSubsequenceBit.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include <unordered_map>
#include "longestCommonSubsequence.h"

static string makeRandomString(int n) {
    string s;
    s.reserve(n);
    for (int i = 0; i < n; i++)
        s.push_back(RandInt32::get() % 3 + 'a');
    return s;
}

void testBitLCS() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Bit-String Longest Common Subsequence ------------------------" << endl;
    {
        string s1 = "gtcttacatccgttcg";
        string s2 = "tgttctagaattcgat";

        int ans = BitLCS::solve(s1, s2);
        int gt = 10;
        assert(ans == gt);
    }
    {
        int ans = BitLCS::solve("aggtab", "gxtxayb");
        assert(ans == 4);
    }
    {
        string s1 = "aaaaaa";
        string s2 = "aaa";

        int ans = BitLCS::solve(s1, s2);
        int gt = 3;
        assert(ans == gt);
    }
    {
        string s1 = "abcabc";
        string s2 = "abc";

        int ans = BitLCS::solve(s1, s2);
        int gt = 3;
        assert(ans == gt);
    }
    {
        int N = 100;
        int M = N * 2;
#ifndef _DEBUG
        N = 1000;
        M = N * 2;
#endif
        int T = 10;
        for (int i = 0; i < T; i++) {
            string s = makeRandomString(N);
            string t = makeRandomString(M);

            int gt = lengthOfLcs(s, t);
            int ans = BitLCS::solve(s, t);

            if (ans != gt)
                cout << "Mismatched between \"" << s << "\", " << t << "\" : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    {
        int N = 1000;
        int M = N * 2;
#ifndef _DEBUG
        N = 10000;
        M = N * 2;
#endif
        string s = makeRandomString(N);
        string t = makeRandomString(M);

        PROFILE_START(0);
        int gt = lengthOfLcs(s, t);
        PROFILE_STOP(0);

        PROFILE_START(1);
        int ans = BitLCS::solve(s, t);
        PROFILE_STOP(1);

        if (ans != gt)
            cout << "Mismatched between \"" << s << "\", " << t << "\" : " << ans << ", " << gt << endl;
    }

    cout << "OK!" << endl;
}
