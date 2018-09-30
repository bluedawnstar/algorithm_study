#include <string>
#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

#include "palindromicFactorization.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <algorithm>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

static string makeRandomString(int n) {
    string s;
    s.reserve(n);
    for (int i = 0; i < n; i++)
        s.push_back(RandInt32::get() % 3 + 'a');
    return s;
}

void testPalindromicFactorization() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Palindromic Factorization --------------------------------" << endl;
    {            // 01234567890123 
        string s = "ababbbabbababa";    // a | bab | bbabb | ababa
                                        // a | babbbab | b | ababa

        PalindromicFactorization fact(s);
        assert(fact.countMinEventFactorization() == 4);
        assert(fact.countMinOddFactorization() == 5);

        PalindromicFactorizationDP fact2(s);
        cout << "Min factorization = " << fact2.countMinFactorization() << endl;

        auto pals = fact2.getPalindromes();
        cout << "One of Min Palindromes = " << pals << endl;

        PalindromicFactorizationDPEx fact3(s);
        cout << "The number of ways = " << fact3.countWays() << endl;

        auto allPals = fact3.getAllPalindromes();
        for (int i = 0; i < int(allPals.size()); i++)
            cout << "pal[" << i << "] = " << allPals[i] << endl;
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

        PalindromicFactorizationDP fact2(s);
        cout << "Min factorization = " << fact2.countMinFactorization() << endl;

        for (int i = 1; i <= int(s.length()); i++) {
            int ans1 = min(fact.countMinEventFactorization(i), fact.countMinOddFactorization(i));
            int ans2 = fact2.countMinFactorization(i);
            if (ans1 != ans2)
                cout << "Mismatched at " << i << " : " << ans1 << ", " << ans2 << endl;
            assert(ans1 == ans2);
        }
    }

    cout << "OK!" << endl;
}
