#include <cmath>
#include <limits>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "weightedMedian.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testWeightedMedian() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Weighted Median -----------------------" << endl;
    {
        vector<int> X{ 1, 2, 3, 5, 6, 7 };
        vector<int> W{ 1, 1, 1, 1, 1, 1 };
        auto ans = WeightedMedian::findWeightedMedian(X, W);
        assert(3 <= ans.first && ans.first <= 5);
        assert(ans.second == 12);
    }
    {
        vector<int> X{ 1, 2, 3, 7, 8, 9 };
        vector<int> W{ 1, 1, 1, 1, 1, 1 };
        auto ans = WeightedMedian::findWeightedMedian(X, W);
        assert(3 <= ans.first && ans.first <= 7);
        assert(ans.second == 18);
    }
    {
        vector<int> X{ 1, 2, 3, 7, 9, 11 };
        vector<int> W{ 1, 1, 1, 1, 1, 1 };
        auto ans = WeightedMedian::findWeightedMedian(X, W);
        assert(3 <= ans.first && ans.first <= 7);
        assert(ans.second == 21);
    }
    {
        vector<int> X{ 1, 2, 3, 5, 6, 7 };
        vector<int> W{ 1, 2, 1, 2, 1, 2 };
        auto ans = WeightedMedian::findWeightedMedian(X, W);
        assert(ans.first == 5);
        assert(ans.second == 17);
    }
    cout << "OK!" << endl;
}
