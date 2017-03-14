#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "factor.h"

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


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testGcd() {
    //return; //TODO: if you want to test functions of this file, make this line to a comment.

    cout << "--- gcd() / lcm() / extGcd() -------------------------" << endl;

    assert(gcd(2 * 2 * 2 * 2 * 2, 3 * 3 * 3) == 1);
    assert(gcd(2 * 3 * 2 * 3 * 7, 3 * 3 * 3) == 9);
    int a = 134232, b = 93231;
    assert(gcd(a, b) * lcm(a, b) == a * b);

    int x, y;
    extGcd(a, b, x, y);
    assert(a * x + b * y == gcd(a, b));
    cout << "extGcd(" << a << ", " << b << ") = " << make_pair(x, y) << endl;

    cout << "OK!" << endl;
}
