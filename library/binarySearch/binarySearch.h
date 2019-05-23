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

template <typename T, typename IterT = const T*>
IterT lowerBound(IterT lo, IterT hi, T x, const function<bool(T,T)>& lessOp = less<T>()) {
    while (lo <= hi) {
        IterT mid = lo + (hi - lo) / 2;
        if (!lessOp(*mid, x))
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return lo;
}

//---

//    xxxxxxxOOOOOOOOO
//    |      ^       |
//   lo              hi
// O(N), inclusive
template <typename T>
T lowerBound2(T lo, T hi, const function<bool(T)>& possible) {
    for (T step = T(hi - lo + 1) / 2; step >= 1; step >>= 1) {
        while (lo + step - 1 <= hi && !possible(lo + step - 1))
            lo += step;
    }
    return lo;
}

template <typename T, typename IterT = const T*>
IterT lowerBound2(IterT lo, IterT hi, T x, const function<bool(T,T)>& lessOp = less<T>()) {
    for (T step = T(hi - lo + 1) / 2; step >= 1; step >>= 1) {
        while (lo + step - 1 <= hi && lessOp(*(lo + step - 1), x))
            lo += step;
    }
    return lo;
}

//---

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

template <typename T, typename IterT = const T*>
IterT lowerBound3(IterT lo, IterT hi, T x, const function<bool(T,T)>& lessOp = less<T>()) {
    T step;
    for (step = 1; lo + step <= hi && lessOp(*(lo + step), x); step <<= 1)
        ;

    hi = min(hi, lo + step);
    lo += step >> 1;

    while (lo <= hi) {
        IterT mid = lo + (hi - lo) / 2;
        if (lessOp(*mid, x))
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    return lo;
}
