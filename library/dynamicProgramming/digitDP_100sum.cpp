#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "digitDP_100sum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"


void testDigitDP100Sum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Digit DP - 100 Sum --------------------------------" << endl;
    {
        DigitDP_100Sum dp;
        assert(0ll == dp.count(100000000000ll));
        assert(0ll == dp.count(199999999998ll));
        assert(1ll == dp.count(199999999999ll));
        assert(75582ll == dp.count(1000000000000ll));
        assert(51240891ll == dp.count(10000000000000ll));
        assert(7174799646ll == dp.count(100000000000000ll));
        assert(457119685656ll == dp.count(1000000000000000ll));
    }
    cout << "OK!" << endl;
}
