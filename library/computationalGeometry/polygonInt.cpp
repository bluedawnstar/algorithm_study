#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "polygonInt.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testPolygonInt() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Integer Polygon ------------------------" << endl;
    {
        vector<pair<int, int>> points{
            { 0, -3 }, { 3, 0 }, { 0, 3 }, { -3, 0 }, { 0, 0 }
        };

        vector<pair<int, int>> polygon = IntPolygon::convexhull(points);
        
        bool ans1 = IntPolygon::isInPolygon(polygon, 1, 1);
        bool gt1 = true;
        if (ans1 != gt1)
            cout << "Mismatched : " << ans1 << ", " << gt1 << endl;
        assert(ans1 == true);

        bool ans2 = IntPolygon::isInPolygonIncludingBoundary(polygon, 1, 1);
        bool gt2 = true;
        if (ans2 != gt2)
            cout << "Mismatched : " << ans1 << ", " << gt2 << endl;
        assert(ans2 == gt2);

        bool ans3 = IntPolygon::isInPolygon(polygon, 2, 1);
        bool gt3 = false;
        if (ans3 != gt3)
            cout << "Mismatched : " << ans3 << ", " << gt3 << endl;
        assert(ans3 == gt3);

        bool ans4 = IntPolygon::isInPolygonIncludingBoundary(polygon, 2, 1);
        bool gt4 = true;
        if (ans4 != gt4)
            cout << "Mismatched : " << ans4 << ", " << gt4 << endl;
        assert(ans4 == gt4);
    }
    cout << "OK!" << endl;
}
