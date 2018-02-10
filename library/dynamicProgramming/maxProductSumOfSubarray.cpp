#include <cmath>
#include <vector>
#include <deque>
#include <numeric>
#include <algorithm>

using namespace std;

#include "convexHullTrick.h"
#include "maxProductSumOfSubarray.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testProductSumOfSubarray() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Max Product-Sum of All Pairs of Subarray -----------------------------------" << endl;
    {
        int N = 10;
        vector<long long> A(1 << N);
        for (int i = 0; i < (1 << N); i++)
            A[i] = rand();

        AllPairMaxProductSumOfSubarray<long long> solver;
        solver.build(A);

        cout << solver.solve() << endl;
    }
    cout << "OK!" << endl;
}
