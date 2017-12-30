#include <cmath>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "ternarySearch.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testTernarySearch() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Ternary Search -----------------------" << endl;
    cout << findMaxWithTernary(-100, 100, [](double x) { return -x*x + 4 * x - 7; }) << endl;

    cout << "OK!" << endl;
}
