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

#define N   (1024 - 31)

bool check(bitset<N>& bs1, BitSet& bs2) {
    for (int i = 0; i < N; i++)
        if (bs1.test(i) != bs2.test(i))
            return false;
    return true;
}

bool check(bitset<N>& bs1, BitSetSimple& bs2) {
    for (int i = 0; i < N; i++)
        if (bs1.test(i) != bs2.test(i))
            return false;
    return true;
}

void testBitSet() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Bit Set ---------------------------" << endl;
    
    bitset<N> bs1;
    BitSet bs2;
    BitSetSimple bs3;
    bs2.init(N);
    bs3.init(N);

    for (int i = 0; i < 10000; i++) {
        switch (rand() % 8) {
        case 0: {
            int pos = rand() % N;
            bool val = (rand() % 2) == 1;
            bs1.set(pos, val);
            bs2.set(pos, val);
            bs3.set(pos, val);
            break;
        }
        case 1: {
            int pos = rand() % N;
            bs1.reset(pos);
            bs2.reset(pos);
            bs3.reset(pos);
            break;
        }
        case 2: {
            int pos = rand() % N;
            bs1.flip(pos);
            bs2.flip(pos);
            bs3.flip(pos);
            break;
        }
        case 3: {
            int n = rand() % 128 + 1;
            bs1 <<= n;
            bs2 <<= n;
            bs3 <<= n;
            break;
        }
        case 4: {
            int n = rand() % 128 + 1;
            bs1 >>= n;
            bs2 >>= n;
            bs3 >>= n;
            break;
        }
        case 5:
        {
            bs1.set();
            bs2.set();
            bs3.set();
            break;
        }
        case 6:
        {
            bs1.reset();
            bs2.reset();
            bs3.reset();
            break;
        }
        case 7:
        {
            bs1.flip();
            bs2.flip();
            bs3.flip();
            break;
        }
        }
        assert(bs1.count() == bs2.count());
        assert(bs1.all() == bs2.all());
        assert(bs1.any() == bs2.any());
        assert(bs1.none() == bs2.none());
        assert(check(bs1, bs2));

        assert(bs1.count() == bs3.count());
        assert(bs1.all() == bs3.all());
        assert(bs1.any() == bs3.any());
        assert(bs1.none() == bs3.none());
        assert(check(bs1, bs3));
    }

    cout << "OK!" << endl;
}
