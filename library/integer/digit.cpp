#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "digit.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testDigit() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "------------ Digit --------------" << endl;

    int ans = 0;

    ans = countDigit(9, 1);
    cout << ans << endl;
    assert(ans == 1);

    ans = countDigit(10, 1);
    cout << ans << endl;
    assert(ans == 2);

    ans = countDigit(11, 1);
    cout << ans << endl;
    assert(ans == 4);

    ans = countDigit(100, 1);
    cout << ans << endl;
    assert(ans == 21);

    ans = countDigit(111, 1);
    cout << ans << endl;
    assert(ans == 36);

    cout << "OK!" << endl;
}
