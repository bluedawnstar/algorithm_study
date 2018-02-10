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

void testBitDP() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Bit DP -----------------------------------" << endl;

    int N = 10;
    vector<int> A(1 << N);
    for (int i = 0; i < (1 << N); i++)
        A[i] = rand();

    auto gt = solveSOSNaive(N, A);
    auto ans = solveSOS(N, A);

    assert(gt == ans);

    cout << "OK!" << endl;
}
