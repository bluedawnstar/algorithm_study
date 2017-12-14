#pragma once

// counting leading zeros
inline int clz(unsigned x) {
#ifndef __GNUC__
    return (int)__lzcnt(x);
#else
    return __builtin_clz(x);
#endif
}

inline int clz(unsigned long long x) {
    if ((x >> 32) != 0)
        return clz(unsigned(x >> 32));
    else
        return 32 + clz(unsigned(x));
}

// pop count
inline int popcount(unsigned x) {
#ifndef __GNUC__
    return (int)__popcnt(x);
    /*
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x0000003F;
    */
#else
    return __builtin_popcount(x);
#endif
}

inline int popcount(unsigned long long x) {
#ifndef __GNUC__
    return (int)__popcnt(unsigned(x)) + (int)__popcnt(unsigned(x >> 32));
#else
    return __builtin_popcountll(x);
#endif
}

// last '1' bit
template <typename T>
inline T getLastBit(T x) {
    return x & -x;
}

template <typename T>
inline T clearLastBit(T x) {
    return x & (x - 1);
}

// integer log2
inline int log2Int(unsigned x) {
    return clz(1u) - clz(x);
}

inline int log2Int(unsigned long long x) {
    return clz(1ull) - clz(x);
}

// submasks
template <typename T>
inline vector<T> getAllSubmasks(T mask) {
    vector<T> res;
    for (T s = mask; ; s = (s - T(1)) & mask) {
        res.push_back(s);
        if (!s)
            break;
    }
    return res;
}

// O(3^n)
inline void foreachAllSubmasksOfAllMasks(int n) {
    for (int m = 0; m < (1 << n); m++) {
        for (int s = m; s; s = (s - 1) & m) {
            //... do something!...
        }
    }
}