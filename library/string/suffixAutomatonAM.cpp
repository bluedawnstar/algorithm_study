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

#define MAXN    100000

// 1. Number of different substrings of a given string .
static vector<long long> gDAM(MAXN * 2, 0);
static long long countSubstrings(SuffixAutomatonAM& sa, int u) {
    if (gDAM[u])
        return gDAM[u];

    long long res = 1;

    for (int i = 0; i < SuffixAutomatonAM::MaxCharN; i++) {
        if (sa.state[u].hasNext(i))
            res += countSubstrings(sa, sa.state[u].getNext(i));
    }

    return gDAM[u] = res;
}

long long countSubstrings(SuffixAutomatonAM& sa) {
    fill(gDAM.begin(), gDAM.end(), 0);
    return countSubstrings(sa, 0) - 1;
}


// 2. Total length of all distinct substrings.
static vector<long long> gTotLenAM(MAXN * 2, 0);
static long long totalLengthOfAllDistinctSubstrings(SuffixAutomatonAM& sa, int u) {
    long long res = 0;

    if (gTotLenAM[u])
        return gTotLenAM[u];

    for (int i = 0; i < SuffixAutomatonAM::MaxCharN; i++)
        if (sa.state[u].hasNext(i))
            //res += totalLengthOfAllDistinctSubstrings(sa, sa.state[u].getNext(i)) + gD[sa.state[u].getNext(i)];
            res += totalLengthOfAllDistinctSubstrings(sa, sa.state[u].getNext(i)) + countSubstrings(sa, sa.state[u].getNext(i));

    return gTotLenAM[u] = res;
}

long long totalLengthOfAllDistinctSubstrings(SuffixAutomatonAM& sa) {
    fill(gDAM.begin(), gDAM.end(), 0);
    countSubstrings(sa);

    fill(gTotLenAM.begin(), gTotLenAM.end(), 0);
    return totalLengthOfAllDistinctSubstrings(sa, 0);
}


// 3. Lexographically kth substring.
// kth >= 1
static bool kthSubstring(SuffixAutomatonAM& sa, string& res, int kth, int u, int& path) {
    for (int i = 0; i < SuffixAutomatonAM::MaxCharN; i++) {
        if (sa.state[u].hasNext(i)) {
            if (++path == kth || kthSubstring(sa, res, kth, sa.state[u].getNext(i), path)) {
                res.push_back('a' + i);
                return true;
            }
        }
    }
    return false;
}

// kth >= 0
string kthSubstring(SuffixAutomatonAM& sa, int kth) {
    string res;
    int path = 0;

    kthSubstring(sa, res, kth + 1, 0, path);

    reverse(res.begin(), res.end());
    return res;
}


// 4. Smallest Cyclic Shift to obtain lexicographical smallest of All possible  
int minShiftAM(const string& s) {
    string ss = s + s;
    SuffixAutomatonAM sa((int)ss.length());
    sa.init();
    sa.extend(ss);

    int res = -1;
    int len = 0, cur = 0;

    do {
        for (int j = 0; j < SuffixAutomatonAM::MaxCharN; j++) {
            if (sa.state[cur].hasNext(j)) {
                if (++len == (int)s.length())
                    res = sa.state[cur].rightEnd - (int)s.length() + 2;
                else
                    cur = sa.state[cur].getNext(j);
                break;
            }
        }
    } while (res < 0);

    return min(res, (int)s.length() - res);
}


// 5. position of first occurrence
int findFirst(SuffixAutomatonAM& sa, const string& pattern) {
    int u = 0;
    for (int i = 0; i < (int)pattern.length(); i++) {
        u = sa.state[u].getNext(SuffixAutomatonAM::ch2i(pattern[i]));
        if (u <= 0)
            return -1;
    }
    return sa.state[u].rightEnd - (int)pattern.length() + 1;
}


// 6. Position of all occurrences.
static void dfsFind(SuffixAutomatonAM& sa, vector<vector<int>>& children, int len, vector<int>& res, int u) {
    if (u <= 0)
        return;

    res.push_back(sa.state[u].rightEnd - len + 1);
    for (int v : children[u])
        dfsFind(sa, children, len, res, v);
}

vector<int> findAll(SuffixAutomatonAM& sa, const string& pattern) {
    vector<vector<int>> children(sa.N);
    for (int u = 1; u < sa.N; u++) {
        children[sa.state[u].suffixLink].push_back(u);
    }

    int u = 0;
    for (int i = 0; i < (int)pattern.length(); i++) {
        u = sa.state[u].getNext(SuffixAutomatonAM::ch2i(pattern[i]));
        if (u <= 0)
            return{};
    }

    vector<int> res;
    dfsFind(sa, children, (int)pattern.length(), res, u);
    res.erase(unique(res.begin(), res.end()), res.end());

    return move(res);
}


