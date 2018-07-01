#include <vector>
#include <algorithm>

using namespace std;

#include "bitSetRangeFlip.h"

// <Related problems>
// https://www.hackerrank.com/contests/hourrank-28/challenges/xorry-queries

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int countOne(vector<bool>& v, int left, int right) {
    int res = 0;
    while (left <= right)
        res += v[left++];
    return res;
}

static void flip(vector<bool>& v, int left, int right) {
    for (; left <= right; left++)
        v[left] = !v[left];
}

void testBitSetRangeUpdate() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Bit Set with Range Flip ---------------------------" << endl;
    {
        int N = 1000;
        int T = 10000;

        vector<bool> v(N);
        for (int i = 0; i < N; i++)
            v[i] = (RandInt32::get() & 1) != 0;

        BitSetRangeFlip bs(v, true);

        for (int i = 0; i < T; i++) {
            int left = RandInt32::get() % N;
            int right = RandInt32::get() % N;
            if (left > right)
                swap(left, right);

            if (RandInt32::get() & 1) {
                flip(v, left, right);
                bs.flip(left, right);
            } else {
                assert(countOne(v, left, right) == bs.countOne(left, right));
            }
        }
    }

    cout << "OK!" << endl;
}
