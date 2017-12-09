#pragma once

// return the number of numbers with a digit in [0, n]
// PRECONDITION: digit != 0
template <typename T>
T countNumberWithDigit(T n, int digit) {
    if (n < digit)
        return 0;
    else if (n < 10)
        return 1;

    // get scale
    T scale = 1;
    for (T t = n / 10; t > 0; t /= 10)
        scale *= 10;

    // calculate the number of numbers with the digit
    T cnt = 0;
    for (T t = 1; t < scale; t *= 10)
        cnt = cnt * 9 + t;

    T res = 0;

    int msd = n / scale;
    if (msd < digit)
        res += msd * cnt + countNumberWithDigit(n % scale, digit);
    else if (msd == digit)
        res += msd * cnt + (n % scale + 1);
    else
        res += (msd - 1) * cnt + scale + countNumberWithDigit(n % scale, digit);

    return res;
}
