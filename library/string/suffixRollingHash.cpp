#include <numeric>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "suffixRollingHash.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "suffixArray.h"
#include "suffixArrayAlgo.h"
#include "suffixArraySimple.h"
#include "suffixSparseTable.h"

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

void testSuffixRollingHash() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Suffix Rolling Hash -------------------" << endl;
    {
        string S("abdaaaaaaaaaaaaaaacccccccccccccccccccccaaaddddddddddddddddaaaaaaadddddddaaafaeef");

        vector<int> a = SuffixArray::buildSuffixArray(S);
        SuffixRollingHash srh(S);

        for (int i = 0; i < 100; i++) {
            int left = RandInt32::get() % S.length();
            int right = RandInt32::get() % S.length();
            if (left > right)
                swap(left, right);

            int lcp = srh.lcp(left, right);

            //cout << "LCP(" << left << ", " << right << ") = " << lcp << endl;
            assert(lcp == SuffixArrayAlgo::commonPrefixNaive(S, left, right));
        }
    }
    //---
    {
        string s("abdadafaaabdfaeef");

        vector<int> gt = makeSuffixArrayNaive(s, int(s.length()));

        SuffixRollingHash srh(s);
        auto sa = srh.buildSuffixArray();

        if (sa != gt)
            cout << "ERROR" << endl;
        assert(sa == gt);
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

            SuffixRollingHash srh(s);
            auto sa = srh.buildSuffixArray();
            for (int i = 0; i < N; i++)
                assert(v[i].second == sa[i]);
        }
    }
    cout << "*** Speed test ..." << endl;
    {
        for (int i = 26; i > 0; i--) {
            cout << "used char count : " << i << endl;
            int N = 1000000;
#ifdef _DEBUG
            N = 1000;
#endif
            auto s = makeRandomString(N, i);

            PROFILE_START(0);
            auto sa1 = SuffixArray::buildSuffixArray(s);
            PROFILE_STOP(0);
            PROFILE_START(1);
            auto lcpa1 = SuffixArray::buildLcpArray(sa1, s);
            PROFILE_STOP(1);

            PROFILE_START(2);
            SimpleSuffixArray sa2(s);
            PROFILE_STOP(2);
            PROFILE_START(3);
            auto lcpa2 = sa2.buildLcpArrayNaive();
            PROFILE_STOP(3);

            PROFILE_START(4);
            PROFILE_START(401);
            SuffixSparseTable sst(s);
            PROFILE_STOP(401);
            PROFILE_START(402);
            auto sa3 = sst.buildSuffixArray();
            PROFILE_STOP(402);
            PROFILE_STOP(4);
            PROFILE_START(5);
            auto lcpa3 = sst.buildLcpArray(sa3);
            PROFILE_STOP(5);

            PROFILE_START(6);
            SuffixRollingHash srh(s);
            auto sa4 = srh.buildSuffixArray();
            PROFILE_STOP(6);
            PROFILE_START(7);
            auto lcpa4 = srh.buildLcpArray(sa4);
            PROFILE_STOP(7);

            if (sa1 != sa2.sa || sa1 != sa3 || sa1 != sa4 || lcpa1 != lcpa2 || lcpa1 != lcpa3 || lcpa1 != lcpa4)
                cout << "ERROR!" << endl;
            assert(sa1 == sa2.sa);
            assert(sa1 == sa3);
            assert(sa1 == sa4);
            assert(lcpa1 == lcpa2);
            assert(lcpa1 == lcpa3);
            assert(lcpa1 == lcpa4);
        }
    }

    cout << "OK!" << endl;
}
