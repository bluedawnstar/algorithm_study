#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "bit.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <climits>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testBit() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Bit -------------------------" << endl;

    assert(clz(1u) == 31);
    assert(clz(0u) == 32);
    assert(clz(3u) == 30);
    assert(popcount(0x10101010u) == 4);
    assert(popcount(0xA5A5A5A5u) == 16);
    assert(popcount(0xFFFFFFFFu) == 32);

    PROFILE_START(0);
    for (int i = 1; i < 10000000; i++) {
        int n = int(log2(i));
        if (n < 0)
            cerr << "What?" << endl;
    }
    PROFILE_STOP(0);

    PROFILE_START(1);
    for (int i = 1; i < 10000000; i++) {
        int n = log2Int((unsigned)i);
        if (n < 0)
            cerr << "What?" << endl;
    }
    PROFILE_STOP(1);

    cout << "OK!" << endl;
}
