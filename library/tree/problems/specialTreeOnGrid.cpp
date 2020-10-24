#include <vector>
#include <algorithm>

using namespace std;

#include "specialTreeOnGrid.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <numeric>
#include <tuple>
#include <set>
#include <vector>
#include <iostream>
#include "../../common/iostreamhelper.h"
#include "../../common/profile.h"
#include "../../common/rand.h"

void testSpecialTreeOnGrid() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Special Tree On Grid -------------------------------" << endl;
    {
        tuple<pair<long long, long long>, pair<long long, long long>, long long> queries[]{
            { { 7, 5 }, { 7, 5 }, 0 },
            { { 1, 1 }, { -1, 1 }, 1 },
            { { 3, 7 }, { 5, 9 }, 6 },
            { { 113, 167 }, { 361765, 170831 }, 78 },
            { { 24844359, 74532371 }, { 14673633991, 3057007029 }, 278031 }
        };

        for (auto& it : queries) {
            long long x1 = get<0>(it).first;
            long long y1 = get<0>(it).second;
            long long x2 = get<1>(it).first;
            long long y2 = get<1>(it).second;
            long long gt = get<2>(it);
            auto ans1 = SpecialTreeOnGrid1::calculateDistance(x1, y1, x2, y2);
            auto ans2 = SpecialTreeOnGrid2::calculateDistance(x1, y1, x2, y2);
            if (ans1 != gt || ans2 != gt) {
                cout << "Mismatched : " << ans1 << ", " << ans2 << ", " << gt << endl;
            }
            assert(ans1 == gt);
            assert(ans2 == gt);
        }
    }
    cout << "OK!" << endl;
}
