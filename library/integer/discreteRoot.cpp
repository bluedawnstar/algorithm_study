#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#include "discreteRoot.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include <random>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

#include "intMod.h"

void testDiscreteRoot() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Discrete Root -------------------------" << endl;
    {
        int T = 1000;
#ifdef _DEBUG
        T = 100;
#endif
        int M = 1000000009;
        for (int i = 0; i < 1000; i++) {
            int x = RandInt32::get() % 65536 + 1;
            int xx = int(1ll * x * x % M);
            auto roots = DiscreteRoot<int>::rootAll(2, xx, M);
            auto ok = (find(roots.begin(), roots.end(), x) != roots.end());
            if (!ok)
                cout << "Can't find answer! : " << x << "^2 = " << xx << endl;
            assert(ok);
        }
    }

    cout << "OK!" << endl;
}
