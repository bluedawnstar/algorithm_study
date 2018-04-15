#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "eggDropping.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testEggDropping() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Egg Dropping ------------------------------------" << endl;
    {
        int MaxN = 10;
        int MaxH = 20;
        int T = 10;
        for (int i = 0; i < T; i++) {
            int n = RandInt32::get() % MaxN + 1;
            int h = RandInt32::get() % MaxH + 1;
            int ans1 = eggDroppingRecursive(n, h);
            int ans2 = eggDroppingDP(n, h);
            int ans3 = eggDroppingBinarySearch(n, h);
            assert(ans1 == ans2);
            assert(ans2 == ans3);
        }
    }
    {
        int MaxN = 21;
        int MaxH = 801;
        int T = 10;
        for (int i = 0; i < T; i++) {
            int n = RandInt32::get() % MaxN + 1;
            int h = RandInt32::get() % MaxH + 1;
            int ans2 = eggDroppingDP(n, h);
            int ans3 = eggDroppingBinarySearch(n, h);
            assert(ans2 == ans3);
        }
    }

    cout << "OK!" << endl;
}
