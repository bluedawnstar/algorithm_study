#pragma once

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
    static long long getTotalDaysSimple(int year, int month, int day) {
        if (month == 1 || month == 2) {
            month += 12;
            year--;
        }
        auto res = 365ll * year + year / 4 + year / 400 - year / 100 + day + (153 * month + 8) / 5;
        return res - 399;
    }


    // [INFO] The combination of month, day, and day of the week repeats every 800 years.
    // 1.1.1 = 801.1.1 = monday
    // day of the week : 0 - sunday, 1 - monday, ..., 6 - saturday
};
