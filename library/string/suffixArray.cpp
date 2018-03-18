#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "suffixArray.h"
#include "suffixArrayExt.h"
#include "suffixArrayAlgo.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
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

void testSuffixArray() {
    return; //TODO: if you want to test string functions, make this line a comment.

    cout << "-- Suffix Array --------------------" << endl;
    {
        string s("abdadafaaabdfaeef");

        vector<int> ans = SuffixArray::buildSuffixArray(s);
        vector<int> gt = makeSuffixArrayNaive(s, (int)s.length());
        assert(ans == gt);

        long long cntAns = SuffixArrayAlgo::countSubstrings(ans, s);
        long long cntGT = SuffixArrayAlgo::countSubstringsNaive(gt, s);
        assert(cntAns == cntGT);
    }
    {
        int T = 10;
        while (T-- > 0) {
            int N = 1000;
            auto s = makeRandomString(N);

            vector<pair<string, int>> v;
            v.reserve(N);
            for (int i = 0; i < N; i++)
                v.emplace_back(s.substr(i, N - i), i);

            sort(v.begin(), v.end());

            auto SA = SuffixArray::buildSuffixArray(s);
            for (int i = 0; i < N; i++)
                assert(v[i].second == SA[i]);
        }
    }
    cout << "OK!" << endl;
}
