#include <vector>
#include <algorithm>

using namespace std;

#include "inversionCounter.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <tuple>
#include <string>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"

void testInversionCounter() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Inversion Counter -----------------------" << endl;
    {
        vector<tuple<int, int, int>> QnA{
            { 10, 0, 1 },
            { 10, 1, 1 },
            { 10, 2, 0 },
            { 10, 3, 1 },
            { 10, 4, 0 },
            { 10, 5, 0 },
            { 10, 6, 0 },
            { 10, 7, 1 },
            { 10, 8, 1 },
            { 10, 9, 0 },
            { 10, 10, 0 }
        };
        int N, K, gt;
        for (auto& it : QnA) {
            tie(N, K, gt) = it;
            auto ans = InversionCounter::countPermutationWithExactInversionsMod2(N, K);
            if (ans != gt) {
                cout << "Mismatched (" << N << "," << K << ") : " << ans << ", " << gt << endl;
            }
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
