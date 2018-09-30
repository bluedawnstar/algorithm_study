#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "lexicographicallyMinimalStringRotation.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <algorithm>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

#include "lyndonFactorization.h"

static string makeRandomString(int n) {
    string s;
    s.reserve(n);
    for (int i = 0; i < n; i++)
        s.push_back(RandInt32::get() % 26 + 'a');
    return s;
}

void testLexicographicallyMinimalStringRotation() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "-- Lexicographically Minimal String Rotation --------------------------------" << endl;
    {
        string s = "ababc";
        auto ans1 = LexicographicallyMinimalStringRotation::booth(s);
        auto ans2 = LexicographicallyMinimalStringRotation::booth2(s);
        auto ans3 = LexicographicallyMinimalStringRotation::duval(s);
        assert(ans1 == 0);
        assert(ans2 == 0);
        assert(ans3 == 0);
    }
    {
        string s = "abcab";
        auto ans1 = LexicographicallyMinimalStringRotation::booth(s);
        auto ans2 = LexicographicallyMinimalStringRotation::booth2(s);
        auto ans3 = LexicographicallyMinimalStringRotation::duval(s);
        assert(ans1 == 3);
        assert(ans2 == 3);
        assert(ans3 == 3);
    }
    {
        string s = "efgdefcdebcdabc";
        auto ans1 = LexicographicallyMinimalStringRotation::booth(s);
        auto ans2 = LexicographicallyMinimalStringRotation::booth2(s);
        auto ans3 = LexicographicallyMinimalStringRotation::duval(s);
        assert(ans1 == 12);
        assert(ans2 == 12);
        assert(ans3 == 12);
    }

    cout << "OK!" << endl;
}