vector<vector<int>> makeLinkTree(SuffixAutomatonAM& sa) {
    vector<vector<int>> children(sa.N);
    for (int u = 1; u < sa.N; u++) {
        children[sa.state[u].suffixLink].push_back(u);
    }
    return move(children);
}

vector<int> findAll(SuffixAutomatonAM& sa, vector<vector<int>>& children, const string& pattern) {
    int u = 0;
    for (int i = 0; i < (int)pattern.length(); i++) {
        u = sa.state[u].getNext(SuffixAutomatonAM::ch2i(pattern[i]));
        if (u <= 0)
            return{};
    }

    vector<int> res;
    dfsFind(sa, children, (int)pattern.length(), res, u);
    res.erase(unique(res.begin(), res.end()), res.end());

    return move(res);
}


// 7. count number of occurrences
vector<int> makeCountTable(SuffixAutomatonAM& sa) {
    vector<pair<int, int>> base;
    vector<int> cnt(sa.N);

    for (int i = 1; i < sa.N; i++) {
        cnt[i] = !sa.state[i].cloned;
        base.emplace_back(sa.state[i].len, i);
    }
    sort(base.begin(), base.end());

    for (int i = (int)base.size() - 1; i >= 0; i--)
        cnt[sa.state[base[i].second].suffixLink] += cnt[base[i].second];

    return move(cnt);
}

int countPattern(SuffixAutomatonAM& sa, const vector<int>& countTable, const string& pattern) {
    int u = 0;
    for (int i = 0; i < (int)pattern.length(); i++) {
        u = sa.state[u].getNext(SuffixAutomatonAM::ch2i(pattern[i]));
        if (u <= 0)
            return 0;
    }
    return countTable[u];
}


// 8. Longest common substring.

// It's better that t's length is shorter than SuffixAutomation's length
int lengthOfLcs(SuffixAutomatonAM& sa, string& t) {
    int v = 0, len = 0, bestPos = 0, bestLen = 0;
    for (int i = 0; i < (int)t.length(); ++i) {
        int ch = SuffixAutomatonAM::ch2i(t[i]);
        while (v && !sa.state[v].hasNext(ch)) {
            v = sa.state[v].suffixLink;
            len = sa.state[v].len;
        }
        if (sa.state[v].hasNext(ch)) {
            v = sa.state[v].getNext(ch);
            ++len;
        }
        if (len > bestLen) {
            bestLen = len;
            bestPos = i;
        }
    }
    return bestLen;
}

string lcs(SuffixAutomatonAM& sa, string& t) {
    int v = 0, len = 0, bestPos = 0, bestLen = 0;
    for (int i = 0; i < (int)t.length(); ++i) {
        int ch = SuffixAutomatonAM::ch2i(t[i]);
        while (v && !sa.state[v].hasNext(ch)) {
            v = sa.state[v].suffixLink;
            len = sa.state[v].len;
        }
        if (sa.state[v].hasNext(ch)) {
            v = sa.state[v].getNext(ch);
            ++len;
        }
        if (len > bestLen) {
            bestLen = len;
            bestPos = i;
        }
    }
    return t.substr(bestPos - bestLen + 1, bestLen);
}

// 9. Longest common substring of multiple substring.
// step1) make SuffixAutomation instances of all strings
// step2) call lengthOfLcs(sa[L], str[R]) - length(str[R]) <= length(str[L])
//        if length(str[L]) < length(str[R]) then swap(L, R)

// 10. Find whether a given string s is a suffix of sa.
bool isSuffix(SuffixAutomatonAM& sa, const string& s) {
    vector<int> term = sa.getTerminals();

    int u = 0;
    for (int i = 0; i < (int)s.length(); i++) {
        u = sa.state[u].getNext(SuffixAutomatonAM::ch2i(s[i]));
        if (u <= 0)
            return false;
    }
    return find(term.begin(), term.end(), u) != term.end();
}

bool isSuffix(SuffixAutomatonAM& sa, vector<int>& term, const string& s) {
    int u = 0;
    for (int i = 0; i < (int)s.length(); i++) {
        u = sa.state[u].getNext(SuffixAutomatonAM::ch2i(s[i]));
        if (u <= 0)
            return false;
    }
    return find(term.begin(), term.end(), u) != term.end();
}

