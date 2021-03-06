#include <tuple>
#include <array>
#include <vector>
#include <algorithm>

using namespace std;

#include "treeDP_TreePathDecompositionCounter.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"


void testTreePathDecompositionCounter() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- The number of path decomposition on a tree ---------------------" << endl;
    {
        const int N = 5;
        const vector<pair<int, int>> edges{
            { 2, 0 },
            { 3, 0 },
            { 0, 4 },
            { 1, 4 }
        };
        TreePathDecompositionCounterMod<> tree(N);
        for (auto& it : edges)
            tree.addEdge(it.first, it.second);
        tree.build();
        assert(tree.countExceptPath(2, 2) == 8);
        assert(tree.countExceptPath(1, 4) == 4);
    }
    {
        const int N = 5;
        const vector<pair<int, int>> edges{
            { 2, 0 },
            { 3, 0 },
            { 0, 4 },
            { 0, 1 }
        };
        TreePathDecompositionCounterMod<> tree(N);
        for (auto& it : edges)
            tree.addEdge(it.first, it.second);
        tree.build();
        assert(tree.countExceptPath(2, 2) == 7);
        assert(tree.countExceptPath(1, 4) == 1);
        assert(tree.countExceptPath(2, 3) == 1);
        assert(tree.countExceptPath(3, 1) == 1);
        assert(tree.countExceptPath(1, 1) == 7);
    }

    cout << "OK!" << endl;
}
