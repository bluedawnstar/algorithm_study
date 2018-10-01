#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "longestCommonSubsequenceCyclic.h"

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

void testCyclicLCS() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Cyclic Longest Common Subsequence ------------------------" << endl;
    {
        string s = "a1b2c3a4b";
        string t = "b5c6a7b8a";

        CyclicLCS clcs;
        assert(clcs.solve(s, t) == 5);
    }
    {
        int N = 20;
        int M = 20;
#ifndef _DEBUG
        N = 100;
        M = 100;
#endif
        int T = 10;

        CyclicLCS clcs;

        for (int i = 0; i < T; i++) {
            string s = makeRandomString(N);
            string t = makeRandomString(M);

            int gt = 0;
            {
                string ss = s + s;
                string tt = t + t;
                for (int i = 0; i < N; i++) {
                    auto ssi = ss.substr(i, N);
                    for (int j = 0; j < M; j++)
                        gt = max(gt, lengthOfLcs(ssi, tt.substr(j, M)));
                }
            }
            int ans = clcs.solve(s, t);
            if (ans != gt)
                cout << "Mismatched between \"" << s << "\", " << t << "\" : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
