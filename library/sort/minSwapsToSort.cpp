#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "minSwapsToSort.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testMinSwapsToSort() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Min Swaps to Sort an Array ------------------------------" << endl;
    {
        vector<int> in{ 4, 3, 2, 1 };
        int gt = 2;
        int ans = MinSwapsToSort::count(in);
        if (ans != gt) {
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        }
        assert(ans == gt);
    }
    {
        vector<int> in{ 101, 758, 315, 730, 472, 619, 460, 479 };
        int gt = 5;
        int ans = MinSwapsToSort::count(in);
        if (ans != gt) {
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
        }
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
