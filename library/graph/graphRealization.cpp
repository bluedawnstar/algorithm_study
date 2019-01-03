#include <climits>
#include <numeric>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "graphRealization.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testGraphRealization() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Graph Realization ----------------" << endl;
    {
        // possible
        vector<int> d{ 1, 2, 3, 4, 5, 5, 6, 7, 8, 9 };
        int ans1 = GraphRealization::possibleSlow(d);
        int ans2 = GraphRealization::possible(d);
        int ans3 = GraphRealization::possibleFast(d);
        assert(ans1 == ans2);
        assert(ans1 == ans3);
    }
    {
        // impossible
        vector<int> d{ 1, 2, 3, 4, 4, 6, 6, 7, 8, 9 };
        int ans1 = GraphRealization::possibleSlow(d);
        int ans2 = GraphRealization::possible(d);
        int ans3 = GraphRealization::possibleFast(d);
        assert(ans1 == ans2);
        assert(ans1 == ans3);
    }
    {
        const int MAXD = 1000;
        int N = 1000;
        int E = N * 10;
        int T = 10000;
#ifdef _DEBUG
        N = 1000;
        T = 100;
#endif
        for (int t = 0; t < T; t++) {
            vector<int> d(N);
            for (int i = 0; i < E; i++) {
                int u = i % N;
                int v;
                do {
                    v = RandInt32::get() % N;
                } while (u == v);
                d[u]++;
                d[v]++;
            }
            if (d[0] & 1) {
                // make it impossible
                if (d[0] > 2)
                    d[0]--;
                d[1]++;
            }

            bool ans1 = GraphRealization::possibleSlow(d);
            bool ans2 = GraphRealization::possible(d);
            bool ans3 = GraphRealization::possibleFast(d);
            if (ans1 != ans2)
                cout << "Mismatched " << ans1 << ", " << ans2 << endl;
            if (ans1 != ans3)
                cout << "Mismatched " << ans1 << ", " << ans3 << endl;
            assert(ans1 == ans2);
            assert(ans1 == ans3);
        }
    }
    {
        int N = 100000;
#ifdef _DEBUG
        N = 1000;
#endif
        int E = N * 100;

        vector<int> d(N);
        // make a valid degree sequence
        for (int i = 0; i < E; i++) {
            int u = i % N;
            int v;
            do {
                v = RandInt32::get() % N;
            } while (u == v);
            d[u]++;
            d[v]++;
        }

        PROFILE_START(0);
        bool ans1 = GraphRealization::possibleSlow(d);
        PROFILE_STOP(0);

        PROFILE_START(1);
        bool ans2 = GraphRealization::possible(d);
        PROFILE_STOP(1);

        PROFILE_START(2);
        bool ans3 = GraphRealization::possibleFast(d);
        PROFILE_STOP(2);

        if (ans1 != ans2)
            cout << "Mismatched " << ans1 << ", " << ans2 << endl;
        if (ans1 != ans3)
            cout << "Mismatched " << ans1 << ", " << ans3 << endl;
        assert(ans1 == ans2);
        assert(ans1 == ans3);
    }
    {
        int N = 100000;
#ifdef _DEBUG
        N = 1000;
#endif

        // circular graph
        vector<int> d(N, 2);

        PROFILE_START(0);
        bool ans1 = GraphRealization::possibleSlow(d);
        PROFILE_STOP(0);

        PROFILE_START(1);
        bool ans2 = GraphRealization::possible(d);
        PROFILE_STOP(1);

        PROFILE_START(2);
        bool ans3 = GraphRealization::possibleFast(d);
        PROFILE_STOP(2);

        if (ans1 != ans2)
            cout << "Mismatched " << ans1 << ", " << ans2 << endl;
        if (ans1 != ans3)
            cout << "Mismatched " << ans1 << ", " << ans3 << endl;
        assert(ans1 == ans2);
        assert(ans1 == ans3);
    }

    cout << "OK" << endl;
}
