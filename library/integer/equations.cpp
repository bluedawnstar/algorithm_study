#include <cassert>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "equations.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testEquations() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Equations -------------------------" << endl;
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
    cout << "OK!" << endl;
}
