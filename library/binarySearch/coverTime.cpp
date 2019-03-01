#include <vector>
#include <numeric>
#include <functional>
#include <algorithm>

using namespace std;

#include "coverTime.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// https://www.hackerrank.com/contests/hourrank-31/challenges/save-the-queen/problem

void testCoverTime() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Cover Time -------------" << endl;
    {
        int N = 3;
        vector<int> A{ 1000, 100, 100, 100 };
        assert(abs(CoverTime::solve(N, A) - 150.0) < 1e-4);
        assert(abs(CoverTime::solve2(N, A) - 150.0) < 1e-9);
    }
    {
        int N = 3;
        vector<int> A{ 10, 15, 10, 100 };
        assert(abs(CoverTime::solve(N, A) - 17.5) < 1e-4);
        assert(abs(CoverTime::solve2(N, A) - 17.5) < 1e-9);
    }

    cout << "OK!" << endl;
}
