#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

#include "twoSameSumSubset.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

// https://leetcode.com/problems/tallest-billboard/

void testTwoSameSumSubset() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Two Same Sum Subset -----------------------------------" << endl;
    {
        assert(TwoSameSumSubset::solveDP(vector<int>{ 1, 2, 3, 6 }, 5000) == 6);
        assert(TwoSameSumSubset::solveDP(vector<int>{ 1, 2, 3, 4, 5, 6 }, 5000) == 10);
        assert(TwoSameSumSubset::solveDP(vector<int>{ 34, 28, 39, 23, 32, 26, 23, 24, 26, 24, 26, 29, 27, 34, 30, 38, 34, 37, 36 }, 5000) == 285);
        assert(TwoSameSumSubset::solveDP(vector<int>{ 156, 160, 153, 149, 158, 136, 172, 147, 148, 133, 147, 147, 146, 131, 171, 165, 145, 143, 148, 145 }, 5000) == 1500);
        assert(TwoSameSumSubset::solveBitDP1(vector<int>{ 1, 2, 3, 6 }) == 6);
        assert(TwoSameSumSubset::solveBitDP1(vector<int>{ 1, 2, 3, 4, 5, 6 }) == 10);
        assert(TwoSameSumSubset::solveBitDP1(vector<int>{ 34, 28, 39, 23, 32, 26, 23, 24, 26, 24, 26, 29, 27, 34, 30, 38, 34, 37, 36 }) == 285);
        assert(TwoSameSumSubset::solveBitDP1(vector<int>{ 156, 160, 153, 149, 158, 136, 172, 147, 148, 133, 147, 147, 146, 131, 171, 165, 145, 143, 148, 145 }) == 1500);
        assert(TwoSameSumSubset::solveBitDP2(vector<int>{ 1, 2, 3, 6 }) == 6);
        assert(TwoSameSumSubset::solveBitDP2(vector<int>{ 1, 2, 3, 4, 5, 6 }) == 10);
        assert(TwoSameSumSubset::solveBitDP2(vector<int>{ 34, 28, 39, 23, 32, 26, 23, 24, 26, 24, 26, 29, 27, 34, 30, 38, 34, 37, 36 }) == 285);
        assert(TwoSameSumSubset::solveBitDP2(vector<int>{ 156, 160, 153, 149, 158, 136, 172, 147, 148, 133, 147, 147, 146, 131, 171, 165, 145, 143, 148, 145 }) == 1500);
    }

    cout << "OK!" << endl;
}
