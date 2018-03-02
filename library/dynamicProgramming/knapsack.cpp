#include <vector>
#include <algorithm>

using namespace std;

#include "knapsack.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testKnapsack() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

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

    cout << "OK!" << endl;
}
