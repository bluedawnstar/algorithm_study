#include <set>
#include <tuple>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "searchZFunction.h"
#include "searchZFunctionAlgo.h"
#include "searchPrefixFunction.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <iostream>
#include <cassert>
#include "../common/rand.h"
#include "../common/iostreamhelper.h"

static string makeRandomString(int len, int charN) {
    string s;
    for (int i = 0; i < len; i++) {
        s.push_back('a' + RandInt32::get() % charN);
    }
    return s;
}

static vector<int> searchNaive(const string& text, const string& pattern) {
    vector<int> res;

    int textLength = int(text.length());
    int patternLength = int(pattern.length());

    for (int i = 0; i <= textLength - patternLength; i++) {
        bool matched = true;
        for (int j = 0; j < patternLength; j++) {
            if (text[i + j] != pattern[j]) {
                matched = false;
                break;
            }
        }

        if (matched)
            res.push_back(i);
    }

    return res;
}

void testSearchZFunction() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Z-Function ---------" << endl;
    {
        string S = "AABAACAADAABAABA", key = "AABA";

        auto gt = searchNaive(S, key);

        auto ans = searchZSimple(S, key);
        if (gt != ans) {
            cerr << "Error: " << gt << " != " << ans << endl;
        }
        assert(gt == ans);

        auto ans2 = searchZ(S, key);
        if (gt != ans2) {
            cerr << "Error: " << gt << " != " << ans2 << endl;
        }
        assert(gt == ans2);
    }
    cout << "** z-function" << endl;
    {
        const int N = 100;
        const int MAXC = 3;
        const int T = 1000;
        for (int tt = 0; tt < T; tt++) {
            for (int c = 1; c <= MAXC; c++) {
                string s = makeRandomString(N, c);

                auto z1 = makeZArrayVerbose(s);
                auto z2 = makeZArray(s);
                if (z1 != z2) {
                    cerr << "Error: " << z1 << " != " << z2 << endl;
                }
                assert(z1 == z2);
            }
        }
    }
    cout << "** search" << endl;
    {
        const int N = 100;
        const int MAXC = 3;
        const int MAXM = 10;
        const int T = 1000;
        for (int tt = 0; tt < T; tt++) {
            for (int c = 1; c <= MAXC; c++) {
                string s = makeRandomString(N, c);
                for (int m = 1; m <= MAXM; m++) {
                    string p = makeRandomString(m, c);

                    auto gt = searchNaive(s, p);

                    auto ans = searchZSimple(s, p);
                    if (gt != ans) {
                        cerr << "Error: " << gt << " != " << ans << endl;
                    }
                    assert(gt == ans);

                    auto ans2 = searchZ(s, p);
                    if (gt != ans2) {
                        cerr << "Error: " << gt << " != " << ans2 << endl;
                    }
                    assert(gt == ans2);
                }
            }
        }
    }
    cout << "** z-function algorithms" << endl;
    {
        const int N = 100;
        const int MAXC = 3;
        const int T = 1000;
        for (int tt = 0; tt < T; tt++) {
            for (int c = 1; c <= MAXC; c++) {
                string s1 = makeRandomString(N, c);

                auto z1 = makeZArray(s1);

                auto s2 = makeSmallestAlphabetStringFromZArray(z1);
                auto z2 = makeZArray(s2);

                if (z1 != z2) {
                    cerr << "Error: z1" << z1 << " != z2" << z2 << endl;
                    cerr << "       s1 = " << s1 << ", s2 = " << s2 << endl;
                }
                assert(z1 == z2);

                //--- prefix-array to z-array

                auto pi1 = ZArrayConverter::buildPrefixArrayFromZArray(z1);
                auto pi2 = prefixFunction(s1);

                if (pi1 != pi2) {
                    cerr << "Error: pi1" << pi1 << " != pi2" << pi2 << endl;
                    cerr << "       s = " << s1 << endl;
                    cerr << "       z = " << z1 << endl;
                }
                assert(pi1 == pi2);

                //--- z-array to prefix-array

                auto zt1 = ZArrayConverter::buildZArrayFromPrefixArray(pi2);

                if (zt1 != z1) {
                    cerr << "Error: zt1" << zt1 << " != z1" << z1 << endl;
                    cerr << "       s = " << s1 << endl;
                    cerr << "       pi = " << pi2 << endl;
                    zt1 = ZArrayConverter::buildZArrayFromPrefixArray(pi2);
                }
                assert(zt1 == z1);
            }
        }
    }
    cout << "OK!" << endl;
}
