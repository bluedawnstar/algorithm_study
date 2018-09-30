#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "substrHashing.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <algorithm>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static string makeRandomString(int n) {
    string s;
    s.reserve(n);
    for (int i = 0; i < n; i++)
        s.push_back(RandInt32::get() % 2 + 'a');
    return s;
}

static int countStringToHavePatternNaive(const vector<string>& strs, int L, int R, const string& pattern) {
    int res = 0;
    while (L <= R) {
        if (strs[L].find(pattern) != string::npos)
            res++;
        L++;
    }
    return res;
}

void testSubstrHashing() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Substring Hashing ---------------------------------------------" << endl;
    {
        int N = 10000;
        int L = 10;
        int T = 10000;

        vector<string> s;
        for (int i = 0; i < N; i++)
            s.push_back(makeRandomString(L));

        vector<tuple<int, int, string>> q(T);
        for (int i = 0; i < T; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);
            q[i] = make_tuple(L, R, makeRandomString(RandInt32::get() % 10 + 1));
        }

        auto ans = CountStringsToHavePattern::solve(s, q);
        for (int i = 0; i < T; i++) {
            int gt = countStringToHavePatternNaive(s, get<0>(q[i]), get<1>(q[i]), get<2>(q[i]));
            if (gt != ans[i])
                cout << "Mismatched: " << gt << ", " << ans[i] << endl;
            assert(gt == ans[i]);
        }
    }
    {
        int N = 200000;
        int L = 50;
        int T = 200000;

        vector<string> s;
        for (int i = 0; i < N; i++)
            s.push_back(makeRandomString(L));

        vector<tuple<int, int, string>> q(T);
        for (int i = 0; i < T; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);
            q[i] = make_tuple(L, R, makeRandomString(RandInt32::get() % 10 + 1));
        }

        PROFILE_START(0);
        auto ans = CountStringsToHavePattern::solve(s, q);
        PROFILE_STOP(0);

        for (int i = 0; i < T; i++) {
            if (ans[i] < 0)
                cout << "ERROR! (It's to prevent optimization)" << endl;
        }
    }
    cout << "OK!" << endl;
}
