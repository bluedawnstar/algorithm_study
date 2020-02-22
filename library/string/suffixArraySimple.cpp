#include <numeric>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "suffixArray.h"
#include "suffixArrayAlgo.h"
#include "suffixArraySimple.h"

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

void testSimpleSuffixArray() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Simple Suffix Array --------------------" << endl;
    {
        string s("abdadafaaabdfaeef");

        vector<int> gt = makeSuffixArrayNaive(s, int(s.length()));

        SimpleSuffixArray sa(s);
        auto sa2 = SimpleSuffixArray2::build(s);

        if (sa.sa != gt || sa2 != gt)
            cout << "Mismatched : " << sa.sa << ", " << sa2 << ", " << gt << endl;

        assert(sa.sa == gt && sa2 == gt);
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

            SimpleSuffixArray sa(s);
            auto sa2 = SimpleSuffixArray2::build(s);

            for (int i = 0; i < N; i++) {
                if (v[i].second != sa.sa[i] || v[i].second != sa2[i])
                    cout << "Mismatched : " << sa.sa[i] << ", " << sa2[i] << ", " << v[i].second << endl;
                assert(v[i].second == sa.sa[i] && v[i].second == sa2[i]);
            }
        }
    }
    cout << "*** Speed test ..." << endl;
    {
        for (int i = 26; i > 0; i--) {
            cout << "used char count : " << i << endl;
            int N = 1000000;
#ifdef _DEBUG
            N = 10000;
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
            auto sa3 = SimpleSuffixArray2::build(s);
            PROFILE_STOP(4);

            if (sa1 != sa2.sa || lcpa1 != lcpa2 || sa1 != sa3)
                cout << "ERROR!" << endl;
            assert(sa1 == sa2.sa);
            assert(sa1 == sa3);
            assert(lcpa1 == lcpa2);
        }
    }
    cout << "OK!" << endl;
}
