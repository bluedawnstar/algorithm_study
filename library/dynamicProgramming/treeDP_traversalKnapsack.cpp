#include <array>
#include <vector>
#include <algorithm>

using namespace std;

#include "treeDP_traversalKnapsack.h"


/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

void testTraversalKnapsackOnTree() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Traversal Knapsack on a Tree -----------------------------------" << endl;
    {
        TreeTraversalKnapsack<int> solver(4);
        solver.add(0, 2);
        solver.add(0, 1);
        solver.add(2, 3);

        vector<int> cost{ 1, 2, 1, 2 };
        vector<int> gain{ 1, 3, 2, 3 };

        int ans = solver.knapsack(cost, gain, 5);
        if (ans != 5) {
            cout << "Mismatched at " << __LINE__ << " : " << ans << ", 5" << endl;
        }
        assert(ans == 5);
    }

    cout << "OK!" << endl;
}
