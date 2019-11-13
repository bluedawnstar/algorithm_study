#include <cmath>
#include <vector>
#include <deque>
#include <numeric>
#include <algorithm>

using namespace std;

#include "maxProfit.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testMaxProfit() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Max Profit -----------------------------------" << endl;
    {
        auto ans = maxProfit(vector<int>{ 3, 2, 6, 5, 0, 3 }, 2);
        int gt = 7;
        if (ans.back() != gt)
            cout << "Invalid answer : " << ans << ", " << gt << endl;
        assert(ans.back() == gt);
    }
    {
        auto ans1 = maxProfitWithMultipleTransactions(vector<int>{ 4, 1, 5 }, 1);
        vector<int> gt1{ 4 };
        if (ans1 != gt1)
            cout << "Invalid answer : " << ans1 << ", " << gt1 << endl;
        assert(ans1 == gt1);

        auto ans2 = maxProfitWithMultipleTransactions(vector<int>{ 3, 4, 5, 1, 4 }, 2);
        vector<int> gt2{ 3, 5 };
        if (ans2 != gt2)
            cout << "Invalid answer : " << ans2 << ", " << gt2 << endl;
        assert(ans2 == gt2);

        auto ans3 = maxProfitWithMultipleTransactions(vector<int>{ 85, 107, 193, 306, 345 }, 2);
        vector<int> gt3{ 260, 459 };
        if (ans3 != gt3)
            cout << "Invalid answer : " << ans3 << ", " << gt3 << endl;
        assert(ans3 == gt3);

        auto ans4 = maxProfitWithMultipleTransactions(vector<int>{ 1, 3, 2, 4 }, 2);
        vector<int> gt4{ 3, 4 };
        if (ans4 != gt4)
            cout << "Invalid answer : " << ans4 << ", " << gt4 << endl;
        assert(ans4 == gt4);
    }

    cout << "OK!" << endl;
}
