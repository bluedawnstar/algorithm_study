#include <cmath>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

#include "vec2D.h"
#include "polygonDiameter.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testPolygonDiameter() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Polygon Diameter ---------------------------------" << endl;

    vector<Vec2D<int>> polygon{ { 0, 0 }, { 4, 0 }, { 4, 4 }, { 2, 5 }, { 0, 4 } };
    cout << calcPolygonDiameter(polygon) << endl;
    
    cout << "OK!" << endl;
}
