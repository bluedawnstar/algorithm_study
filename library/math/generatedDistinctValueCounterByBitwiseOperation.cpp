#include <cmath>
#include <functional>
#include <bitset>
#include <vector>
#include <algorithm>

using namespace std;

#include "generatedDistinctValueCounterByBitwiseOperation.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testGeneratedDistinctValueCounterByBitwiseOperation() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Generated Distinct Value Counter By Bitwise Operation ------" << endl;
    {
        assert(GeneratedDistinctValueCounterByBitwiseOperation::countBitwiseOr(7, 9) == 4);
    }

    cout << "OK!" << endl;
}
