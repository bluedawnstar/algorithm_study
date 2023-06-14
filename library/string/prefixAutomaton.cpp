#include <tuple>
#include <functional>
#include <string>
#include <array>
#include <vector>
#include <algorithm>

using namespace std;

#include "prefixAutomaton.h"
#include "prefixAutomatonAlgo.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "prefixFunction.h"

static string makeString(int length, int charCount) {
    string res;
    for (int i = 0; i < length; i++)
        res.push_back('a' + RandInt32::get() % charCount);
    return res;
}

static vector<int> getPIofT(const string& s, const string& t) {
    string st = s + '#' + t;
    PrefixFunction pf(st);
    return vector<int>(pf.pi.begin() + s.length() + 1, pf.pi.end());
}

static vector<int> findAllSlow(const string& s, const string& t) {
    auto pi = getPIofT(s, t);
    vector<int> res;
    for (int i = 0; i < int(pi.size()); i++) {
        if (pi[i] == int(s.length()))
            res.push_back(i - int(s.length()) + 1);
    }
    return res;
}


void testPrefixAutomaton() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Prefix Automaton ------------" << endl;
    {
        string S = "abababcab";
        string T = "ababababab";
        
        PrefixAutomaton<> pa(S);
        auto ans = pa.getPI(T);
        auto gt = getPIofT(S, T);
        if (ans != gt) {
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        }
        assert(ans == gt);
    }
    {
        const int T1 = 100;
        const int T2 = 100;
        const int MAXC = 2;
        const int SN = 100;
        const int TN = 100;
        for (int i = 0; i < T1; i++) {
            string S = makeString(SN, MAXC);
            PrefixAutomaton<> pa(S);
            for (int j = 0; j < T2; j++) {
                string T = makeString(TN, MAXC);
                auto ans = pa.getPI(T);
                auto gt = getPIofT(S, T);
                if (ans != gt) {
                    cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
                }
                assert(ans == gt);
            }
        }
    }
    cout << "OK!" << endl;
    {
        string S = "ab";
        string T = "ababcacabcab";
        //          012345678901
        vector<int> gt{ 0, 2, 7, 10 };

        PrefixAutomaton<> pa(S);
        auto ans = pa.findAll(T);
        if (ans != gt) {
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        }
        assert(ans == gt);
    }
    {
        string S = "aa";
        string T = "aaaaaaaaaaaa";
        //          012345678901
        vector<int> gt{ 0, 1, 2, 3, 4, 5 , 6, 7, 8, 9, 10 };

        PrefixAutomaton<> pa(S);
        auto ans = pa.findAll(T);
        if (ans != gt) {
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        }
        assert(ans == gt);
    }
    {
        const int T1 = 100;
        const int T2 = 100;
        const int MAXC = 2;
        const int SN = 2;
        const int TN = 100;
        for (int i = 0; i < T1; i++) {
            string S = makeString(SN, MAXC);
            PrefixAutomaton<> pa(S);
            for (int j = 0; j < T2; j++) {
                string T = makeString(TN, MAXC);
                auto ans = pa.findAll(T);
                auto gt = findAllSlow(S, T);
                if (ans != gt) {
                    cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
                    cout << "              S = " << S << ", T = " << T << endl;
                }
                assert(ans == gt);
            }
        }
    }
    cout << "OK!" << endl;
    {
        string S  = "ab";
        string T  = "ababca?abcab";
        string T2 = "ababcababcab";
        const int gt = 5;
        const pair<string, int> gt2{ T2, gt };

        int  ans = PrefixAutomatonAlgo::maximizeOccurrences(S, T);
        auto ans1 = PrefixAutomatonAlgo::maximizeOccurrences2(S, T);
        int  ans2 = PrefixAutomatonAlgo::maximizeOccurrencesDP(S, T);
        auto ans3 = PrefixAutomatonAlgo::maximizeOccurrencesDP2(S, T);
        if (ans != gt) {
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        }
        if (ans1 != gt2) {
            cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << gt2 << endl;
        }
        if (ans2 != gt) {
            cout << "Mismatched at " << __LINE__ << " : " << ans2 << ", " << gt << endl;
        }
        if (ans3 != gt2) {
            cout << "Mismatched at " << __LINE__ << " : " << ans3 << ", " << gt2 << endl;
        }
        assert(ans == gt);
        assert(ans1 == gt2);
        assert(ans2 == gt);
        assert(ans3 == gt2);
    }
    {
        string S  = "abc";
        string T  = "ababca????cab";
        string T2 = "ababcabcabcab";
        const int gt = 3;
        const pair<string, int> gt2{ T2, gt };

        int  ans = PrefixAutomatonAlgo::maximizeOccurrences(S, T);
        auto ans1 = PrefixAutomatonAlgo::maximizeOccurrences2(S, T);
        int  ans2 = PrefixAutomatonAlgo::maximizeOccurrencesDP(S, T);
        auto ans3 = PrefixAutomatonAlgo::maximizeOccurrencesDP2(S, T);
        if (ans != gt) {
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        }
        if (ans1 != gt2) {
            cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << gt2 << endl;
        }
        if (ans2 != gt) {
            cout << "Mismatched at " << __LINE__ << " : " << ans2 << ", " << gt << endl;
        }
        if (ans3 != gt2) {
            cout << "Mismatched at " << __LINE__ << " : " << ans3 << ", " << gt2 << endl;
        }
        assert(ans == gt);
        assert(ans1 == gt2);
        assert(ans2 == gt);
        assert(ans3 == gt2);
    }
    {
        string S  = "aa";
        string T  = "aa?aaaa?aaa?";
        string T2 = "aaaaaaaaaaaa";
        const int gt = 11;
        const pair<string, int> gt2{ T2, gt };

        int  ans = PrefixAutomatonAlgo::maximizeOccurrences(S, T);
        auto ans1 = PrefixAutomatonAlgo::maximizeOccurrences2(S, T);
        int  ans2 = PrefixAutomatonAlgo::maximizeOccurrencesDP(S, T);
        auto ans3 = PrefixAutomatonAlgo::maximizeOccurrencesDP2(S, T);
        if (ans != gt) {
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        }
        if (ans1 != gt2) {
            cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << gt2 << endl;
        }
        if (ans2 != gt) {
            cout << "Mismatched at " << __LINE__ << " : " << ans2 << ", " << gt << endl;
        }
        if (ans3 != gt2) {
            cout << "Mismatched at " << __LINE__ << " : " << ans3 << ", " << gt2 << endl;
        }
        assert(ans == gt);
        assert(ans1 == gt2);
        assert(ans2 == gt);
        assert(ans3 == gt2);
    }

    cout << "OK!" << endl;
}
