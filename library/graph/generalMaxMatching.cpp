#include <queue>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

#include "generalMaxMatching.h"
#include "generalMaxMatchingRandomized.h"
#include "generalMaxMatchingAlgebraic.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <unordered_set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testGeneralMaxMatching() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Blossom Algorithm for General Matching -------------" << endl;
    {
        const int N = 4;
        vector<pair<int, int>> edges{
            { 0, 1 },
            { 1, 2 },
            { 2, 3 },
            { 0, 3 }
        };

        GeneralMaxMatching graph(N);
        RandomizedGeneralMaxMatching randGraph(N);
        AlgebraicGeneralMaxMatching algGraph(N);
        for (auto& it : edges) {
            graph.addEdge(it.first, it.second);
            randGraph.addEdge(it.first, it.second);
            algGraph.addEdge(it.first, it.second);
        }

        int ans = graph.calcMaxMatching();
        int ans2 = randGraph.calcMaxMatching();
        int ans3 = algGraph.calcMaxMatching();
        if (ans != ans2 || ans != ans3)
            cout << "Mismatched  at " << __LINE__ << " : " << ans << ", " << ans2 << ", " << ans3 << endl;
        assert(ans == 2 && ans2 == 2 && ans3 == 2);
    }
    {
        const int N = 10;
        vector<pair<int, int>> edges{
            { 0, 1 },
            { 1, 2 },
            { 2, 3 },
            { 3, 5 },
            { 1, 9 },
            { 9, 8 },
            { 8, 4 },
            { 4, 5 },
            { 8, 7 },
            { 4, 6 },
            { 5, 6 },
            { 7, 6 }
        };

        GeneralMaxMatching graph(N);
        RandomizedGeneralMaxMatching randGraph(N);
        AlgebraicGeneralMaxMatching algGraph(N);
        for (auto& it : edges) {
            graph.addEdge(it.first, it.second);
            randGraph.addEdge(it.first, it.second);
            algGraph.addEdge(it.first, it.second);
        }

        int ans = graph.calcMaxMatching();
        int ans2 = randGraph.calcMaxMatching();
        int ans3 = algGraph.calcMaxMatching();
        if (ans != ans2 || ans != ans3)
            cout << "Mismatched  at " << __LINE__ << " : " << ans << ", " << ans2 << ", " << ans3 << endl;
        assert(ans == 5 && ans2 == 5 && ans3 == 5);
    }

    cout << "OK" << endl;
}
