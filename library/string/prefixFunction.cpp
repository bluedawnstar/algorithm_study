#include <cmath>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>

using namespace std;

#include "prefixFunction.h"
#include "prefixFunctionKMP.h"
#include "prefixFunctionAlgo.h"
#include "prefixFunctionSparseTable.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

#define testCheckRepeatedString(s, gt) \
    do { \
        PrefixFunction pref(s); \
        assert(pref.checkRepeatedString() == (gt)); \
    } while (0)

#define testGetPrefixSuffix(s, gt) \
    do { \
        PrefixFunction pref(s); \
        auto ans = pref.getPrefixSuffix(); \
        if (ans != gt) { \
            cout << "Mismatched : " << ans << ", " << gt << endl; \
        } \
        assert(ans == gt); \
    } while (0)

#define testKMP(s, p, gt) \
    do { \
        PrefixFunction pref(p); \
        assert(kmp(s, pref) == gt); \
    } while (0)


int getMaxCommonProperPrefixSuffix(const string& s, int x, int y) {
    if (s[x] != s[y])
        return 0;

    for (int n = x; n > 0; n--) {
        auto s1 = s.substr(0, n);
        auto sx = s.substr(x - n + 1, n);
        auto sy = s.substr(y - n + 1, n);
        if (s1 == sx && s1 == sy)
            return n;
    }
    return 0;
}

static string makeString(int length, int charCount) {
    string res;
    for (int i = 0; i < length; i++)
        res.push_back('a' + RandInt32::get() % charCount);
    return res;
}

static string makeRepeatString(int length, int charCount) {
    string s;
    for (int i = 0; i < charCount; i++)
        s.push_back('a' + i);

    string res;
    for (int i = 0, n = length / charCount; i < n; i++)
        res += s;

    int r = length % charCount;
    if (r)
        res += s.substr(0, r);

    return res;
}

