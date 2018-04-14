#include <cmath>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

#include "vec2D.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testVector() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Vector Class ------------------------" << endl;

    Vec2D<int> a{ 1, 1 };
    cout << a.isCollinear(Vec2D<int>{-1, -1}) << endl;
    cout << a.angle(Vec2D<int>{-1, 1}) << endl;

    cout << "OK!" << endl;
}
