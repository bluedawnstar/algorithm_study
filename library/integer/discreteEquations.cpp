#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "discreteEquations.h"
#include "discreteLog.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include <random>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

#include "intMod.h"

void testDiscreteEquations() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Modular Equations -------------------------" << endl;
    {
        auto ans = LinearCongruence<int>::solve(14, 30, 100);
        assert(ans.size() == 2 && ans[0] == 45 && ans[1] == 95);
    }
    cout << "OK!" << endl;
}