void testPrefixFunction() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Prefix Function ---------" << endl;
    // checkRepeatedSubstring()
    {
        testCheckRepeatedString("abcdefg", make_pair(-1, -1));
        testCheckRepeatedString("abcabc", make_pair(3, 2));
        testCheckRepeatedString("abacdacdb", make_pair(-1, -1));
        testCheckRepeatedString("aaaaaaaa", make_pair(1, 8));
        testCheckRepeatedString("abcabcabcabc", make_pair(3, 4));
    }
    // getMaxRepeatedSubstring()
    {
        assert(PrefixFunction::getMaxRepeatedSubstring("abcdefg") == make_tuple(-1, -1, -1));
        assert(PrefixFunction::getMaxRepeatedSubstring("ababcabcab") == make_tuple(2, 3, 2));
        assert(PrefixFunction::getMaxRepeatedSubstring("abacdacdb") == make_tuple(2, 3, 2));
        assert(PrefixFunction::getMaxRepeatedSubstring("aaaaaaaa") == make_tuple(0, 1, 8));
        assert(PrefixFunction::getMaxRepeatedSubstring("xabcabcabcabcx") == make_tuple(1, 3, 4));
    }
    // getPrefixSuffix()
    {
        testGetPrefixSuffix("abcabcabc", (vector<int>{ 9, 6, 3 }));
        testGetPrefixSuffix("abcabcabca", (vector<int>{ 10, 7, 4, 1 }));
    }
    // kmp()
    {
        testKMP("abcdefg", "cd", (vector<int>{ 2 }));
        testKMP("ababcabcab", "ab", (vector<int>{ 0, 2, 5, 8 }));
        testKMP("abacdacdb", "ac", (vector<int>{ 2, 5 }));
        testKMP("aaaaaaaa", "a", (vector<int>{ 0, 1, 2, 3, 4, 5, 6, 7 }));
        testKMP("aaaaaaaa", "aaa", (vector<int>{ 0, 1, 2, 3, 4, 5 }));
        testKMP("xabcabcabcabcx", "abc", (vector<int>{ 1, 4, 7, 10 }));
    }
    // maxOverlap()
    {
        assert(maxOverlapLength("anon", "nona") == 3);
        assert(maxOverlapLength("abcdefg", "defgabc") == 4);
        assert(maxOverlapLength("aaaaaaa", "aaaaaaaa") == 7);
        assert(maxOverlapLength("aaaaaaa", "aaaaaaab") == 7);
        assert(maxOverlapLength("aaaaaab", "aaaaaaaa") == 0);

        PrefixFunction pref1("nona");
        PrefixFunction pref2("defgabc");
        PrefixFunction pref3("aaaaaaaa");
        PrefixFunction pref4("aaaaaaab");
        PrefixFunction pref5("aaaaaaaa");
        assert(maxOverlapLength("anon", pref1) == 3);
        assert(maxOverlapLength("abcdefg", pref2) == 4);
        assert(maxOverlapLength("aaaaaaa", pref3) == 7);
        assert(maxOverlapLength("aaaaaaa", pref4) == 7);
        assert(maxOverlapLength("aaaaaab", pref5) == 0);
    }
    // PrefixFunctionSparseTable
    {
        PrefixFunction pf("abcabca");
        PrefixFunctionSparseTable pfst(pf);
        assert(pfst.getPI(4) == 2);
        assert(pfst.getPI(6) == 4);
        assert(pfst.getPI(6, 0) == 4);
        assert(pfst.getPI(6, 1) == 1);
        assert(pfst.getPI(6, 2) == 0);
        assert(pfst.getPI(6, 3) == 0);
        assert(pfst.getMinPrefixSuffix(4) == 2);
        assert(pfst.getMinPrefixSuffix(6) == 1);
        assert(pfst.getMaxPrefixSuffixLessThanEqualK(6, 5) == 4);
        assert(pfst.getMaxPrefixSuffixLessThanEqualK(6, 4) == 4);
        assert(pfst.getMaxPrefixSuffixLessThanEqualK(6, 3) == 1);
        assert(pfst.getMaxPrefixSuffixLessThanEqualK(5, 6) == 3);
        assert(pfst.getMaxPrefixSuffixLessThanEqualK(5, 5) == 3);
        assert(pfst.getMaxPrefixSuffixLessThanEqualK(5, 4) == 3);
        assert(pfst.getMaxPrefixSuffixLessThanEqualK(5, 3) == 3);
        assert(pfst.getMaxPrefixSuffixLessThanEqualK(5, 2) == 0);
        assert(pfst.getMaxPrefixSuffixLessThanEqualK(5, 1) == 0);
        assert(pfst.getMaxPrefixSuffixLessThanEqualK(5, 0) == 0);
    }
    //--- max common proper prefix-suffix
    {
        for (int CHAR_N = 1; CHAR_N <= 10; CHAR_N++) {
            const int N = CHAR_N * 5;
            auto s = makeRepeatString(N, CHAR_N);

            PrefixFunction pf(s);
            for (int x = 0; x < N; x++) {
                for (int y = x; y < N; y++) {
                    int gt = getMaxCommonProperPrefixSuffix(s, x, y);
                    int ans1 = pf.getMaxCommonProperPrefixSuffix(x, y);
                    int ans2 = pf.getMaxCommonProperPrefixSuffixFast(x, y);
                    if (gt != ans1 || gt != ans2) {
                        cout << "Mismatched : (" << x << ", " << y << ") = " << ans1 << ", " << ans2 << ", " << gt << endl;
                        cout << " s = " << s << endl;
                        ans1 = pf.getMaxCommonProperPrefixSuffix(x, y);
                        ans2 = pf.getMaxCommonProperPrefixSuffixFast(x, y);
                    }
                    assert(gt == ans1 && gt == ans2);
                }
            }
        }
    }
    {
        auto rep1 = makeRepeatString(10, 1);
        auto rep2 = makeRepeatString(10, 2);

        auto s = rep1 + rep2 + rep1 + rep2;
        const int N = int(s.length());

        PrefixFunction pf(s);
        for (int x = 0; x < N; x++) {
            for (int y = x; y < N; y++) {
                int gt = getMaxCommonProperPrefixSuffix(s, x, y);
                int ans1 = pf.getMaxCommonProperPrefixSuffix(x, y);
                int ans2 = pf.getMaxCommonProperPrefixSuffixFast(x, y);
                if (gt != ans1 || gt != ans2) {
                    cout << "Mismatched : (" << x << ", " << y << ") = " << ans1 << ", " << ans2 << ", " << gt << endl;
                    cout << " s = " << s << endl;
                    ans1 = pf.getMaxCommonProperPrefixSuffix(x, y);
                    ans2 = pf.getMaxCommonProperPrefixSuffixFast(x, y);
                }
                assert(gt == ans1 && gt == ans2);
            }
        }
    }
    {
        auto rep1 = makeRepeatString(10, 2);
        auto rep2 = makeRepeatString(10, 1);

        auto s = rep1 + rep2 + rep1 + rep2;
        const int N = int(s.length());

        PrefixFunction pf(s);
        for (int x = 0; x < N; x++) {
            for (int y = x; y < N; y++) {
                int gt = getMaxCommonProperPrefixSuffix(s, x, y);
                int ans1 = pf.getMaxCommonProperPrefixSuffix(x, y);
                int ans2 = pf.getMaxCommonProperPrefixSuffixFast(x, y);
                if (gt != ans1 || gt != ans2) {
                    cout << "Mismatched : (" << x << ", " << y << ") = " << ans1 << ", " << ans2 << ", " << gt << endl;
                    cout << " s = " << s << endl;
                    ans1 = pf.getMaxCommonProperPrefixSuffix(x, y);
                    ans2 = pf.getMaxCommonProperPrefixSuffixFast(x, y);
                }
                assert(gt == ans1 && gt == ans2);
            }
        }
    }
    {
        for (int CHAR_N = 2; CHAR_N <= 5; CHAR_N++) {
            auto rep1 = makeRepeatString(CHAR_N * 2, CHAR_N);
            auto rep2 = makeRepeatString(CHAR_N * 2, CHAR_N - 1);

            auto s = rep1 + rep2 + rep1 + rep2;
            const int N = int(s.length());

            PrefixFunction pf(s);
            for (int x = 0; x < N; x++) {
                for (int y = x; y < N; y++) {
                    int gt = getMaxCommonProperPrefixSuffix(s, x, y);
                    int ans1 = pf.getMaxCommonProperPrefixSuffix(x, y);
                    int ans2 = pf.getMaxCommonProperPrefixSuffixFast(x, y);
                    if (gt != ans1 || gt != ans2) {
                        cout << "Mismatched : (" << x << ", " << y << ") = " << ans1 << ", " << ans2 << ", " << gt << endl;
                        cout << " s = " << s << endl;
                        ans1 = pf.getMaxCommonProperPrefixSuffix(x, y);
                        ans2 = pf.getMaxCommonProperPrefixSuffixFast(x, y);
                    }
                    assert(gt == ans1 && gt == ans2);
                }
            }
        }
    }
    {
        for (int CHAR_N = 1; CHAR_N <= 3; CHAR_N++) {
            int N = 1000;
#ifdef _DEBUG
            N = 100;
#endif
            auto s = makeString(N, CHAR_N);

            PrefixFunction pf(s);
            for (int x = 0; x < N; x++) {
                for (int y = x; y < N; y++) {
                    int gt = getMaxCommonProperPrefixSuffix(s, x, y);
                    int ans1 = pf.getMaxCommonProperPrefixSuffix(x, y);
                    int ans2 = pf.getMaxCommonProperPrefixSuffixFast(x, y);
                    if (gt != ans1 || gt != ans2) {
                        cout << "Mismatched : (" << x << ", " << y << ") = " << ans1 << ", " << ans2 << ", " << gt << endl;
                        cout << " s = " << s << endl;
                        ans1 = pf.getMaxCommonProperPrefixSuffix(x, y);
                        ans2 = pf.getMaxCommonProperPrefixSuffixFast(x, y);
                    }
                    assert(gt == ans1 && gt == ans2);
                }
            }
        }
    }

    cout << "OK!" << endl;
}
