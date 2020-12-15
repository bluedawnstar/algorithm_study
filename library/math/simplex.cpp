#include <cmath>
#include <numeric>
#include <vector>

using namespace std;

#include "simplex.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testSimplex() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Linear Programming (Simplex) ------------------------------" << endl;
    {
        vector<vector<double>> A{
            {  1, -1 },
            { -1,  1 },
            { -1, -2 }
        };
        vector<double> b{ 1, 1, -4 };
        vector<double> c{ -1, -1 };

        vector<double> x;

        auto ans = Simplex(A, b, c).solve(x);
        cout << "max c^T x = " << ans << endl;
        cout << "x = " << x << endl;
    }

    cout << "OK!" << endl;
}
