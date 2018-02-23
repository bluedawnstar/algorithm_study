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


void testPrefixFunction() {
    //return; //TODO: if you want to test string functions, make this line a comment.

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
    cout << "OK!" << endl;
}
