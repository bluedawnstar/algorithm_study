#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "nextPermutation.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include <numeric>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

#include "../integer/bit.h"

void testNextPermutation() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Next Permutation --------------------------------------" << endl;
    {
        int N = 8;
        vector<int> p1(N), p2(N);
        iota(p1.begin(), p1.end(), 0);
        iota(p2.begin(), p2.end(), 0);

        bool ok1, ok2;
        do {
            assert(p1 == p2);
            ok1 = next_permutation(p1.begin(), p1.end());
            ok2 = nextPermutation(p2);
            assert(ok1 == ok2);
        } while (ok1);
    }

    cout << "OK!" << endl;
}
