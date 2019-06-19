#include <vector>

using namespace std;

#include "lagrange.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testLagrange() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Lagrange's Interpolation ------------------------" << endl;
    {
        auto ans = LagrangePolynomial::interpolate(vector<pair<int, int>>{ {0, 2}, { 1,3 }, { 2,12 }, { 5,147 }}, 3);
        cout << "f(3) = " << ans << endl;
        assert(ans == 35);
    }

    cout << "OK!" << endl;
}
