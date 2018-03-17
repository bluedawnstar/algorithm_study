#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

#include "bitVectorRank.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int count(vector<bool>& v, int L, int R) {
    int res = 0;
    for (int i = L; i <= R; i++) {
        res += v[i];
    }
    return res;
}

void testBitVectorRank() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- BitVector with Rank --------------------------------------" << endl;

    int N = 100;
    int T = 10000;
    for (int i = 0; i < T; i++) {
        vector<bool> in(N);
        for (int j = 0; j < N; j++)
            in[j] = bool(RandInt32::get() & 1);

        BitVectorRank vec;
        vec.init(N);
        for (int j = 0; j < N; j++) {
            if (in[j])
                vec.set(j);
        }
        vec.buildRank();

        for (int j = 0; j < 10; j++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;

            if (L > R)
                swap(L, R);

            int gt = count(in, 0, R);
            int ans = vec.rank1(R);
            if (ans != gt) {
                cout << "GT = " << gt << ", " << "ans = " << ans << endl;
                ans = vec.rank1(R);
            }
            assert(ans == gt);

            gt = count(in, L, R);
            ans = vec.rank1(L, R);
            if (ans != gt) {
                cout << "GT = " << gt << ", " << "ans = " << ans << endl;
                ans = vec.rank1(L, R);
            }
            assert(ans == gt);
        }

        vec.set();
        vec.buildRank();
        assert(vec.count() == vec.rank1(N - 1));
    }

    cout << "OK!" << endl;
}
