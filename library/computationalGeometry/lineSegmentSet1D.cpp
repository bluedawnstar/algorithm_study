#include <tuple>
#include <set>
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

        LineSegmentSet1D lineSet;
        for (auto it : lines)
            lineSet.add(it.first, it.second);

        assert(lineSet.getCovered() == 7);
    }
    cout << "OK!" << endl;
}
