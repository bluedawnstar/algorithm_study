#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "labeledGraphCounter.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

const int MOD = 1'000'000'007;

void testLabeledGraphCounter() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- The number of Labeled Graph -------------------------------" << endl;
    {
        vector<int> gt{
            1,
            0,
            1,
            4,
            41,
            768,
            27449,
            1887284,
            252522481,
            int(66376424160ll % MOD),
            int(34509011894545ll % MOD),
            int(35645504882731588ll % MOD)
        };
        LabeledGraphCounter<MOD> counter(20);
        for (int i = 0; i < int(gt.size()); i++) {
            int ans = counter.count(i);
            if (ans != gt[i])
                cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt << endl;
            assert(ans == gt[i]);
        }
    }
    cout << "OK!" << endl;
}
