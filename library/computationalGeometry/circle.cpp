#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "circle.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

// https://codingcompetitions.withgoogle.com/codejam/round/00000000008778ec/0000000000b158f7
static long long test(int R) {
    long long area = Circle::countIntegerPointsInCircle(R);

    long long wrongArea = 1; // (0,0) point
    for (int r = 1; r <= R; r++)
        wrongArea += Circle::countIntegerPointsOnCirclePerimeter(r);

    return area - wrongArea;
}

void testCircle() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Circle-related Functions ------------------------" << endl;

    assert(test(2) == 4);
    assert(test(8) == 24);
    assert(test(50) == 812);

    cout << "OK!" << endl;
}
