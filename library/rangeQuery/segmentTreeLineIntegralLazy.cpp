#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreeLineIntegralLazy.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// https://www.hackerearth.com/practice/data-structures/advanced-data-structures/segment-trees/practice-problems/algorithm/tom-and-jerry-love-matrices-16fd6e8e/

void testSegmentTreeLineIntegralLazy() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Line Segment Integral Tree with Lazy Propagation ------------------------" << endl;
    {
        /*
            |  1   2   3   4   5   6   7   8   9  10
        ----+----------------------------------------
          1 |  2   3   4   x   6   7   x   x   x  11
          2 |  3   4   5   6   7   8   9  10  11  12
          3 |  4   5   6   7   8   9  10  11  12  13
          4 |  5   6   7   8   9  10  11  12  13  14
          5 |  x   x   x   x  10  11  12  13  14  15
          6 |  7   8   9  10  11  12  13  14  15  16
          7 |  x   x   x   x   x  13  14  15  16  17
          8 |  9  10  11  12  13  14  15  16  17  18
          9 | 10  11  12  13  14  15  16  17  18  19
         10 |  x   x   x   x   x  16  17  18  19  20
        
          2, 5, 6, 8, 10, 11, [12, 13, 16, [21
        */
        vector<tuple<int, int, int, int>> lines{
            { 2, 3, 1, -1 },    // 0
            { 5, 1, 1, -1 },    // 1
            { 6, 2, 1, -1 },    // 2
            { 8, 2, 1, -1 },    // 3
            { 10, 1, 1, -1 },   // 4
            { 11, 1, 1, -1 },   // 5
            { 12, 1, -1, 21 },  // 6
            { 13, 3, -1, 21 },  // 7
            { 16, 5, -1, 21 }   // 8
        };
        LineSegmentIntegralTreeLazy seg(lines);

        seg.add(3, 6, -1);  // [ 8, 13)
        seg.add(5, 7, -1);  // [11, 16)
        seg.add(3, 4, -1);  // [ 8, 11)
        seg.add(1, 1, -1);  // [ 5,  6)
        seg.add(2, 3, -1);  // [ 6, 10)

        auto ans1 = seg.findKthX(1);
        assert(ans1 == 2);

        auto ans10 = seg.findKthX(10);
        assert(ans10 == 6);

        auto ans14 = seg.findKthX(14);
        assert(ans14 == 7);

        auto ans18 = seg.findKthX(18);
        assert(ans18 == 7);

        auto ans19 = seg.findKthX(19);
        assert(ans19 == 8);
    }

    cout << "OK!" << endl;
}
