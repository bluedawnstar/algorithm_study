#include <vector>
#include <algorithm>

using namespace std;

#include "squareSubsets.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testSquareSubsets() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Square Subsets -----------------------------" << endl;
    {
        int ans = SquareSubsets<1000000007>::solveMax70(vector<int>{ 1, 1, 1, 1 });
        int gt = 15;
        if (ans != gt) {
            cout << "Mismatch : " << ans << ", " << gt << endl;
        }
        assert(ans == gt);
    }
    {
        int ans = SquareSubsets<1000000007>::solveMax70(vector<int>{ 1, 2, 4, 5, 8 });
        int gt = 7;
        if (ans != gt) {
            cout << "Mismatch : " << ans << ", " << gt << endl;
        }
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
