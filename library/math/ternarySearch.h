#pragma once

// return (argmax F(x), max F(x)) in [lo, hi], inclusive
inline pair<double,double> ternarySearchMax(double lo, double hi, const function<double(double)>& F) {
    for (int i = 0; i < 100; i++) {
        double a = (2 * lo + hi) / 3.0;
        double b = (lo + 2 * hi) / 3.0;
        if (F(a) > F(b))
            hi = b;
        else
            lo = a;
    }
    return make_pair((lo + hi) / 2.0, F((lo + hi) / 2.0));
}
