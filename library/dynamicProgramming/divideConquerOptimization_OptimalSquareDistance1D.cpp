#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "divideConquerOptimization_OptimalSquareDistance1D.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testOptimalSquareDistance1D() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- OptimalSquareDistance1D ------------------------------" << endl;
    {
        OptimalSquareDistance1D osd(vector<int>{ 1, 2, 4 });
        assert(osd.solve(2) == 0.5);
    }

    cout << "OK!" << endl;
}
