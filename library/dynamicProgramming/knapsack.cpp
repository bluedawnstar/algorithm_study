#include <vector>
#include <algorithm>

using namespace std;

#include "knapsack.h"
#include "knapsack2.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testKnapsack() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Knapsack Problems ------------------------" << endl;
    {
        vector<int> value{ 60, 100, 120 };
        vector<int> weight{ 10, 20, 30 };
        int W = 50;
        assert(knapsackNaive(weight, value, W) == 220);
        assert(knapsack(weight, value, W) == 220);
        assert(knapsack(weight, value, W, 200) == 220);
    }
    {
        vector<int> value{ 10, 30, 20 };
        vector<int> weight{ 5, 10, 15 };
        int W = 100;
        assert(unboundedKnapsack(weight, value, W) == 300);
    }
    {
        vector<int> A{ 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 14, 23, 37, 61, 9 };
        vector<int> B{ 1, 1, 4, 2, 2 };
        vector<int> C{ 31, 26, 33, 21, 40 };
        
        assert(MinLastValue::solveWithMeetInTheMiddle(A) == 0);
        assert(MinLastValue::solveWithKnapsack(A, 1000) == 0);

        assert(MinLastValue::solveWithMeetInTheMiddle(B) == 0);
        assert(MinLastValue::solveWithKnapsack(B, 1000) == 0);

        assert(MinLastValue::solveWithMeetInTheMiddle(C) == 5);
        assert(MinLastValue::solveWithKnapsack(C, 1000) == 5);
    }

    cout << "OK!" << endl;
}
