#pragma once

// recursive
template <typename T>
T gcd(T p, T q) {
    return q == 0 ? p : gcd(q, p % q);
}

// iterative
template <typename T>
T gcd2(T p, T q) {
    if (p < q)
        swap(p, q);

    T t;
    while (q != 0) {
        t = q;
        q = p % q;
        p = t;
    }

    return p;
}

template <typename T>
T lcm(T p, T q) {
    return (p / gcd(p, q)) * q;
}


// a * x + b * y = gcd(a, b)
template <typename T>
T extGcd(T a, T b, T& x, T& y) {
    if (b == 0) {
        x = T(1);
        y = T(0);
        return a;
    }

    T x1, y1;
    T g = extGcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}


inline unsigned int gcdFast(unsigned int p, unsigned int q) {
    if (p == 0)
        return q;
    if (q == 0)
        return p;

#ifdef __GNUC__
    int shift = int(__builtin_ctz(p | q));

    p >>= __builtin_ctz(p);
    do {
        q >>= __builtin_ctz(q);
        if (p > q) {
            unsigned int t = q;
            q = p;
            p = t;
        }
        q -= p;
    } while (q != 0);

    return p << shift;
#else
    int shift = _tzcnt_u32(p | q);
    
    p >>= _tzcnt_u32(p);
    do {
        q >>= _tzcnt_u32(q);
        if (p > q) {
            unsigned int t = q;
            q = p;
            p = t;
        }
        q -= p;
    } while (q != 0);
    
    return p << shift;
#endif
}

inline unsigned long long gcdFast(unsigned long long p, unsigned long long q) {
    if (p == 0)
        return q;
    if (q == 0)
        return p;

#ifdef __GNUC__
    int shift = int(__builtin_ctzll(p | q));

    p >>= __builtin_ctzll(p);
    do {
        q >>= __builtin_ctzll(q);
        if (p > q) {
            unsigned long long t = q;
            q = p;
            p = t;
        }
        q -= p;
    } while (q != 0);

    return p << shift;
#elif defined(_M_X64) 
    int shift = int(_tzcnt_u64(p | q));

    p >>= _tzcnt_u64(p);
    do {
        q >>= _tzcnt_u64(q);
        if (p > q) {
            unsigned long long t = q;
            q = p;
            p = t;
        }
        q -= p;
    } while (q != 0);

    return p << shift;
#else
    return gcd(p, q);
#endif
}
