#pragma once

// return maximum value of f(x) in inclusive range [lo,hi]
inline double findMaxWithTernary(double lo, double hi, function<double(double)> f) {
    for (int i = 0; i < 100; i++) {
        double a = (2 * lo + hi) / 3.0;
        double b = (lo + 2 * hi) / 3.0;
        if (f(a) > f(b))
            hi = b;
        else
            lo = a;
    }
    return (lo + hi) / 2.0;
}
