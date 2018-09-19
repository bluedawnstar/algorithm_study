#pragma once

// return the number of numbers with a digit in [0, n]
// 0 < digit <= 9
template <typename T>
T countDigit(T n, int digit) {
    if (n < digit)
        return 0;
    else if (n < 10)
        return 1;

    T scale = 1;
    for (T t = n / 10; t > 0; t /= 10)
        scale *= 10;

    int msd = n / scale;

    int res = 0;

    res += msd * countDigit(scale - 1, digit);  // ex) countDigit(2345, 2) --> 2 * countDigit(999, 2)
    res += countDigit(n % scale, digit);        // ex) countDigit(2345, 2) --> countDigit(345, 2)

    if (msd > digit)
        res += scale;                           // ex) countDigit(2345, 1) --> 1000
    else if (msd == digit)
        res += n % scale + 1;                   // ex) countDigit(2345, 2) --> 345 + 1

    return res;
}
