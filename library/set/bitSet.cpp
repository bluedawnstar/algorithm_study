#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;

#include "bitSet.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

#define N   100

bool checkBitset(int n, bitset<N>& bs1, BitSet& bs2) {
    for (int i = 0; i < n; i++)
        if (bs1.test(i) != bs2.test(i))
            return false;
    return true;
}

void testBitSet() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Bit Set ---------------------------" << endl;

    bitset<N> bs1;
    BitSet bs2;
    bs2.init(N);

    for (int i = 0; i < 10000; i++) {
        switch (rand() % 3) {
        case 0: {
            int pos = rand() % N;
            bool val = (rand() % 2) == 1;
            bs1.set(pos, val);
            bs2.set(pos, val);
            break;
        }
        case 1: {
            int pos = rand() % N;
            bs1.reset(pos);
            bs2.reset(pos);
            break;
        }
        case 2: {
            int pos = rand() % N;
            bs1.flip(pos);
            bs2.flip(pos);
            break;
        }
        }
        assert(check(bs1, bs2));
    }

    cout << "OK!" << endl;
}
