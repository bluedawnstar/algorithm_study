#include <cmath>
#include <functional>
#include <bitset>
#include <vector>
#include <algorithm>

using namespace std;

#include "generatedDistinctNumbersCounterByBitwiseOperation.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testGeneratedDistinctNumbersCounterByBitwiseOperation() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Generated Distinct Numbers Counter By Bitwise Operation ------" << endl;
    {
        assert(GeneratedDistinctNumbersCounterByBitwiseOperation::countWithSubsetBitwiseOr(7, 9) == 4);
    }

    cout << "OK!" << endl;
}
