#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "digitDP.h"
#include "digitDPFast.h"
#include "digitCounter.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int sumDigits(int a, int b) {
    int res = 0;
    for (int i = a; i <= b; i++) {
        for (int t = i; t; t /= 10)
            res += t % 10;
    }
    return res;
}

static int countDigit(int a, int b, int d) {
    int res = 0;

    if (d == 0 && a == 0)
        res++;

    for (int i = a; i <= b; i++) {
        for (int t = i; t; t /= 10)
            res += ((t % 10) == d);
    }
    return res;
}


void testDigitDP() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Digit DP -----------------------------------" << endl;
    {
        DigitDP dp;
        assert(dp.rangeDigitSum(7, 7) == sumDigits(7, 7));
        assert(dp.rangeDigitSum(17, 18) == sumDigits(17, 18));
        assert(dp.rangeDigitSum(123, 1024) == sumDigits(123, 1024));
        assert(dp.rangeDigitSum(20, 21) == sumDigits(20, 21));
        assert(dp.rangeDigitSum(1000, 1009) == sumDigits(1000, 1009));

        assert(DigitDPFast::rangeDigitSum(7, 7) == sumDigits(7, 7));
        assert(DigitDPFast::rangeDigitSum(17, 18) == sumDigits(17, 18));
        assert(DigitDPFast::rangeDigitSum(123, 1024) == sumDigits(123, 1024));
        assert(DigitDPFast::rangeDigitSum(20, 21) == sumDigits(20, 21));
        assert(DigitDPFast::rangeDigitSum(1000, 1009) == sumDigits(1000, 1009));

        for (int i = 1; i < 1000; i++) {
            auto ans = DigitDPFast::rangeDigitSum(1, i);
            auto gt = sumDigits(1, i);
            if (ans != gt)
                cout << "Mismatched : digitSum(1, " << i << ") = " << ans << ", " << gt << endl;
        }

        for (int i = 0; i < 100; i++) {
            int a = RandInt32::get() % 10000;
            int b = RandInt32::get() % 10000;
            if (a > b)
                swap(a, b);
            auto ans = DigitDPFast::rangeDigitSum(a, b);
            auto gt = sumDigits(a, b);
            if (ans != gt)
                cout << "Mismatched : digitSum(" << a << ", " << b << ") = " << ans << ", " << gt << endl;
        }
    }
    {
        for (int i = 0; i < 1000; i++) {
            for (int d = 0; d < 10; d++) {
                auto ans1 = DigitDPFast::rangeDigitCount(0, i, d);
                auto ans2 = DigitCounter::count(0, i, d);
                auto gt = countDigit(0, i, d);
                if (ans1 != gt || ans2 != gt)
                    cout << "Mismatched : digitSum(1, " << i << ") = " << ans1 << ", " << ans2 << ", " << gt << endl;
                assert(ans1 == gt && ans2 == gt);
            }
        }

        for (int i = 0; i < 100; i++) {
            int a = RandInt32::get() % 10000;
            int b = RandInt32::get() % 10000;
            if (a > b)
                swap(a, b);
            int d = RandInt32::get() % 10;
            auto ans1 = DigitDPFast::rangeDigitCount(a, b, d);
            auto ans2 = DigitCounter::count(a, b, d);
            auto gt = countDigit(a, b, d);
            if (ans1 != gt || ans2 != gt)
                cout << "Mismatched : digitCount(" << a << ", " << b << ") = " << ans1 << ", " << ans2 << ", " << gt << endl;
            assert(ans1 == gt && ans2 == gt);
        }
    }
    {
        int T = 100000;
#ifdef _DEBUG
        T = 1000;
#endif

        DigitDP dp;

        PROFILE_START(0);
        long long ans1 = 0;
        for (int i = 0; i < T; i++)
            ans1 += dp.rangeDigitSum(1, i);
        PROFILE_STOP(0);

        PROFILE_START(1);
        long long ans2 = 0;
        for (int i = 0; i < T; i++)
            ans2 += DigitDPFast::rangeDigitSum(1, i);
        PROFILE_STOP(1);

        if (ans1 != ans2)
            cout << "ERROR" << endl;
    }
    cout << "OK!" << endl;
}
