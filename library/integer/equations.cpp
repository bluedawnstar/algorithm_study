#include <cassert>
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
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Equations -------------------------" << endl;
    cout << "* linear modular equation" << endl;
    {
        auto ans = solveModularEq(14, 30, 100);
        assert(ans.size() == 2 && ans[0] == 45 && ans[1] == 95);
    }
    cout << "* linear diophantine equation" << endl;
    {
        int x, y;
        bool ok = solveDiophantineEq(7, 2, 5, x, y);
        assert(ok);
        assert(x == 5 && y == -15);
    }
    cout << "OK!" << endl;
}
