#include <vector>
#include <numeric>
#include <functional>
#include <algorithm>

using namespace std;

#include "smallestBinaryArrayWithInversions.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <unordered_set>
#include <iostream>
#include "../../common/iostreamhelper.h"
#include "../../common/profile.h"
#include "../../common/rand.h"

// https://www.hackerearth.com/problem/algorithm/binary-inversions-cf645357/

void testSmallestBinaryArrayWithInversions() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Smallest Binary Array With Inversions -------------" << endl;
    {
        vector<int> gt{ 0, 1, 0, 1, 1 };
        auto ans = SmallestBinaryArrayWithInversions::solve(2, 3, 1);
        assert(ans == gt);
    }
    cout << "OK!" << endl;
}
