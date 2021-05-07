#include <memory>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "maxWeightOfAllConcatenatedStrings.h"
#include "maxWeightOfAllConcatenatedStrings2.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testMaxWeightOfAllConcatenatedStrings() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Max Weight Of All Concatenated Strings ----------------" << endl;
    {
        string A = "hitech";
        string B = "codechef";
        vector<string> patterns{
            "chef",
            "code",
            "eche"
        };
        vector<int> weight{
            3,
            1,
            5
        };

        auto ans = MaxWeightOfAllConcatenatedStrings::solve(A, B, patterns, weight, 26);
        auto ans2 = MaxWeightOfAllConcatenatedStrings2::solve(A, B, patterns, weight, 26);
        auto gt = 13ll;
        if (ans != gt || ans2 != gt) {
            cout << "Mismatched : " << ans << ", " << gt << endl;
        }
        assert(ans == gt && ans2 == gt);
    }

    cout << "OK!" << endl;
}
