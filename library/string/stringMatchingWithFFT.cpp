#include <cmath>
#include <string>
#include <vector>

using namespace std;

#include "stringMatchingWithFFT.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "../string/searchPrefixFunction.h"

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

static string makeRandomString(int n, int charCnt) {
    string s;
    s.reserve(n);
    for (int i = 0; i < n; i++)
        s.push_back(RandInt32::get() % charCnt + 'a');
    return s;
}

void testStringMatchingWithFFT() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- String Matching With FFT --------------------" << endl;
    {
        string S = "aabaacaadaabaaba", key = "aaba";

        auto gt = searchNaive(S, key);
        auto ans = StringMatchingWithFFT::search(S, key);
        if (gt != ans) {
            cerr << "Error: " << gt << " <-> " << ans << endl;
        }
        assert(gt == ans);
    }
    {
        string S = "aabaacaadaabaaba", key = "aaba";

        auto gt = searchNaive(S, key);
        auto ans = StringMatchingWithFFT::searchWithWildcard(S, key);
        if (gt != ans) {
            cerr << "Error: " << gt << " <-> " << ans << endl;
        }
        assert(gt == ans);
    }
    {
        string S = "aabaacaadaabaaba", key = "aa?a";
        //          0123456789012345
        vector<int> gt{ 0, 3, 6, 9, 12 };
        auto ans = StringMatchingWithFFT::searchWithWildcard(S, key);
        if (gt != ans) {
            cerr << "Error: " << gt << " <-> " << ans << endl;
        }
        assert(gt == ans);
    }
    {
        string S = "aabaacaadaabaaba", key = "a?a";
        //          0123456789012345
        vector<int> gt{ 1, 4, 7, 10, 13 };
        auto ans = StringMatchingWithFFT::searchWithWildcard(S, key);
        if (gt != ans) {
            cerr << "Error: " << gt << " <-> " << ans << endl;
        }
        assert(gt == ans);
    }
    {
        int T = 10;
        int TEXTN = 1000000;
        int PATTN = 10;

#ifdef _DEBUG
        TEXTN = 1000;
        PATTN = 5;
#endif

        for (int i = 0; i < T; i++) {
            cout << i << "...\r";

            auto text = makeRandomString(TEXTN, 2);
            auto patt = makeRandomString(PATTN, 2);

            auto gt = searchNaive(text, patt);
            auto ans = StringMatchingWithFFT::search(text, patt);
            if (gt != ans) {
                cout << "Mismatched at " << i << ": " << ans << ", " << gt << endl;
            }
            if (gt.empty())
                cout << "No matched at " << i << endl;
            assert(gt == ans);
        }
    }
    {
        int TEXTN = 1000000;
        int PATTN = 1000;

#ifdef _DEBUG
        TEXTN = 10000;
        PATTN = 100;
#endif

        string patt = makeRandomString(PATTN, 26);
        string text;

        vector<int> gt;
        for (int i = 0; i < TEXTN; i += PATTN) {
            gt.push_back(i);
            text.append(patt);
        }

        PROFILE_START(0);
        auto ans = StringMatchingWithFFT::search(text, patt);
        PROFILE_STOP(0);

        PROFILE_START(1);
        auto ans2 = kmp(text, patt);
        PROFILE_STOP(1);

        if (gt != ans || ans != ans2) {
            cout << "Mismatched at line " << __LINE__ << ": " << ans << ", " << gt << endl;
        }
        assert(gt == ans && ans == ans2);
    }

    cout << "OK!  " << endl;
}
