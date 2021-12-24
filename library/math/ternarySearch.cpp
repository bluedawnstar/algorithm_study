#include <cmath>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "ternarySearch.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testTernarySearch() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Ternary Search -----------------------" << endl;
    {
        const double EPSILON = 1e-4;

        auto ans = ternarySearchMax(-100, 100, [](double x) { return -x * x + 4 * x - 7; });

        if (abs(ans.first - 2) > EPSILON || abs(ans.second - -3) > EPSILON)
            cout << "Mismatched : ans = " << ans << ", gt = (2, -3)" << endl;
        assert(abs(ans.first - 2) <= EPSILON && abs(ans.second - -3) <= EPSILON);
    }

    cout << "OK!" << endl;
}
