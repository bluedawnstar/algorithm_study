#include <climits>
#include <numeric>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#include "maxIndependentSet.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static int maxWeightedIndependentSet(vector<unsigned long long>& g, const vector<int>& weights) {
    int res = 0;
    int n = (int)g.size();

    for (int i = 0; i < n; i++)
        g[i] |= (1ull << i);

    for (int set = 0; set < (1 << n); set++) {
        bool ok = true;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < i; j++)
                ok &= (set & (1 << i)) == 0 || (set & (1 << j)) == 0 || (g[i] & (1ull << j)) == 0;

        if (ok) {
            int cur = 0;
            for (int i = 0; i < n; i++)
                if ((set & (1 << i)) != 0)
                    cur += weights[i];
            res = max(res, cur);
        }
    }

    return res;
}

void testMaxIndependentSet() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Max Weighted Independent Set -----------------------" << endl;
    {
        for (int step = 0; step < 10; step++) {
            int N = rand() % 16 + 1;

            vector<unsigned long long> G(N);
            vector<int> weights(N);

            for (int i = 0; i < N; i++)
                weights[i] = rand() % 1000;

            for (int i = 0; i < N; i++) {
                for (int j = 0; j < i; j++)
                    if (rand() & 1) {
                        G[i] |= 1ull << j;
                        G[j] |= 1ull << i;
                    }
            }

            int ans = MaxIndependentSet::getMaxWeightedSum(G, weights);
            int gt = maxWeightedIndependentSet(G, weights);
            if (ans != gt) {
                cerr << "Mismatched: gt = " << gt << ", ans = " << ans << endl;
            }
            assert(ans == gt);
        }
    }
    {
        int N = 64;

        vector<unsigned long long> G(N);
        vector<int> weights(N);
        for (int i = 0; i < N; i++)
            weights[i] = rand() % 1000;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < i; j++) {
                if (rand() & 1) {
                    G[i] |= 1ull << j;
                    G[j] |= 1ull << i;
                }
            }
        }

        PROFILE_START(0);
        for (int step = 0; step < 10; step++) {
            int ans = MaxIndependentSet::getMaxWeightedSum(G, weights);
            if (ans < 0)
                cerr << "ERROR!" << endl;
        }
        PROFILE_STOP(0);
    }

    cout << "OK" << endl;
}
