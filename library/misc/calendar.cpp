#include <functional>
#include <vector>

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
            long long day2 = Calendar::getTotalDays(400 + i, 1, 1);
            if (day1 % 7 != day2 % 7)
                cout << "Mismatched : " << (day1 % 7) << ", " << (day2 % 7) << endl;
            assert(day1 % 7 == day2 % 7);
        }
    }
    {
        vector<tuple<int, int, int>> testSet{
            { 2022, 6, 28 },
            { 2020, 1, 1 },
            { 2020, 12, 31 },
            { 2021, 1, 1 },
            { 2021, 12, 31 },
            { 2022, 1, 1 },
            { 2022, 12, 31 },
            { 2023, 1, 1 },
            { 2023, 12, 31 },
            { 2020, 2, 29 },
            { 2020, 3, 1 },
            { 2021, 2, 28 },
            { 2021, 3, 1 },
            { 2022, 2, 28 },
            { 2022, 3, 1 },
            { 2023, 2, 28 },
            { 2023, 3, 1 },
            { 2024, 2, 29 },
            { 2024, 3, 1 },
        };
        int year, month, day;
        for (auto it : testSet) {
            tie(year, month, day) = it;
            auto days = Calendar::getTotalDays(year, month, day);

            auto date = Calendar::convertTotalDaysToDate(days);
            if (year != get<0>(date) || month != get<1>(date) || day != get<2>(date))
                cout << "Mismatched : " << year << "/" << month << "/" << day << ", "
                                        << get<0>(date) << "/" << get<1>(date) << "/" << get<2>(date) << endl;
        }
    }
    {
        int T = 1000000;
#ifdef _DEBUG
        T = 10000;
#endif
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

            long long days1 = Calendar::getTotalDays(year, month, day);
            long long days2 = Calendar::getTotalDaysSimple(year, month, day);
            if (days1 != days2)
                cout << "Mismatched : " << days1 << ", " << days2 << endl;
            assert(days1 == days2);

            auto date = Calendar::convertTotalDaysToDate(days1);
            if (year != get<0>(date) || month != get<1>(date) || day != get<2>(date))
                cout << "Mismatched : " << year << "/" << month << "/" << day << ", "
                                        << get<0>(date) << "/" << get<1>(date) << "/" << get<2>(date) << endl;
            assert(year == get<0>(date) && month == get<1>(date) && day == get<2>(date));
        }
    }

    cout << "OK!" << endl;
}
