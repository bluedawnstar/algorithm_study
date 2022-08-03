#pragma once

#include <tuple>

struct Calendar {
    static bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    // 1.1.1 = monday
    static long long getTotalDays(int year, int month, int day) {
        static int mdays[] = {
            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };

        int y = year - 1;

        long long res = 365ll * y + y / 4 - y / 100 + y / 400;
        if (month > 2 && isLeapYear(year))
            res++;

        for (int i = 1; i < month; i++)
            res += mdays[i - 1];
        res += day;

        return res;
    }

    // magical algorithm
    // https://leetcode.com/problems/number-of-days-between-two-dates/discuss/517582/Python-Magical-Formula
    // http://www.ams.org/publicoutreach/feature-column/fcarc-time?fbclid=IwAR3XPF5qaGqb-oFrWlkGJT1TC56jWMwzJNbN6LathIswnyDAenBOzdREqjk
    static long long getTotalDaysSimple(int year, int month, int day) {
        if (month == 1 || month == 2) {
            month += 12;
            year--;
        }
        auto res = 365ll * year + year / 4 + year / 400 - year / 100 + day + (153 * month + 8) / 5;
        return res - 399;
    }


    static tuple<int, int, int> convertTotalDaysToDate(long long totalDays) {
        int year;
        {
            int lo = 0, hi = numeric_limits<int>::max();
            while (lo <= hi) {
                int mid = (lo + hi) >> 1;
                if (getTotalDaysSimple(mid, 12, 31) >= totalDays)
                    hi = mid - 1;
                else
                    lo = mid + 1;
            }
            year = lo;
        }

        long long days = getTotalDaysSimple(year, 1, 1) - 1;

        vector<int> mdays{
            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };
        if (isLeapYear(year))
            mdays[1]++;

        int i;
        for (i = 0; i < 12; i++) {
            if (totalDays <= days + mdays[i])
                break;
            days += mdays[i];
        }
        return make_tuple(year, i + 1, static_cast<int>(totalDays - days));
    }


    // [INFO] The combination of month, day, and day of the week repeats every 400 years. ((365 * 400 + 97) % 7 = 0)
    // 1.1.1 = 401.1.1 = monday
    // day of the week : 0 - sunday, 1 - monday, ..., 6 - saturday
};
