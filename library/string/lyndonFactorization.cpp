#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "lyndonFactorization.h"

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
        s.push_back(RandInt32::get() % 26 + 'a');
    return s;
}

void testLyndonFactorization() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Lyndon Factorization --------------------------------" << endl;
    {
        string s = "abcab";

        auto ans = LyndonFactorization::factorize(s);
        cout << ans << endl;

        vector<string> gt{ "abc", "ab" };
        assert(ans == gt);

        auto ans2 = LyndonFactorization::minCyclicShift(s);
        cout << ans2 << endl;
        assert(ans2 == "ababc");
    }
    {
        string s = "efgdefcdebcdabc";

        auto ans = LyndonFactorization::factorize(s);
        cout << ans << endl;

        vector<string> gt{ "efg", "def", "cde", "bcd", "abc" };
        assert(ans == gt);

        auto ans2 = LyndonFactorization::minCyclicShift(s);
        cout << ans2 << endl;
        assert(ans2 == "abcefgdefcdebcd");
    }

    cout << "OK!" << endl;
}
