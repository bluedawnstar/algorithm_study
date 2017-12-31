using namespace std;

#include "rootFindingLaguerre.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static ostream& operator <<(ostream& os, const Complex<double>& c) {
    os << "(" << c.first << "," << c.second <<")";
    return os;
}

void testRootFindingLaguerre() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Polynomial Root Finding - Laguerre -------------------" << endl;
    vector<Complex<double>> poly;
    poly.emplace_back(140, 0);
    poly.emplace_back(-13, 0);
    poly.emplace_back(-8, 0);
    poly.emplace_back(1, 0);

    auto ans = LaguerreMethod::findAllRoots(poly);
    sort(ans.begin(), ans.end());
    assert(fabs(ans[0].first - -4.0) < EPSILON && fabs(ans[0].second) < EPSILON);
    assert(fabs(ans[1].first - 5.0) < EPSILON && fabs(ans[1].second) < EPSILON);
    assert(fabs(ans[2].first - 7.0) < EPSILON && fabs(ans[2].second) < EPSILON);
    cout << ans << endl;

    cout << "OK!" << endl;
}
