#include <stack>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "reservoirSampling.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testReservoirSampling() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Reservoir Sampling ------------------------" << endl;
    {
        int T = 1000, K = 10;

        vector<int> in(T);
        iota(in.begin(), in.end(), 0);

        auto sample = ReservoirSampling<int>::select(in, K);
        sort(sample.begin(), sample.end());
        sample.erase(unique(sample.begin(), sample.end()), sample.end());

        cout << sample << endl;

        assert(sample.size() == K);
    }

    cout << "OK!" << endl;
}
