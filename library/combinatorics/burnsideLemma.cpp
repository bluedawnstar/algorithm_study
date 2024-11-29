#include <numeric>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#include "burnsideLemma_coloringNecklace.h"
#include "burnsideLemma_coloringNecklaceMod.h"
#include "burnsideLemma_coloringAllNecklacesMod.h"
#include "burnsideLemma_coloringNecklaceNonUniformFixedPosition.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

void testBurnsideLemma() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Burnside's Lemma -----------------------------------" << endl;
    cout << "** Coloring Necklaces" << endl;
    {
        // in = { (the number of beads, the number of colors), ... }
        vector<pair<int, int>> in{
            { 1, 1 },
            { 1, 2 },
            { 2, 2 },
            { 1, 5 },
            { 5, 2 },
            { 6, 2 },
            { 2, 6 }
        };
        vector<int> gt{
            1,
            2,
            3,
            5,
            8,
            13,
            21
        };

        for (int i = 0; i < int(in.size()); i++) {
            int N = in[i].first;
            int C = in[i].second;
            int ans1 = ColoringNecklace::count(N, C, true);
            int ans2 = ColoringNecklaceMod<>::count(N, C, true);

            if (ans1 != gt[i])
                cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << gt[i] << endl;
            assert(ans1 == gt[i]);

            if (ans2 != gt[i])
                cout << "Mismatched at " << __LINE__ << " : " << ans2 << ", " << gt[i] << endl;
            assert(ans2 == gt[i]);
        }
    }
    cout << "** Coloring All Necklaces" << endl;
    {
        const int MOD = 1'000'000'007;

        // in = { (maximum number of beads, the number of colors), ... }
        vector<pair<int, int>> in{
            { 8, 8 },
            { 100, 100 },
        };
        vector<int> gt{ 1237325, 639265925 };

        for (int i = 0; i < int(in.size()); i++) {
            int N = in[i].first;
            int C = in[i].second;

            ColoringAllNecklacesMod<MOD> solver(N, C);

            int ans1 = 1;    // N=0
            int ans2 = 1;    // N=0
            for (int i = 1; i <= N; i++) {
                ans1 += solver.count(i, true);
                ans2 += solver.countFast(i, true);
                if (ans1 >= MOD)
                    ans1 -= MOD;
                if (ans2 >= MOD)
                    ans2 -= MOD;
            }

            if (ans1 != gt[i])
                cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << gt[i] << endl;
            assert(ans1 == gt[i]);

            if (ans2 != gt[i])
                cout << "Mismatched at " << __LINE__ << " : " << ans2 << ", " << gt[i] << endl;
            assert(ans2 == gt[i]);
        }
    }
    cout << "** Coloring a necklace with fixed positions on a circle" << endl;
    {
        const int MOD = 100'000'007;
        const int MaxAngle = 360'000;
        // in = { (colorCount, { angle0, angle1, ... }), ... }
        vector<pair<int, vector<int>>> in{
            { 2, { 0, 90'000, 180'000, 270'000 } },
            { 100, { 0, 45'000, 90'000, 180'000, 270'000 } }
        };
        vector<int> gt{ 6, 99999307 };

        for (int i = 0; i < int(in.size()); i++) {
            int C = in[i].first;
            auto& angles = in[i].second;

            int ans = ColoringNecklaceNonUniformFixedPosition<MOD>::count(angles, C, MaxAngle);

            if (ans != gt[i])
                cout << "Mismatched at " << __LINE__ << " : " << ans << ", " << gt[i] << endl;
            assert(ans == gt[i]);
        }        
    }

    cout << "OK!" << endl;
}
