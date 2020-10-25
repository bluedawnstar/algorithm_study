#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

#include "treeDP_independentEdgeSetOnTree.h"
#include "treeDP_independentEdgeSetOnTreeMod2.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testIndependentEdgeSetOnTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Independent Edge Set on a Tree -----------------------------------" << endl;
    {
        vector<pair<vector<tuple<int, int, int>>, int>> graphs{
            {
                {
                    { 1, 2, 6 },
                    { 0, 1, 1 }
                },
                7
            },
            {
                {
                    { 0, 1, 1 },
                    { 0, 2, 2 },
                    { 1, 3, 4 },
                    { 1, 4, 8 },
                    { 2, 5, 16 },
                    { 2, 6, 32 }
                },
                3
            },
            {
                {
                    { 0, 1, 1 },
                    { 1, 2, 2 },
                    { 2, 3, 4 }
                },
                2
            },
            {
                {
                    { 0, 1, 1 },
                    { 1, 2, 2 },
                    { 2, 3, 4 },
                    { 3, 4, 8 }
                },
                9
            }
        };

        for (auto& it : graphs) {
            int N = int(it.first.size()) + 1;

            IndependentEdgeSetOnTree<> G1(N);
            IndependentEdgeSetOnTreeMod2 G2(N);

            for (auto& it2 : it.first) {
                G1.add(get<0>(it2), get<1>(it2));
                G2.add(get<0>(it2), get<1>(it2));
            }

            auto cnt1 = G1.countIndependentEdgeSetByEachEdgeUsed();
            auto cnt2 = G2.countIndependentEdgeSetByEachEdgeUsed();

            int ans1 = 0;
            int ans2 = 0;
            for (int i = 0; i < int(it.first.size()); i++) {
                if (cnt1[i] & 1)
                    ans1 ^= get<2>(it.first[i]);
                if (cnt2[i] & 1)
                    ans2 ^= get<2>(it.first[i]);
            }

            if (ans1 != ans2)
                cout << "Mismatched at " << __LINE__ << " : " << ans1 << ", " << ans2 << endl;
            assert(ans1 == ans2);
        }
    }

    cout << "OK!" << endl;
}
