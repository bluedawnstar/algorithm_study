#include <cmath>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "simpsonIntegration.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

#ifndef M_PI
#define M_PI        3.14159265358979323846   // pi
#endif

void testSimpsonIntegration() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Simpson Integration -----------------------" << endl;
    cout << SimpsonIntegration::integrate([](double x) { return sin(x); }, 0, M_PI) << endl;
    cout << SimpsonIntegration::integrate([](double x) { return sin(x); }, -M_PI, 0) << endl;
    cout << SimpsonIntegration::integrate([](double x) { return 2 * sqrt(1.0 - x * x); }, -1, 1) << endl;

    cout << "OK!" << endl;
}
