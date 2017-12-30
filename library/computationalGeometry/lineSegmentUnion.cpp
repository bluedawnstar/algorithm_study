#include <cmath>
#include <vector>
#include <cmath>
#include "vec2D.h"
#include <algorithm>

using namespace std;

#include "lineSegmentUnion.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testLineSegmentUnion() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Line Union Functions ------------------------" << endl;

    vector<pair<int, int>> lines{ { 1, 5 }, { 2, 3 }, { 6, 8 }, { 7, 9 } };
    assert(lengthOfLineSegmentUnion(lines) == 7);

    cout << "OK!" << endl;
}
