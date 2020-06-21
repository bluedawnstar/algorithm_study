#include <vector>
#include <algorithm>
#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

using namespace std;

#include "treeUniqueValueCounterInDistanceOfSubtree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

// <Related Problems>
// https://www.codechef.com/problems/DIFVAL

void testUniqueValueCounterInDistanceOfSubtree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Unique Value Counter In Distance Of Subtree -----------------" << endl;
    {
        const int N = 11;

        UniqueValueCounterInDistanceOfSubtree tree(N);

        tree.addEdge(0, 1);
        tree.addEdge(1, 2);
        tree.addEdge(1, 3);
        tree.addEdge(1, 4);
        tree.addEdge(3, 5);
        tree.addEdge(3, 6);
        tree.addEdge(0, 7);
        tree.addEdge(7, 8);
        tree.addEdge(7, 9);
        tree.addEdge(9, 10);

        vector<int> A{ 5, 3, 2, 3, 1, 1, 2, 4, 1, 2, 3 };
        tree.build(0, A);

        // { ((node, depth), gt), ... }
        vector<pair<pair<int, int>, int>> qry{
            { { 1, 1 }, 3 },
            { { 1, 2 }, 3 },
            { { 0, 0 }, 1 },
            { { 0, 1 }, 3 },
            { { 0, 2 }, 5 },
            { { 0, 3 }, 5 },
            { { 7, 1 }, 3 },
            { { 7, 2 }, 4 },
            { { 8, 0 }, 1 },
        };

        for (int i = 0; i < int(qry.size()); i++) {
            int ans = tree.query(qry[i].first.first, qry[i].first.second);
            int gt = qry[i].second;
            if (ans != gt)
                cout << "Mismatched : " << ans << ", " << gt << endl;
            assert(ans == gt);
        }
    }
    cout << "OK!" << endl;
}
