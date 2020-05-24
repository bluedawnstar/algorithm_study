#include <cmath>
#include <functional>
#include <bitset>
#include <vector>
#include <algorithm>

using namespace std;

#include "arrayGeneratorWithDistinctSubarraySums_GolombRuler.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testArrayGeneratorWithDistinctSubarraySums() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Array Generator With Distinct Subarray Sums ------" << endl;
    {
        for (int n = 3; n <= 20; n++) {
            auto v = ArrayGeneratorWithDistinctSubarraySums::build(n);
            // check #1
            {
                bool ok = DistinctSubarrySumChecker<1000>::check(v, n);
                if (!ok) {
                    cout << "invalid sequence : N = " << n << ", seq = " << v << endl;
                }
                assert(ok);
            }
            // check #2 : incremental checker
            {
                DistinctSubarrySumChecker<1000> checker;
                checker.init(n);
                for (int i = 0; i < n; i++) {
                    if (!checker.checkAndAdd(v[i])) {
                        cout << "invalid sequence : N = " << n << ", seq = " << v << endl;
                        break;
                    }
                }
            }
        }
    }

    cout << "OK!" << endl;
}
