#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "digitDP_PalindromicNumber.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"


void testDigitDPPalindromicNumber() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Digit DP - Palindromic Number ----------------------------" << endl;
    {
        DigitDP_PalindromicNumber dp;

        assert(9 == dp.count(10ll));
        assert(10 == dp.count(11ll));

        assert(18 == dp.count(100ll));
        assert(19 == dp.count(101ll));

        assert(108 == dp.count(1000ll));
        assert(109 == dp.count(1001ll));

        assert(198 == dp.count(10000ll));
        assert(199 == dp.count(10001ll));

        assert(1098 == dp.count(100000ll));
        assert(1099 == dp.count(100001ll));

        assert(109998 == dp.count(1000000000ll));
        assert(109999 == dp.count(1000000001ll));

        assert(199998 == dp.count(10000000000ll));
        assert(199999 == dp.count(10000000001ll));
    }
    cout << "OK!" << endl;
}
