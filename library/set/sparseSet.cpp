#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

#include "sparseSet.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "bitSetSimple.h"

static vector<int> toVector(BitSetSimple& bs) {
    vector<int> res;
    for (int i = 0; i < bs.N; i++) {
        if (bs.test(i))
            res.push_back(i);
    }
    return res;
}


void testSparseSet() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Sparse Set ------------------------" << endl;
    {
        int N = 10000;
        int T = 10000;

        BitSetSimple bs1(N);
        BitSetSimple bs2(N);
        BitSetSimple bs3(N);
        SparseSet ss1(N - 1);
        SparseSet ss2(N - 1);
        SparseSet ss3(N - 1);

        for (int i = 0; i < T; i++) {
            int x = RandInt32::get() % N;
            switch (i % 3) {
            case 0:
                if (i & 1) {
                    bs1.set(x);
                    ss1.set(x);
                } else {
                    bs1.reset(x);
                    ss1.reset(x);
                }
                break;
            case 1:
                if (i & 1) {
                    bs2.set(x);
                    ss2.set(x);
                } else {
                    bs2.reset(x);
                    ss2.reset(x);
                }
                break;
            case 2:
                if (i & 1) {
                    bs3.set(x);
                    ss3.set(x);
                } else {
                    bs3.reset(x);
                    ss3.reset(x);
                }
                break;
            }
        }

        assert(toVector(bs1) == ss1.toVector());
        assert(toVector(bs2) == ss2.toVector());
        assert(toVector(bs3) == ss3.toVector());

        auto bsAnd = bs1 & bs2;
        auto ssAnd = ss1 & ss2;
        assert(toVector(bsAnd) == ssAnd.toVector());

        auto bsOr = bs1 | bs2;
        auto ssOr = ss1 | ss2;
        assert(toVector(bsOr) == ssOr.toVector());
    }

    cout << "OK!" << endl;
}
