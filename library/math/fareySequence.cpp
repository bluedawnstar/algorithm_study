#include <cmath>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "fareySequence.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testFareySequence() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Farey Sequence -----------------" << endl;
    {
        vector<pair<int, int>> F8 = {
            { 0, 1 }, { 1, 8 }, { 1, 7 }, { 1, 6 }, { 1, 5 }, { 1, 4 }, { 2, 7 }, { 1, 3 }, { 3, 8 }, { 2, 5 }, { 3, 7 }, { 1, 2 }, { 4, 7 }, { 3, 5 }, { 5, 8 }, { 2, 3 }, { 5, 7 }, { 3, 4 }, { 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 8 }, { 1, 1 }
        };

        for (int n = 4; n <= 100; n++) {
            FareySequenceRank rank(n);
            for (int i = 0; i <= n; i++) {
                auto ans1 = FareySequence::rank(i, n, n);
                auto ans2 = rank.rank(i, n);
                if (ans1 != ans2)
                    cout << i << ":" << ans1 << ", " << ans2 << endl;
                assert(ans1 == ans2);
            }
        }

        FareySequenceRank rank(8);

        assert(FareySequence::getSequenceLength(8) == F8.size());
        for (int i = 1; i < int(F8.size()); i++) {
            assert(FareySequence::kthNaive(8, i) == F8[i - 1]);
            assert(FareySequence::kthFast(8, i) == F8[i - 1]);
            assert(FareySequence::kthFast(rank, 8, i) == F8[i - 1]);
        }
    }

    cout << "OK!" << endl;
}
