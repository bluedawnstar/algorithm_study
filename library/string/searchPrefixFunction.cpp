#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "searchPrefixFunction.h"
#include "searchPrefixFunctionAlgo.h"

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

void testSearchPrefixFunction() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Prefix Function ---------" << endl;
    {
        cout << "-- checkRepeatedString() ---------" << endl;
        cout << checkRepeatedString("abcdefg") << endl;                 // (-1, -1)
        cout << checkRepeatedString("abcabc") << endl;                  // (3, 2)
        cout << checkRepeatedString("abacdacdb") << endl;               // (-1, -1)
        cout << checkRepeatedString("aaaaaaaa") << endl;                // (1, 8)
        cout << checkRepeatedString("abcabcabcabc") << endl;            // (3, 4)
        cout << "-- getMaxRepeatedSubstring() ---------" << endl;
        cout << getMaxRepeatedSubstring("abcdefg") << endl;             // (-1, -1, -1)
        cout << getMaxRepeatedSubstring("ababcabcab") << endl;          // (2, (3, 2))
        cout << getMaxRepeatedSubstring("abacdacdb") << endl;           // (2, (3, 2))
        cout << getMaxRepeatedSubstring("aaaaaaaa") << endl;            // (0, (1, 8))
        cout << getMaxRepeatedSubstring("xabcabcabcabcx") << endl;      // (1, (3, 4))
        cout << "-- kmp() ---------" << endl;
        cout << kmp("abcdefg", "cd") << endl;                           // (2)
        cout << kmp("ababcabcab", "ab") << endl;                        // (0, 2, 5, 8)
        cout << kmp("abacdacdb", "ac") << endl;                         // (2, 5)
        cout << kmp("aaaaaaaa", "a") << endl;                           // (0, 1, 2, 3, 4, 5, 6, 7)
        cout << kmp("aaaaaaaa", "aaa") << endl;                         // (0, 1, 2, 3, 4, 5)
        cout << kmp("xabcabcabcabcx", "abc") << endl;                   // (1, 4, 7, 10)

        assert(getMaxOverlapLength("dcbabcd", "abcdefg") == 4);
        assert(getMaxOverlapLength("dcbabce", "abcdefg") == 0);
        assert(getMaxOverlapLength("dcbabcd", "abcd") == 4);
        assert(getMaxOverlapLength("dcbabcd", "bcd") == 3);
        assert(getMaxOverlapLength("abcd", "abcdefg") == 4);
        assert(getMaxOverlapLength("abc", "abcdefg") == 3);
    }
    {
        string S = "AABAACAADAABAABA", key = "AABA";

        auto gt = searchNaive(S, key);

        auto ans = kmp(S, key);
        if (gt != ans) {
            cerr << "Error: " << gt << " != " << ans << endl;
        }
        assert(gt == ans);
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
                    auto ans = kmp(s, p);
                    if (gt != ans) {
                        cerr << "Error: " << gt << " != " << ans << endl;
                    }
                    assert(gt == ans);
                }
            }
        }
    }
    cout << "** prefix-function algorithms" << endl;
    {
        const int N = 100;
        const int MAXC = 3;
        const int T = 1000;
        for (int tt = 0; tt < T; tt++) {
            for (int c = 1; c <= MAXC; c++) {
                string s1 = makeRandomString(N, c);

                auto pi1 = prefixFunction(s1);

                auto s2 = makeSmallestAlphabetStringFromPrefixArray(pi1);
                auto pi2 = prefixFunction(s2);

                if (pi1 != pi2) {
                    cerr << "Error: pi1" << pi1 << " != pi2" << pi2 << endl;
                    cerr << "       s1 = " << s1 << ", s2 = " << s2 << endl;
                }
                assert(pi1 == pi2);
            }
        }
    }
    cout << "OK!" << endl;
}
