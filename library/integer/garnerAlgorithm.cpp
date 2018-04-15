#include <cassert>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <iostream>

using namespace std;

#include "garnerAlgorithm.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static bigint getRandomBigint(int len) {
    string s;
    for (int i = 0; i < len; i++)
        s += RandInt32::get() % 10 + '0';
    return bigint(s);
}

void testGarnerAlgorithm() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Garner Algorithm ----------------------------------" << endl;
    {
        bigint a("99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
        bigint b("19999999999999999999999999999999999999999999999999999999999999999999999999999999999999998");
        auto ans1 = a * b;

        GarnerBigInt ga(a), gb(b);
        auto ans2 = ga * gb;

        assert(ans1 == ans2.get());

        for (int step = 0; step < 100; step++) {
            bigint a = getRandomBigint(900);
            bigint b = getRandomBigint(900);
            bigint x = a * b;

            GarnerBigInt ga(a), gb(b);
            auto y = ga * gb;

            assert(x == y.get());
        }
    }
    {
        int N = 1;
        vector<bigint> x(N), y(N);
        for (int i = 0; i < N; i++) {
            x[i] = getRandomBigint(60000);
            y[i] = getRandomBigint(60000);
        }

        PROFILE_START(0);
        for (int i = 0; i < N; i++) {
            bigint z = x[i] * y[i];
            if (z.sign < 0)
                cout << "What?" << endl;
        }
        PROFILE_STOP(0);


        vector<GarnerBigInt> gx(N), gy(N);
        for (int i = 0; i < N; i++) {
            gx[i] = getRandomBigint(60000);
            gy[i] = getRandomBigint(60000);
        }

        PROFILE_START(1);
        for (int i = 0; i < N; i++) {
            auto z = gx[i] * gy[i];
            if (z.get().sign < 0)
                cout << "What?" << endl;
        }
        PROFILE_STOP(1);
    }
    {
        bigint x(6);
        assert(x == bigint(6));

        GarnerBigInt y(6);
        assert(x == y.get());
    }
    cout << "OK!" << endl;
}
