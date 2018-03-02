#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "digitDP.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testDigitDP() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Digit DP -----------------------------------" << endl;

    DigitDP dp;
    assert(dp.rangeDigitSum(7, 7) == 7);
    assert(dp.rangeDigitSum(17, 18) == 17);
    assert(dp.rangeDigitSum(123, 1024) == 12613);
    assert(dp.rangeDigitSum(20, 21) == 5);
    assert(dp.rangeDigitSum(1000, 1009) == 55);

    cout << "OK!" << endl;
}
