#pragma once

// return the number of numbers with a digit in [0, n]
// PRECONDITION: digit != 0
template <typename T>
T countNumberWithDigit(T n, int digit) {
    if (n < digit)
        return 0;
    else if (n < 10)
        return 1;

    T scale = 1;
    for (T t = n / 10; t > 0; t /= 10)
        scale *= 10;

    int msd = n / scale;

    int res = 0;

    res += msd * countNumberWithDigit(scale - 1, digit);    // ex) count(0 ~ 2345, 2) --> 2 * count(0 ~ 999, 2)
    res += countNumberWithDigit(n % scale, digit);          // ex) count(0 ~ 2345, 2) --> count(0 ~ 345, 2)

    if (msd > digit)
        res += scale;                                       // ex) count(0 ~ 2345, 1) --> 1000 (2xxx)
    else if (msd == digit)
        res += n % scale + 1;                               // ex) count(0 ~ 2345, 2) --> 345 + 1 (2xxx)

    return res;
}
