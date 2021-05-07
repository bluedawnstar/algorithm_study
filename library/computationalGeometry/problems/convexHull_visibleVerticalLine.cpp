#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "convexHull_visibleVerticalLine.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../../common/iostreamhelper.h"
#include "../../common/profile.h"
#include "../../common/rand.h"

void testConvexHullVisibleVerticalLine() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Convex Hull - Visible Vertical Line ---------------------" << endl;
    {
        auto ans = VisibleVerticalLine::findPreviousVisibleVLine(vector<int>{ 1, 2 });
        vector<int> gt{ -1, 0 };
        assert(ans == gt);
    }
    {
        auto ans = VisibleVerticalLine::findPreviousVisibleVLine(vector<int>{ 1, 2, 3 });
        vector<int> gt{ -1, 0, 0 };
        assert(ans == gt);
    }
    {
        auto ans = VisibleVerticalLine::findPreviousVisibleVLine(vector<int>{ 3, 2, 5, 3, 2, 4, 3 });
        vector<int> gt{ -1, 0, 0, 2, 2, 2, 5 };
        assert(ans == gt);
    }
    cout << "OK!" << endl;
}
