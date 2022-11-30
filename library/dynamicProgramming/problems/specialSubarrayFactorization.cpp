#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "specialSubarrayFactorization.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../../common/iostreamhelper.h"
#include "../../common/profile.h"

void testSpecialSubarrayFactorization() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Special Subarray Factorization ------------------------" << endl;
    {
        vector<int> in{ 10, 1, 5, 6, 1, 7, 7, 1, 5, 5, 1, 10 };

        int gt[3]{ 5, 11, 19 };

        auto ans0 = SpecialSubarrayFactorization::countPrefixArrays(in);
        auto ans1 = SpecialSubarrayFactorization::countWaysToSplitPrefixArrays(in);
        auto ans2 = SpecialSubarrayFactorization::countWaysToSplitSubarrays(in);

        if (ans0 != gt[0])
            cout << "Mismatched at " << __LINE__ << " : " << ans0 << ", " << gt[0] << endl;
        assert(ans0 == gt[0]);

        if (ans1 != gt[1])
            cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << gt[1] << endl;
        assert(ans1 == gt[1]);

        if (ans2 != gt[2])
            cout << "Mismatched at " << __LINE__ << " : " << ans2 << ", " << gt[2] << endl;
        assert(ans2 == gt[2]);
    }

    cout << "OK!" << endl;
}
