#include <tuple>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#include "lineSegmentSet1D.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"

void testLineSegmentSet1D() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Line Segment Set for 1D ------------------------" << endl;
    {
        vector<pair<int, int>> lines{ { 1, 5 },{ 2, 3 },{ 6, 8 },{ 7, 9 } };

        LineSegmentSet1D<int> lineSet;
        lineSet.add(lines[0].first, lines[0].second);
        assert(lineSet.getCovered() == 4);
        lineSet.add(lines[1].first, lines[1].second);
        assert(lineSet.getCovered() == 4);
        lineSet.add(lines[2].first, lines[2].second);
        assert(lineSet.getCovered() == 6);
        lineSet.add(lines[3].first, lines[3].second);
        assert(lineSet.getCovered() == 7);
    }
    cout << "OK!" << endl;
}
