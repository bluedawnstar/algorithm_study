#include <vector>
#include <algorithm>
#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

using namespace std;

#include "assignGraphColorsByDividing.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static inline int clz(unsigned x) {
#ifndef __GNUC__
    return int(_lzcnt_u32(x));
#else
    return __builtin_clz(x);
#endif
}

void testAssignGraphColorsByDividing() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Assigning Graph Colors By Dividing -------------" << endl;
    {
        AssignGraphColorsByDividing solver(3);
        solver.add(0, 1);
        solver.add(0, 2);
        solver.add(1, 2);

        auto ok = solver.assignColor();
        if (!ok)
            cout << "ERROR at " << __LINE__ << endl;
        assert(ok);

        int maxColor = *max_element(solver.nodeColor.begin(), solver.nodeColor.end());
        int divideCount = 32 - clz(maxColor);
        if (divideCount != 2)
            cout << "ERROR at " << __LINE__ << endl;
        assert(divideCount == 2);
    }
    {
        AssignGraphColorsByDividing solver(4);
        solver.add(0, 1);
        solver.add(2, 3);

        auto ok = solver.assignColor();
        if (ok)
            cout << "ERROR at " << __LINE__ << endl;
        assert(!ok);
    }
    cout << "OK!" << endl;
}
