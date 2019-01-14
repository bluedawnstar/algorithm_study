#include <cmath>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

#include "vec2D.h"
#include "dynamicConvexHull.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

template <typename T>
static ostream& operator <<(ostream& os, const Vec2D<T>& x) {
    os << "(" << x.first << "," << x.second << ")";
    return os;
}

void testDynamicConvexHull() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Dynamic Convex Hull ------------------------" << endl;
    vector<Vec2D<int>> hull{ { 0, 0  }, { 3, -1 }, { 4, 5 }, { -1, 4 } };

    DynamicConvexHull<int>::addPoint(hull, Vec2D<int>{ 100, 100 });
    cout << hull << endl;

    vector<Vec2D<int>> gt{ { -1, 4 }, { 0, 0 }, { 3, -1 }, { 100, 100 } };
    assert(hull == gt);

    cout << "OK!" << endl;
}
