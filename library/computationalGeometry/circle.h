#pragma once

#include <math.h>

// https://codingcompetitions.withgoogle.com/codejam/round/00000000008778ec/0000000000b158f7

struct Circle {
    // y = INT{sqrt(R^2 - x^2) + 0.5}
    // O(1)
    static int countIntegerPointsOnCirclePerimeter(int R) {
        const double SQRT2 = sqrt(2);

        int res = 0;

        /*
            x = r * cos(PI/4) = r / sqrt(2)

            if sqrt(r^2 - x^2) + 0.5 >= x + 1 then increase x

             => sqrt(r^2 - x^2) >= x + 0.5
            <=> r^2 - x^2 >= (x + 0.5)^2
            <=> r^2 - x^2 >= x^2 + x + 0.25

            <=> r^2 - x^2 > x^2 + x
        */
        int cx = int(R / SQRT2);
        if (1ll * R * R - 1ll * cx * cx > 1ll * cx * cx + cx)
            cx++;

        res += (cx - 1) * 8;
        /*
            if sqrt(r^2 - x^2) + 0.5 >= x then ans += 4

             => sqrt(r^2 - x^2) >= x - 0.5
            <=> r^2 - x^2 >= (x - 0.5)^2
            <=> r^2 - x^2 >= x^2 - x + 0.25

            <=> r^2 - x^2 > x^2 - x
        */
        if (1ll * R * R - 1ll * cx * cx > 1ll * cx * cx - cx)
            res += 4;

        // 4 points on X/Y-axis
        res += 4;

        return res;
    }

    // INT{sqrt(x^2 + y^2) + 0.5} <= R
    // O(R)
    static long long countIntegerPointsInCircle(int R) {
        long long area = R * 4 + 1;
        for (int x = 1, y = R; x <= R; x++) {
            /*
                if sqrt(x^2 + y^2) + 0.5 >= r + 1 then decrease y

                => sqrt(x^2 + y^2) >= r + 0.5
                <=> x^2 + y^2 >= (r + 0.5)^2
                <=> x^2 + y^2 >= r^2 + r + 0.25

                <=> x^2 + y^2 > r^2 + r
            */
            while (1ll * x * x + 1ll * y * y > 1ll * R * R + R)
                y--;
            area += y * 4;
        }
        return area;
    }
};
