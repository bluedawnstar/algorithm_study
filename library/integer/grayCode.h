#pragma once

// https://e-maxx-eng.appspot.com/algebra/gray-code.html

// n = 0, 1, 2, ...
inline int nthGrayCode(int n) {
    return n ^ (n >> 1);
}

// n = inverse(nthGrayCode(n))
inline int inverseGrayCode(int g) {
    int n = 0;
    for (; g; g >>= 1)
        n ^= g;
    return n;
}
