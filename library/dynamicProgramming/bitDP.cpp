#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "bitDP.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testBitDP() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Bit DP -----------------------------------" << endl;

    int N = 10;
    vector<int> A(1 << N);
    for (int i = 0; i < (1 << N); i++)
        A[i] = RandInt32::get() % 65536;

    auto gt = solveSOSNaive(N, A);
    auto ans = solveSOS(N, A);

    assert(gt == ans);

    cout << "OK!" << endl;
}
