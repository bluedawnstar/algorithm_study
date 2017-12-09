#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

#include "chineseRemainderTheorem.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testChineseRemainderTheorem() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Chinese Remainder Theorem -------------------------" << endl;
    {
        int num[] = { 3, 4, 5 };
        int rem[] = { 2, 3, 1 };
        int k = sizeof(num) / sizeof(num[0]);

        auto ans = solveCRT(num, rem, k);
        cout << ans << endl;
        assert(ans.first == 11 && ans.second == 60);
    }
    {
        int num[] = { 3, 5, 7 };
        int rem[] = { 2, 3, 2 };
        int k = sizeof(num) / sizeof(num[0]);

        auto ans = solveCRT(num, rem, k);
        cout << ans << endl;
        assert(ans.first == 23 && ans.second == 105);
    }
    {
        int num[] = { 4, 6 };
        int rem[] = { 3, 5 };
        int k = sizeof(num) / sizeof(num[0]);

        auto ans = solveCRTExt(num, rem, k);
        cout << ans << endl;
        assert(ans.first == 11 && ans.second == 12);
    }

    cout << "OK!" << endl;
}
