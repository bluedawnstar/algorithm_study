#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "suffixArray.h"
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

static string makeRandomString(int n, int charCnt) {
    string s;
    s.reserve(n);
    for (int i = 0; i < n; i++)
        s.push_back(RandInt32::get() % charCnt + 'a');
    return s;
}

static int lowerBoundForwardSlow(const vector<int>& sa, int left, int length, const string& s) {
    for (int i = left + 1; i < int(sa.size()); i++) {
        if (SuffixArrayAlgo::commonPrefixNaive(s, sa[left], sa[i]) <= length)
            return i;
    }

    return int(sa.size());
}

static int lowerBoundBackwardSlow(const vector<int>& sa, int right, int length, const string& s) {
    for (int i = right - 1; i >= 0; i--) {
        if (SuffixArrayAlgo::commonPrefixNaive(s, sa[i], sa[right]) <= length)
            return i;
    }

    return -1;
}

void testSuffixArray() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Suffix Array --------------------" << endl;
    {
        string s("abdadafaaabdfaeef");

        vector<int> ans = SuffixArray::buildSuffixArray(s);
        vector<int> gt = makeSuffixArrayNaive(s, int(s.length()));
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
    {
        int N = 100;
        int T = 10;
        int L = 5;
        string s = makeRandomString(N, 3);

        SuffixArray SA(s);

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < T; j++) {
                int len = RandInt32::get() % L;
                assert(SA.lowerBoundLcpForward(i, len) == lowerBoundForwardSlow(SA.suffixArray, i, len, s));
                assert(SA.upperBoundLcpForward(i, len) == lowerBoundForwardSlow(SA.suffixArray, i, len - 1, s));
                assert(SA.lowerBoundLcpBackward(i, len) == lowerBoundBackwardSlow(SA.suffixArray, i, len, s));
                assert(SA.upperBoundLcpBackward(i, len) == lowerBoundBackwardSlow(SA.suffixArray, i, len - 1, s));
            }
        }
    }
    cout << "OK!" << endl;
}
