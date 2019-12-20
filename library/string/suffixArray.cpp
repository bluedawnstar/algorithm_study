#include <tuple>
#include <memory>
#include <queue>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

using namespace std;

#include "suffixArray.h"
#include "suffixArrayAlgo.h"

#include "suffixArray_LarssonSadakane.h"
#include "suffixArray_ManberMyers.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
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

static string makeRandomRepeatedString(int n, int charCnt) {
    string s;
    s.reserve(n);
    for (int i = 0; i < n && i < charCnt; i++)
        s.push_back(RandInt32::get() % charCnt + 'a');
    for (int i = charCnt; i < n; i++)
        s.push_back(s[i % charCnt]);
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
    //return; //TODO: if you want to test, make this line a comment.

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
        //        01234567890123456
        string s("abdadafaaabdfaeef");

        SuffixArrayLarssonSadakane larsson;
        vector<int> ans1 = larsson.build(s);

        vector<int> ans2 = SuffixArrayManberMyers::build(s);

        vector<int> gt = makeSuffixArrayNaive(s, int(s.length()));
        if (ans1 != gt || ans2 != gt)
            cout << "Mismatched : " << ans1 << ", " << ans2 << ", " << gt << endl;
        assert(ans1 == gt && ans2 == gt);

        long long cntAns = SuffixArrayAlgo::countSubstrings(ans1, s);
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

            SuffixArrayLarssonSadakane larsson;
            vector<int> SA2 = larsson.build(s);

            vector<int> SA3 = SuffixArrayManberMyers::build(s);

            for (int i = 0; i < N; i++) {
                if (v[i].second != SA[i] || v[i].second != SA2[i] || v[i].second != SA3[i])
                    cout << "Mismatched  at " << __LINE__ << endl;
                assert(v[i].second == SA[i] && v[i].second == SA2[i] && v[i].second == SA3[i]);
            }
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
    cout << "*** speed test" << endl;
    {
#ifdef _DEBUG
        vector<pair<int, int>> testCases{
            { 100, 100 }
        };
#else
        vector<pair<int, int>> testCases{
            { 10000, 10000 },
            { 1000, 100000 },
            { 100, 1000000 }
        };
#endif
        for (auto& tc : testCases) {
            int T = tc.first;
            int N = tc.second;

            vector<string> in;
            for (int i = 0; i < T; i++)
                in.push_back(makeRandomString(N));

            int sum1 = 0;
            int sum2 = 0;
            int sum3 = 0;

            PROFILE_START(0);
            for (int i = 0; i < T; i++) {
                auto SA = SuffixArray::buildSuffixArray(in[i]);
                sum1 += SA[0];
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            SuffixArrayLarssonSadakane larsson;
            for (int i = 0; i < T; i++) {
                vector<int> SA = larsson.build(in[i]);
                sum2 += SA[0];
            }
            PROFILE_STOP(1);

            PROFILE_START(2);
            for (int i = 0; i < T; i++) {
                vector<int> SA = SuffixArrayManberMyers::build(in[i]);
                sum3 += SA[0];
            }
            PROFILE_STOP(2);

            if (sum1 != sum2 || sum1 != sum3)
                cout << "Mismatched  at " << __LINE__ << endl;
        }
    }
    cout << "*** speed test for repeated strings" << endl;
    {
#ifdef _DEBUG
        vector<pair<int, int>> testCases{
            { 100, 100 },
        };
#else
        vector<pair<int, int>> testCases{
            { 10000, 10000 },
            { 1000, 100000 },
            { 100, 1000000 }
        };
#endif
        for (auto& tc : testCases) {
            int T = tc.first;
            int N = tc.second;

            vector<string> in;
            for (int i = 0; i < T; i++)
                in.push_back(makeRandomRepeatedString(N, 3));

            int sum1 = 0;
            int sum2 = 0;
            int sum3 = 0;

            PROFILE_START(0);
            for (int i = 0; i < T; i++) {
                auto SA = SuffixArray::buildSuffixArray(in[i]);
                sum1 += SA[0];
            }
            PROFILE_STOP(0);

            PROFILE_START(1);
            SuffixArrayLarssonSadakane larsson;
            for (int i = 0; i < T; i++) {
                vector<int> SA = larsson.build(in[i]);
                sum2 += SA[0];
            }
            PROFILE_STOP(1);

            PROFILE_START(2);
            for (int i = 0; i < T; i++) {
                vector<int> SA = SuffixArrayManberMyers::build(in[i]);
                sum3 += SA[0];
            }
            PROFILE_STOP(2);

            if (sum1 != sum2 || sum1 != sum3)
                cout << "Mismatched  at " << __LINE__ << endl;
        }
    }
    cout << "OK!" << endl;
}
