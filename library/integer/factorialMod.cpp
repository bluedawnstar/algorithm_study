#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

#include "factorialMod.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testFactorialMod() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Modular Factorial -------------------------" << endl;
    {
        auto ans1 = factorialModNaive(25, 29);
        auto ans2 = factorialModWilson(25, 29);
        auto ans3 = factorialMod(25, 29);
        assert(ans1 == ans2);
        assert(ans1 == ans3);
        assert(ans1 == 5);
    }

    cout << "OK!" << endl;
}
