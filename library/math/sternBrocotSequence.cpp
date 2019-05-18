#include <cmath>
#include <vector>
#include <limits>
#include <numeric>
#include <queue>
#include <algorithm>

using namespace std;

#include "sternBrocotSequence.h"

// 
// <Related Links>
// http://www.sciencedirect.com/science/article/pii/S0166218X09000894
// https://en.wikipedia.org/wiki/Continued_fraction
// https://en.wikipedia.org/wiki/Farey_sequence
// https://en.wikipedia.org/wiki/Mediant_(mathematics)
// https://en.wikipedia.org/wiki/Stern%E2%80%93Brocot_tree
// 
// <HackerRank's Question & Solutions>
// https://www.hackerrank.com/contests/w29/challenges/minimal-distance-to-pi/submissions/code/1300697342
// https://www.hackerrank.com/contests/w29/challenges/minimal-distance-to-pi/submissions/code/1300700017
// https://codepair.hackerrank.com/paper/OqDsIuXQ?b=eyJyb2xlIjoiY2FuZGlkYXRlIiwibmFtZSI6ImJsdWVkYXduc3RhciIsImVtYWlsIjoieW91bmdtYW4ucm9AZ21haWwuY29tIn0%3D
// https://codepair.hackerrank.com/paper/PLC7kLzG?b=eyJyb2xlIjoiY2FuZGlkYXRlIiwibmFtZSI6ImJsdWVkYXduc3RhciIsImVtYWlsIjoieW91bmdtYW4ucm9AZ21haWwuY29tIn0%3D
// 
// <PI>
// http://qin.laya.com/tech_projects_approxpi.html
// https://oeis.org/A001203
// 

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

void testSternBrocotSequence() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Stern Brocot Sequence -------------------------" << endl;
    // Stern-Brocot sequence
    {
        vector<pair<int, int>> F5 = {
            { 0, 1 }, { 1, 5 }, { 1, 4 }, { 1, 3 }, { 2, 5 }, { 1, 2 }, { 3, 5 }, { 2, 3 }, { 3, 4 }, { 4, 5 },
            { 1, 1 }, { 5, 4 }, { 4, 3 }, { 3, 2 }, { 5, 3 }, { 2, 1 }, { 5, 2 }, { 3, 1 }, { 4, 1 }, { 5, 1 },
        };

        SternBrocotRank rank(5);

        for (int i = 0; i < int(F5.size()); i++) {
            auto r = rank.rank(F5[i].first, F5[i].second);
            cout << "rank(" << F5[i].first << " / " << F5[i].second << ") = " << r << endl;
            assert(r == i);
        }

        for (int n = 4; n <= 100; n++) {
            SternBrocotRank rank(n);
            for (int i = 0; i <= n; i++) {
                auto ans1 = SternBrocotSequence::rank(i, n, n);
                auto ans2 = rank.rank(i, n);
                if (ans1 != ans2)
                    cout << i << ":" << ans1 << ", " << ans2 << endl;
                assert(ans1 == ans2);
            }
        }

        for (int i = 0; i < int(F5.size()); i++) {
            assert(SternBrocotSequence::kthNaive(5, i + 1) == F5[i]);
            assert(SternBrocotSequence::kthFast(5, i + 1) == F5[i]);
            assert(SternBrocotSequence::kthFast(rank, 5, i + 1) == F5[i]);
        }
    }

    cout << "OK!" << endl;
}
