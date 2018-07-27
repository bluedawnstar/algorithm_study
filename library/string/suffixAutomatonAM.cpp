#include <memory.h>
#include <cassert>
#include <cstring>
#include <string>
#include <memory>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;

// https://saisumit.wordpress.com/2016/01/26/suffix-automaton/ 

#include "suffixAutomatonAM.h"
#include "suffixAutomatonAMAlgo.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <vector>
#include "../common/iostreamhelper.h"

#define MAXN    100000

void testSuffixAutomationAM() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Suffix Automation - Array Mapped --------------------" << endl;
    string s("aabaa");
    SuffixAutomatonAM sa(10);
    sa.extend(s);

    string s2("abbb");
    SuffixAutomatonAM sa2(10);
    sa2.extend(s2);

    SuffixAutomatonAMAlgo saA(sa, MAXN);
    SuffixAutomatonAMAlgo saA2(sa2, MAXN);

    //--- countSubstrings() ---
    cout << "countSubstrings(" << s << ") : " << saA.countSubstrings() << endl;
    assert(saA.countSubstrings() == 11);

    //--- totalLengthOfAllDistinctSubstrings() ---
    cout << "totalLengthOfAllDistinctSubstrings(" << s << ") : " << saA.totalLengthOfAllDistinctSubstrings() << endl;
    assert(saA.totalLengthOfAllDistinctSubstrings() == 30);

    //--- kthSubstring() ---
    int cnt = int(saA.countSubstrings());
    for (int i = 0; i < cnt; i++)
        cout << "kthSubstring(" << s << ", " << i << ") : " << saA.kthSubstring(i) << endl;

    //--- minShift() ---
    pair<string, int> minShiftTest[] = {
        { "aab", 0 },
        { "aba", 1 },
        { "baa", 1 },
        { "bab", 1 },
        { "abaa", 2 }
    };
    for (int i = 0; i < sizeof(minShiftTest) / sizeof(minShiftTest[0]); i++) {
        cout << "minShift(" << minShiftTest[i].first << ") : " << SuffixAutomatonAMAlgo::minShiftAM(minShiftTest[i].first) << endl;
        assert(SuffixAutomatonAMAlgo::minShiftAM(minShiftTest[i].first) == minShiftTest[i].second);
    }

    //--- findFirst() ---
    pair<string, int> findFirstTest[] = {
        { "a", 0 },
        { "ab", 1 },
        { "ba", 2 },
        { "baa", 2 },
        { "aba", 1 },
        { "aaa", -1 },
        { "bab", -1 },
    };
    for (int i = 0; i < sizeof(findFirstTest) / sizeof(findFirstTest[0]); i++) {
        cout << "findFirst(" << findFirstTest[i].first << ") : " << saA.findFirst(findFirstTest[i].first) << endl;
        assert(saA.findFirst(findFirstTest[i].first) == findFirstTest[i].second);
    }

    //--- findAll() ---
    cout << "findAll(" << "a" << ") : " << saA.findAll("a") << endl;
    cout << "findAll(" << "aa" << ") : " << saA.findAll("aa") << endl;
    cout << "findAll(" << "b" << ") : " << saA.findAll("b") << endl;
    cout << "findAll(" << "aba" << ") : " << saA.findAll("aba") << endl;
    cout << "findAll(" << "b" << ") in \"abbb\" : " << saA2.findAll("b") << endl;

    vector<vector<int>> linkTree = saA.makeLinkTree();
    cout << "findAll(" << "a" << ") : " << saA.findAll(linkTree, "a") << endl;
    cout << "findAll(" << "aa" << ") : " << saA.findAll(linkTree, "aa") << endl;
    cout << "findAll(" << "b" << ") : " << saA.findAll(linkTree, "b") << endl;
    cout << "findAll(" << "aba" << ") : " << saA.findAll(linkTree, "aba") << endl;

    //--- countPattern() ---
    pair<string, int> countPatternTest[] = {
        { "a", 4 },
        { "ab", 1 },
        { "ba", 1 },
        { "baa", 1 },
        { "aba", 1 },
        { "aa", 2 },
        { "bab", 0 },
    };
    vector<int> countTable = saA.makeCountTable();
    for (int i = 0; i < sizeof(countPatternTest) / sizeof(countPatternTest[0]); i++) {
        cout << "countPattern(" << countPatternTest[i].first << ") : " << saA.countPattern(countTable, countPatternTest[i].first) << endl;
        assert(saA.countPattern(countTable, countPatternTest[i].first) == countPatternTest[i].second);
    }

    //--- lcs() ---
    pair<string, string> lcsTest[] = {
        { "a", "a" },
        { "ab", "ab" },
        { "bbabab", "aba" },
        { "bbaabb", "baa" },
        { "babaab", "abaa" },
        { "ccccc", "" },
        { "ccccbccacc", "b" },
    };
    for (int i = 0; i < sizeof(lcsTest) / sizeof(lcsTest[0]); i++) {
        cout << "lcs(" << lcsTest[i].first << ") : " << saA.lcs(lcsTest[i].first) << endl;
        assert(saA.lcs(lcsTest[i].first) == lcsTest[i].second);
    }

    //--- isSuffix() ---
    pair<string, bool> suffixTest[] = {
        { "a", true },
        { "aa", true },
        { "baa", true },
        { "abaa", true },
        { "aabaa", true },
        { "ab", false },
        { "b", false },
        { "ba", false },
        { "aba", false },
        { "ccc", false },
    };
    for (int i = 0; i < sizeof(suffixTest) / sizeof(suffixTest[0]); i++) {
        cout << "isSuffix(" << suffixTest[i].first << ") : " << saA.isSuffix(suffixTest[i].first) << endl;
        assert(saA.isSuffix(suffixTest[i].first) == suffixTest[i].second);
    }

    //--- isSuffixWithSortedTerm() ---
    vector<int> term = sa.getTerminals();
    sort(term.begin(), term.end());
    for (int i = 0; i < sizeof(suffixTest) / sizeof(suffixTest[0]); i++) {
        cout << "isSuffixWithSortedTerm(" << suffixTest[i].first << ") : " << saA.isSuffixWithSortedTerm(term, suffixTest[i].first) << endl;
        assert(saA.isSuffixWithSortedTerm(term, suffixTest[i].first) == suffixTest[i].second);
    }
    cout << "OK!" << endl;
}
