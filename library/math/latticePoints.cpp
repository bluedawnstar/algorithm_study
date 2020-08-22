#include <cmath>
#include <vector>
#include <limits>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

#include "latticePoints.h"

// https://cp-algorithms.com/geometry/lattice-points.html


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <assert.h>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

template <typename T>
static T gcd(T p, T q) {
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

static long long sumFloorSlow(long long a, long long b, long long L, long long R) {
    long long res = 0;

    for (long long i = L; i <= R; i++)
        res += a * i / b;

    return res;
}

static long long sumFloorSlow(long long a, long long b, long long c, long long L, long long R) {
    long long res = 0;

    for (long long i = L; i <= R; i++)
        res += a * i / b + c;

    return res;
}

static long long sumFloorSlow(Fraction<long long> a, Fraction<long long> b, long long L, long long R) {
    long long res = 0;

    for (long long i = L; i <= R; i++) {
        res += (a * i + b).floor();
    }

    return res;
}

void testLatticePoints() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--------- Lattice Points -----------------" << endl;
    {
        int N = 1000000;
        int T = 100;
        for (int i = 0; i < T; i++) {
            int a = RandInt32::get() % N + 1;
            int b = RandInt32::get() % N + 1;
            int n = RandInt32::get() % N;

            int g = gcd(a, b);
            a /= g;
            b /= g;

            long long ans = LatticePoints::count(a, b, n);
            long long gt = sumFloorSlow(a, b, 0, n);

            if (ans != gt)
                cout << "Mismatched : sumFloor(" << n << ", " << a << ", " << b << ") = " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    {
        int N = 1000000;
        int T = 100;
        for (int i = 0; i < T; i++) {
            int a = RandInt32::get() % N + 1;
            int b = RandInt32::get() % N + 1;
            int n = RandInt32::get() % N;

            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);

            int g = gcd(a, b);
            a /= g;
            b /= g;

            long long ans = LatticePoints::countRange(a, b, L, R);
            long long gt = sumFloorSlow(a, b, L, R);

            if (ans != gt)
                cout << "Mismatched : sumFloor(" << n << ", " << a << ", " << b << ") = " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    {
        int N = 1000000;
        int T = 100;
        for (int i = 0; i < T; i++) {
            int a = RandInt32::get() % N + 1;
            int b = RandInt32::get() % N + 1;
            int c = RandInt32::get() % N + 1;
            int n = RandInt32::get() % N;

            int g = gcd(a, b);
            a /= g;
            b /= g;

            long long ans = LatticePoints::count(a, b, c, n);
            long long gt = sumFloorSlow(a, b, c, 0, n);

            if (ans != gt)
                cout << "Mismatched : sumFloor(" << n << ", " << a << ", " << b << ", " << c << ") = " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    {
        int N = 1000000;
        int T = 100;
        for (int i = 0; i < T; i++) {
            int a = RandInt32::get() % N + 1;
            int b = RandInt32::get() % N + 1;
            int c = RandInt32::get() % N + 1;
            int n = RandInt32::get() % N;

            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);

            int g = gcd(a, b);
            a /= g;
            b /= g;

            long long ans = LatticePoints::countRange(a, b, c, L, R);
            long long gt = sumFloorSlow(a, b, c, L, R);

            if (ans != gt)
                cout << "Mismatched : sumFloor(" << n << ", " << a << ", " << b << ", " << c << ") = " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    {
        int N = 1000000;
        int T = 100;
        for (int i = 0; i < T; i++) {
            int a = RandInt32::get() % N + 1;
            int b = RandInt32::get() % N + 1;
            int c = RandInt32::get() % N + 1;
            int n = RandInt32::get() % N;

            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);

            int g = gcd(a, b);
            a /= g;
            b /= g;

            Fraction<long long> fa(a, b);
            Fraction<long long> fb(c, 1);
            fa.reduce();

            long long ans = LatticePoints::countRange(fa, fb, L, R);
            long long gt = sumFloorSlow(a, b, c, L, R);

            if (ans != gt)
                cout << "Mismatched : sumFloor(" << n << ", (" << a << "/" << b << "), " << c << ") = " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    {
        int N = 100000;
        int T = 100;
        for (int i = 0; i < T; i++) {
            int a = RandInt32::get() % N + 1;
            int b = RandInt32::get() % N + 1;
            int c = RandInt32::get() % N + 1;
            int d = RandInt32::get() % N + 1;
            int n = RandInt32::get() % N;

            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);

            int g = gcd(a, b);
            a /= g;
            b /= g;

            g = gcd(c, d);
            c /= g;
            d /= g;

            Fraction<long long> fa(a, b);
            Fraction<long long> fb(c, d);

            fa.reduce();
            fb.reduce();
            long long ans = LatticePoints::countRange(fa, fb, L, R);
            long long gt = sumFloorSlow(fa, fb, L, R);

            if (ans != gt)
                cout << "Mismatched : sumFloor(" << n << ", (" << a << "/" << b << "), (" << c << "/" << d << ")) = " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
