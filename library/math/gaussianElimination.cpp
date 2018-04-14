#include <cmath>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "gaussianElimination.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testGaussianElimination() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Gaussian Elimination -----------------------" << endl;
    auto ans = gauss(Matrix<double>(vector<vector<double>>{ { 4, 2, -1 }, { 2, 4, 3 }, { -1, 3, 5 } }), vector<double>{ 5, 19, 20 });
    cout << ans << endl;
    assert(int(ans[0] + 0.5) == 1);
    assert(int(ans[1] + 0.5) == 2);
    assert(int(ans[2] + 0.5) == 3);

    cout << "OK!" << endl;
}
