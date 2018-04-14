#include <cmath>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

#include "lineSegmentIntersection.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testLineSegmentIntersection() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Line Segment Intersection ------------------" << endl;

    vector<LineSegmentIntersection<int>::LineSegment> segments;
    segments.emplace_back(Vec2D<int>{ 1, 1 }, Vec2D<int>{ 10, 10 });
    segments.emplace_back(Vec2D<int>{ 2, 1 }, Vec2D<int>{ 4, 1 });
    segments.emplace_back(Vec2D<int>{ 4, 2 }, Vec2D<int>{ 1, 10 });

    auto ans = LineSegmentIntersection<int>::findIntersection(segments);
    cout << ans << endl;
    assert(ans == make_pair(0, 2));

    cout << "OK!" << endl;
}
