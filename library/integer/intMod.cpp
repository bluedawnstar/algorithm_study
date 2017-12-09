#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

#include "intMod.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testIntMod() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Modular Operations -------------------------" << endl;

    assert(modAdd(3, 7, 5) == 0);
    assert(modMul(3, 7, 5) == 1);

    cout << "OK!" << endl;
}
