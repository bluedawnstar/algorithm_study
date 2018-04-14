#include <cmath>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

#include "bigint.h"

//https://e-maxx-eng.appspot.com/algebra/big-integer.html

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
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

void testBigInt() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "------------ Big Integer --------------" << endl;
    {
        bigint a("99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
        bigint b("19999999999999999999999999999999999999999999999999999999999999999999999999999999999999998");
        auto ans1 = a * b;
        auto ans2 = a.multiplyKaratsuba(b);
        auto ans3 = a.multiplySimple(b);
        cout << ans1 << ", " << ans2 << ", " << ans3 << endl;
        assert(ans1 == ans3);
        assert(ans1 == ans2);

        cout << "a * b = " << a * b << endl;
        cout << "a / b = " << a / b << endl;
        cout << "a % b = " << a % b << endl;

        for (int step = 0; step < 100; step++) {
            bigint a = getRandomBigint(1000);
            bigint b = getRandomBigint(1000);
            bigint x = a * b;
            bigint y = a.multiplySimple(b);
            bigint z = a.multiplyKaratsuba(b);
            assert(x == y);
            assert(x == z);
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

        PROFILE_START(1);
        for (int i = 0; i < N; i++) {
            bigint z = x[i].multiplyKaratsuba(y[i]);
            if (z.sign < 0)
                cout << "What?" << endl;
        }
        PROFILE_STOP(1);
    }
    {
        auto a = getRandomBigint(10000);
        auto b = getRandomBigint(2000);
        PROFILE_START(2);
        bigint c = a / b;
        PROFILE_STOP(2);
        if (c.sign < 0)
            cout << "What?" << endl;
    }
    {
        bigint z(5);
        z = 6;
        assert(z == bigint(6));
    }
    cout << "OK!" << endl;
}
