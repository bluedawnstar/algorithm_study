#include <cmath>
#include <cctype>
#include <functional>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

#include "transformOperationByAddOrMult.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testTransformOperationByAddOrMult() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Transform Operation by Add or Mult -------------------" << endl;
    {
        vector<int> X{ 3, 5, 7 };
        vector<int> Y{ 6, 5, 10 };

        TransformOperationByAddOrMult solver;
        int ans = solver.solve(X, Y);
        int gt = 1;
        if (ans != gt) {
            cout << "Mismatched : " << ans << ", " << gt << endl;
        }
        assert(ans == gt);
    }
    {
        vector<int> X{ 8, 6, 3 };
        vector<int> Y{ 9, 7, 8 };

        TransformOperationByAddOrMult solver;
        int ans = solver.solve(X, Y);
        int gt = 2;
        if (ans != gt) {
            cout << "Mismatched : " << ans << ", " << gt << endl;
        }
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
