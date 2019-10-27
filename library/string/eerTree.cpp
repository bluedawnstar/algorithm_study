#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "eerTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <tuple>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "palindromicFactorization.h"

static string makeRandomString(int n) {
    string s;
    s.reserve(n);
    for (int i = 0; i < n; i++)
        s.push_back(RandInt32::get() % 3 + 'a');
    return s;
}

static bool isPalindrome(const string& s, int L, int R) {
    while (L < R) {
        if (s[L++] != s[R--])
            return false;
    }
    return true;
}

static vector<string> findSuffixPalindromes(const string& s, int R) {
    vector<string> res;
    for (int i = R; i >= 0; i--) {
        if (isPalindrome(s, i, R))
            res.push_back(s.substr(i, R - i + 1));
    }
    return res;
}

void testEerTree() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- eerTree --------------------------------" << endl;
    {            // 01234567890123 
        string s = "ababbbabbababa";    // a | bab | bbabb | ababa
                                        // a | babbbab | b | ababa

        PalindromicFactorization fact(s);
        int ans1 = min(fact.countMinEventFactorization(), fact.countMinOddFactorization());

        eerTree tr(100);
        tr.build(s);
        int ans2 = tr.countMinFactorization();

        cout << "Any min factorization = " << tr.getAnyMinFactorization() << endl;

        if (ans1 != ans2)
            cout << "Mismatched : " << ans1 << ", " << ans2 << endl;

        assert(ans1 == ans2);
    }
    {
        const string s = "ababaacabd";

        eerTree tree(int(s.length()));
        for (int i = 0; i < int(s.length()); i++) {
            tree.extend(s[i]);

            vector<string> pals = tree.getLastSuffixPalindromes();
            reverse(pals.begin(), pals.end());

            vector<string> gt = findSuffixPalindromes(s, i);

            if (pals != gt)
                cout << "Mismatched at " << i << ": " << pals << endl;
            assert(pals == gt);
            assert(pals.size() == tree.nodes[tree.lastSuffix].count);
        }
    }
    {
        int N = 1000;
#ifndef _DEBUG
        N = 10000;
#endif
        string s = makeRandomString(N);

        PalindromicFactorization fact(s);
        //cout << "S = " << s << endl;
        cout << "Min event factorization = " << fact.countMinEventFactorization() << endl;
        cout << "Min odd factorization = " << fact.countMinOddFactorization() << endl;

        eerTree tr(N);
        tr.build(s);

        for (int i = 1; i <= int(s.length()); i++) {
            int ans1 = min(fact.countMinEventFactorization(i), fact.countMinOddFactorization(i));
            
            int ans2 = tr.countMinFactorization(i);
            if (ans1 != ans2)
                cout << "Mismatched at " << i << " : " << ans1 << ", " << ans2 << endl;
            assert(ans1 == ans2);
        }
    }
    cout << "*** Speed Test ***" << endl;
    {
        int N = 100000;
#ifndef _DEBUG
        N = 10000000;
#endif
        string s = makeRandomString(N);

        PROFILE_START(0);
        PalindromicFactorization fact(s);
        PROFILE_STOP(0);

        cout << "Min event factorization = " << fact.countMinEventFactorization() << endl;
        cout << "Min odd factorization = " << fact.countMinOddFactorization() << endl;

        PROFILE_START(1);
        eerTree tr(N);
        tr.build(s);
        PROFILE_STOP(1);

        for (int i = 1; i <= int(s.length()); i++) {
            int ans1 = min(fact.countMinEventFactorization(i), fact.countMinOddFactorization(i));

            int ans2 = tr.countMinFactorization(i);
            if (ans1 != ans2)
                cout << "Mismatched at " << i << " : " << ans1 << ", " << ans2 << endl;
            assert(ans1 == ans2);
        }
    }

    cout << "OK!" << endl;
}
