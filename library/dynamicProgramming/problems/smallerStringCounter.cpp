#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "smallerStringCounter.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../../common/iostreamhelper.h"
#include "../../common/profile.h"

void testSmallerStringCounter() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Smaller String Counter ------------------------" << endl;
    {
        vector<pair<string, int>> qry{
            { "abcd", 1 },
            { "abcd", 2 },
        };
        vector<int> gt{
            6, 100
        };

        for (int i = 0; i < int(qry.size()); i++) {
            int ans = SmallerStringCounter<>::countSmallerStringsWithExactKDifferent(qry[i].first, qry[i].second);
            if (ans != gt[i])
                cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt[i] << endl;
            assert(ans == gt[i]);
        }
    }

    cout << "OK!" << endl;
}
