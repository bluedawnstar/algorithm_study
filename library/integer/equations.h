#pragma once

#include "intMod.h"
#include "gcd.h"

// finds all solutions to ax = b (mod M)
template <typename T, typename U>
inline vector<T> solveModularEq(T a, T b, U M) {
    vector<T> res;

    T x, y;
    T g = extGcd(a, M, x, y);
    if (!(b % g)) {
        x = (x * (b / g) % M + M) % M;
        for (int i = 0; i < g; i++)
            res.push_back((x + i * (M / g) % M + M) % M);
    }
    sort(res.begin(), res.end());

    return res;
}

// https://e-maxx-eng.appspot.com/algebra/linear-diophantine-equation.html
// find (x, y) satisfying ax + by = c (diophantine equation)
template <typename T>
struct DiophantineEquation {
    // find any solution
    // return whether the solution exists
    static bool findAny(T a, T b, T c, T& x0, T& y0, T& g) {
        g = extGcd(abs(a), abs(b), x0, y0);
        if (c % g)
            return false;

        x0 *= c / g;
        y0 *= c / g;
        if (a < 0)
            x0 = -x0;
        if (b < 0)
            y0 = -y0;

        return true;
    }

    // make solution generator
    //   (x, y) = (x0 + k * b / g, y0 - k * a / g)
    //          = (result.first(k), y = result.second(k)),  k = any integer
    static pair<function<T(T)>, function<T(T)>> makeSolutionGenerator(T a, T b, T c) {
        T x0, y0, g;
        findAny(a, b, c, x0, y0, g);
        return make_pair([=](T k) { return x0 + k * b / g; }, [=](T k) { return y0 - k * a / g; });
    }

    // find the number of solutions and the solutions in a given interval
    // return (count, lx)
    //   --> x = lx + k * b     , k = 0, 1, ..., count - 1
    //   --> y = (c - a * x) / b
    static pair<T, T> findAll(T a, T b, T c, T minx, T maxx, T miny, T maxy) {
        T x, y, g;
        if (!findAny(a, b, c, x, y, g))
            return make_pair(0, minx);

        a /= g;
        b /= g;

        T sign_a = a > 0 ? +1 : -1;
        T sign_b = b > 0 ? +1 : -1;

        shiftSolution(x, y, a, b, (minx - x) / b);
        if (x < minx)
            shiftSolution(x, y, a, b, sign_b);
        if (x > maxx)
            return make_pair(0, minx);
        T lx1 = x;

        shiftSolution(x, y, a, b, (maxx - x) / b);
        if (x > maxx)
            shiftSolution(x, y, a, b, -sign_b);
        T rx1 = x;

        shiftSolution(x, y, a, b, -(miny - y) / a);
        if (y < miny)
            shiftSolution(x, y, a, b, -sign_a);
        if (y > maxy)
            return make_pair(0, minx);
        T lx2 = x;

        shiftSolution(x, y, a, b, -(maxy - y) / a);
        if (y > maxy)
            shiftSolution(x, y, a, b, sign_a);
        T rx2 = x;

        if (lx2 > rx2)
            swap(lx2, rx2);
        T lx = max(lx1, lx2);
        T rx = min(rx1, rx2);

        return make_pair((rx - lx) / abs(b) + 1, lx);
    }

private:
    static void shiftSolution(T& x, T& y, T a, T b, T cnt) {
        x += cnt * b;
        y -= cnt * a;
    }
};