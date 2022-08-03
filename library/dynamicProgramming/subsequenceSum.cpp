#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "subsequenceSum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testSubsequenceSum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Subsequence Sum ------------------------" << endl;

    SubsequenceSum<> solver;
    solver.prepare(100'000);
    {
        int ans = solver.solve("203");
        int gt = 254;
        if (ans != gt)
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }
    {
        int ans = solver.solve("901");
        int gt = 1093;
        if (ans != gt)
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