bool isSuffixWithSortedTerm(SuffixAutomatonAM& sa, vector<int>& term, const string& s) {
    int u = 0;
    for (int i = 0; i < (int)s.length(); i++) {
        u = sa.state[u].getNext(SuffixAutomatonAM::ch2i(s[i]));
        if (u <= 0)
            return false;
    }
    return binary_search(term.begin(), term.end(), u);
}


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <vector>
#include "../common/iostreamhelper.h"

void testSuffixAutomationAM() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Suffix Automation - Array Mapped --------------------" << endl;
    string s("aabaa");
    SuffixAutomatonAM sa(10);
    sa.extend(s);

    string s2("abbb");
    SuffixAutomatonAM sa2(10);
    sa2.extend(s2);

    //--- countSubstrings() ---
    cout << "countSubstrings(" << s << ") : " << countSubstrings(sa) << endl;
    assert(countSubstrings(sa) == 11);

    //--- totalLengthOfAllDistinctSubstrings() ---
    cout << "totalLengthOfAllDistinctSubstrings(" << s << ") : " << totalLengthOfAllDistinctSubstrings(sa) << endl;
    assert(totalLengthOfAllDistinctSubstrings(sa) == 30);

    //--- kthSubstring() ---
    int cnt = (int)countSubstrings(sa);
    for (int i = 0; i < cnt; i++)
        cout << "kthSubstring(" << s << ", " << i << ") : " << kthSubstring(sa, i) << endl;

    //--- minShift() ---
    pair<string, int> minShiftTest[] = {
        { "aab", 0 },
        { "aba", 1 },
        { "baa", 1 },
        { "bab", 1 },
        { "abaa", 2 }
    };
    for (int i = 0; i < sizeof(minShiftTest) / sizeof(minShiftTest[0]); i++) {
        cout << "minShift(" << minShiftTest[i].first << ") : " << minShiftAM(minShiftTest[i].first) << endl;
        assert(minShiftAM(minShiftTest[i].first) == minShiftTest[i].second);
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
        cout << "findFirst(" << findFirstTest[i].first << ") : " << findFirst(sa, findFirstTest[i].first) << endl;
        assert(findFirst(sa, findFirstTest[i].first) == findFirstTest[i].second);
    }

    //--- findAll() ---
    cout << "findAll(" << "a" << ") : " << findAll(sa, "a") << endl;
    cout << "findAll(" << "aa" << ") : " << findAll(sa, "aa") << endl;
    cout << "findAll(" << "b" << ") : " << findAll(sa, "b") << endl;
    cout << "findAll(" << "aba" << ") : " << findAll(sa, "aba") << endl;
    cout << "findAll(" << "b" << ") in \"abbb\" : " << findAll(sa2, "b") << endl;

    vector<vector<int>> linkTree = makeLinkTree(sa);
    cout << "findAll(" << "a" << ") : " << findAll(sa, linkTree, "a") << endl;
    cout << "findAll(" << "aa" << ") : " << findAll(sa, linkTree, "aa") << endl;
    cout << "findAll(" << "b" << ") : " << findAll(sa, linkTree, "b") << endl;
    cout << "findAll(" << "aba" << ") : " << findAll(sa, linkTree, "aba") << endl;

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
    vector<int> countTable = makeCountTable(sa);
    for (int i = 0; i < sizeof(countPatternTest) / sizeof(countPatternTest[0]); i++) {
        cout << "countPattern(" << countPatternTest[i].first << ") : " << countPattern(sa, countTable, countPatternTest[i].first) << endl;
        assert(countPattern(sa, countTable, countPatternTest[i].first) == countPatternTest[i].second);
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
        cout << "lcs(" << lcsTest[i].first << ") : " << lcs(sa, lcsTest[i].first) << endl;
        assert(lcs(sa, lcsTest[i].first) == lcsTest[i].second);
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
        cout << "isSuffix(" << suffixTest[i].first << ") : " << isSuffix(sa, suffixTest[i].first) << endl;
        assert(isSuffix(sa, suffixTest[i].first) == suffixTest[i].second);
    }

    //--- isSuffixWithSortedTerm() ---
    vector<int> term = sa.getTerminals();
    sort(term.begin(), term.end());
    for (int i = 0; i < sizeof(suffixTest) / sizeof(suffixTest[0]); i++) {
        cout << "isSuffixWithSortedTerm(" << suffixTest[i].first << ") : " << isSuffixWithSortedTerm(sa, term, suffixTest[i].first) << endl;
        assert(isSuffixWithSortedTerm(sa, term, suffixTest[i].first) == suffixTest[i].second);
    }
    cout << "OK!" << endl;
}
