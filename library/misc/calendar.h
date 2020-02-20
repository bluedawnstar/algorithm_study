#pragma once

struct Calendar {
    static bool isLeapYear(int y) {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }

    // 1.1.1 = monday
    static long long getTotalDays(int year, int month, int day) {
        static int mdays[] = {
            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };

        int y = year - 1;

        long long res = 365 * y + y / 4 - y / 100 + y / 400;
        if (month > 2 && isLeapYear(year))
            res++;

        for (int i = 1; i < month; i++)
            res += mdays[i - 1];
        res += day;

        return res;
    }

    // [INFO] The combination of month, day, and day of the week repeats every 800 years.
    // 1.1.1 = 801.1.1 = monday
    // day of the week : 0 - sunday, 1 - monday, ..., 6 - saturday
};
