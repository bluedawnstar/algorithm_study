#include <tuple>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#include "rangePointSet.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"

void testRangePointSet() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Range Point Set ------------------------" << endl;
    {
        vector<pair<int, int>> lines{ { 1, 5 },{ 2, 3 },{ 6, 8 },{ 7, 9 } };

        RangePointSet<int> pointSet;
        pointSet.add(lines[0].first, lines[0].second);
        assert(pointSet.getCovered() == 5);
        pointSet.add(lines[1].first, lines[1].second);
        assert(pointSet.getCovered() == 5);
        pointSet.add(lines[2].first, lines[2].second);
        assert(pointSet.getCovered() == 8);
        pointSet.add(lines[3].first, lines[3].second);
        assert(pointSet.getCovered() == 9);
    }
    cout << "OK!" << endl;
}
