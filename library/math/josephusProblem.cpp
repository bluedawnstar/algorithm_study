#include <cmath>
#include <functional>
#include <bitset>
#include <vector>
#include <algorithm>

using namespace std;

#include "josephusProblem.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testJosephusProblem() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Josephus problem -----------------------" << endl;
    {
        assert(JosephusProblemK2<int>::getSafePosition(5) + 1 == 3);
        assert(JosephusProblemK2<int>::getSafePosition2(5) + 1 == 3);
    }
    {
        for (int N = 100; N <= 200; N++) {
            for (int K = 2; K <= N; K++) {
                int ans1 = JosephusProblem<int>::getSafePosition(N, K);
                int ans2 = JosephusProblem<int>::getSafePositionNaive(N, K);
                if (ans1 != ans2)
                    cout << "Mismatched (" << N << ", " << K << ") : " << ans1 << ", " << ans2 << endl;
                assert(ans1 == ans2);
            }
        }
    }
    cout << "OK!" << endl;
}
