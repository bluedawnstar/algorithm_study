#include <vector>
#include <algorithm>

using namespace std;

#include "xTimesValueChecker.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testXTimesValueChecker() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- X-Times Value Checker with Range Queries ----------------------" << endl;
    {
        int N = 8;
        int M = 4;
        vector<int> A{ 1, 1, 0, 3, 3, 1, 3, 3 };
        vector<int> X{ 1, 2, 3, 4 };

        vector<pair<int, int>> query{
            { 0, 1 },
            { 0, 4 },
            { 0, 2 },
            { 1, 7 },
            { 2, 2 },
            { 4, 7 }
        };
        vector<bool> gt{ true, false, true, true, true, false };

        auto ans = XTimesValueChecker::solve(A, X, query);
        if (ans != gt) {
            cout << "Mismatched : ans = " << ans << ", gt = " << gt << endl;
        }
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
