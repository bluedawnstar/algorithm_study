#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "factor.h"
#include "gcd.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testGcd() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- gcd() / lcm() / extGcd() -------------------------" << endl;

    assert(gcd(2 * 2 * 2 * 2 * 2, 3 * 3 * 3) == 1);
    assert(gcd(2 * 3 * 2 * 3 * 7, 3 * 3 * 3) == 9);
    int a = 134232, b = 93231;
    assert(gcd(a, b) * lcm(a, b) == a * b);

    int x, y;
    extGcd(a, b, x, y);
    assert(a * x + b * y == gcd(a, b));
    cout << "extGcd(" << a << ", " << b << ") = " << make_pair(x, y) << endl;

    cout << "OK!" << endl;
}
