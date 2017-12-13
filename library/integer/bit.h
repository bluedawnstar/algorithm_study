#pragma once

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


inline int popcount(unsigned x) {
#ifndef __GNUC__
    return _mm_popcnt_u32(x);
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
    return _mm_popcnt_u32(unsigned(x)) + _mm_popcnt_u32(unsigned(x >> 32));
#else
    return __builtin_popcountll(x);
#endif
}


template <typename T>
inline T getLastBit(T x) {
    return x & -x;
}

template <typename T>
inline T clearLastBit(T x) {
    return x & (x - 1);
}


inline int log2Int(unsigned x) {
    return clz(1u) - clz(x);
}

inline int log2Int(unsigned long long x) {
    return clz(1ull) - clz(x);
}
