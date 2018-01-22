#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

#include "largestRectangle.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testLargestRectangle() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Largest Rectangle ------------------------" << endl;

    assert(largestRectangle(vector<int>{2, 1, 5, 6, 2, 3}) == 10);

    vector<vector<bool>> matrix{
        {true, false, true , false, false},
        {true, false, true , true , true },
        {true, true , true , true , true },
        {true, false, false, true , false}
    };
    assert(largestRectangle(matrix) == 6);

    cout << "OK!" << endl;
}
