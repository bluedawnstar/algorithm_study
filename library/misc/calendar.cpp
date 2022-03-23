#include <functional>

using namespace std;

#include "calendar.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testCalendar() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Calendar ---------------------------" << endl;
    {
        assert(Calendar::getTotalDays(1, 1, 1) % 7 == 1);
        for (int i = 1; i <= 800; i++) {
            long long day1 = Calendar::getTotalDays(i, 1, 1);
            long long day2 = Calendar::getTotalDays(800 + i, 1, 1);
            if (day1 % 7 != day2 % 7)
                cout << "Mismatched : " << (day1 % 7) << ", " << (day2 % 7) << endl;
            assert(day1 % 7 == day2 % 7);

            long long day3 = Calendar::getTotalDays(400 + i, 1, 1);
            if (day1 % 7 != day3 % 7)
                cout << "Mismatched : " << (day1 % 7) << ", " << (day3 % 7) << endl;
            assert(day1 % 7 == day3 % 7);
        }
    }
    {
        int T = 10000;
        while (T-- > 0) {
            int year = RandInt32::get() % 99999 + 1;
            int month = RandInt32::get() % 12 + 1;
            int day = RandInt32::get() % 31 + 1;
            switch (month) {
            case 2:
                if (Calendar::isLeapYear(year))
                    day = min(day, 29);
                else
                    day = min(day, 28);
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                day = min(day, 30);
            }

            long long day1 = Calendar::getTotalDays(year, month, day);
            long long day2 = Calendar::getTotalDaysSimple(year, month, day);
            if (day1 != day2)
                cout << "Mismatched : " << day1 << ", " << day2 << endl;
            assert(day1 == day2);
        }
    }

    cout << "OK!" << endl;
}
