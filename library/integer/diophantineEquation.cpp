#include <cassert>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "diophantineEquation.h"
#include "diophantineEquation2.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

void testDiophantineEquation() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Diophantine Equations -------------------------" << endl;
    // linear diophantine equation
    {
        int a = 7, b = 2, c = 5;
        int x, y, g;
        bool ok = DiophantineEquation<int>::findAny(a, b, c, x, y, g);
        assert(ok);
        assert(x == 5 && y == -15);
    }
    {
        int a = 7, b = 2, c = 5;
        int minx = 10, maxx = 100;
        int miny = 10, maxy = 100;
        auto ans = DiophantineEquation<int>::findAll(a, b, c, minx, maxx, miny, maxy);
        for (int k = -1; k <= ans.first; k++) {
            int xx = ans.second + k * b;
            int yy = (c - a * xx) / b;
            if (k < 0 || k >= ans.first)
                assert(!(minx <= xx && xx <= maxx && miny <= yy && yy <= maxy));
            else
                assert(minx <= xx && xx <= maxx && miny <= yy && yy <= maxy);
        }
    }
    {
        int T = 1000;
        int N = 10000;
        while (T-- > 0) {
            long long a = RandInt32::get() % N + 1;
            long long b = RandInt32::get() % N + 1;
            long long c = RandInt32::get() % N + 1;
            long long i, j;
            if (DiophantineEquation2::solveDiophantine(a, b, c, i, j)) {
                int y = a * i - b * j;
                if (y != c) {
                    cout << "Mismatched at " << i << " : " << a << " * i - " << b << " * j = " << c << endl;
                    cout << "          " << a << " * " << i << " - " << b << " * " << j << " = " << y << endl;
                }
                assert(a * i - b * j == c);
            } else {
                cout << "Can't solve this equation : " << a << " * i - " << b << " * j = " << c << endl;
            }
        }
    }
    cout << "OK!" << endl;
}
