#pragma once

struct DiophantineEquation2 {
    // a*i - b*j = c (diophantine equation), (a > 0, b > 0, c > 0, i > 0, j >= 0)
    // find (i, j) with absolute minimum value of i and j
    template <typename T>
    static bool solveDiophantine(T a, T b, T c, T& i, T& j) {
        T g = extGcd(a, b, i, j);
        if (c % g)
            return false;

        i *= c / g;
        j *= c / g;

        T at = a / g;
        T bt = b / g;

        if (i < 0 || j > 0) {
            // (i < 0 && j >= 0) or (i <= 0 && j > 0)
            T k = min((abs(i) + bt - 1) / bt, (abs(j) + at - 1) / at);
            i += bt * k;
            j -= at * k;
        } else {
            // (i >= 0 && j <= 0)
            T k = min(abs(i) / bt, abs(j) / at);
            i -= bt * k;
            j += at * k;
        }
        if (i == 0) {
            i += bt;
            j -= at;
        }
        if (j < 0)
            j = -j;

        return true;
    }

private:
    // a * x + b * y = gcd(a, b)
    template <typename T>
    static T extGcd(T a, T b, T& x, T& y) {
        if (b == 0) {
            x = 1;
            y = 0;
            return a;
        }

        T x1, y1;
        T g = extGcd(b, a % b, x1, y1);

        x = y1;
        y = x1 - (a / b) * y1;
        return g;
    }
};
