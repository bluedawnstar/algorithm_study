#pragma once

//    xxxxxxxOOOOOOOOO
//    |      ^       |
//   lo              hi
// O(N), inclusive
template <typename T>
T lowerBound(T lo, T hi, const function<bool(T)>& possible) {
    while (lo <= hi) {
        T mid = lo + (hi - lo) / 2;
        if (possible(mid))
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return lo;
}

//    xxxxxxxOOOOOOOOO
//    |      ^       |
//   lo              hi
// O(N), inclusive
template <typename T>
T lowerBound2(T lo, T hi, const function<bool(T)>& possible) {
    T N = (hi - lo + 1);
    for (T step = N / 2; step >= 1; step >>= 1) {
        while (lo + step <= hi && !possible(lo + step - 1))
            lo += step;
    }
    return lo;
}

//    xxxxxxxOOOOOOOOO
//    |      ^       |
//   lo              hi
// O(N), inclusive
template <typename T>
T lowerBound3(T lo, T hi, const function<bool(T)>& possible) {
    while (lo <= hi && !possible(lo)) {
        ++lo;

        T step;
        for (step = 1; lo + step <= hi && !possible(lo + step); step <<= 1)
            ;
        lo += step >> 1;
    }
    return lo;
}
