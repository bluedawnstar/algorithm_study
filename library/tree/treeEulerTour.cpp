#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

#include "treeEulerTour.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testEulerTour() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Euler Tour Tree -----------------------------" << endl;

    int N = 4, LOGN = 4;
    EulerTourTree tree(N, LOGN);

    tree.addEdge(0, 1);
    tree.addEdge(0, 2);
    tree.addEdge(2, 3);

    tree.build(0);

    // time: 0 1 2 3 4 5 6 7
    // node: 0 1 1 2 3 3 2 0
    vector<pair<int, int>> gtVT = vector<pair<int, int>>{ { 0, 7 }, { 1, 2 }, { 3, 6 }, { 4, 5 } };
    vector<int> gtT2N = { 0, 1, 1, 2, 3, 3, 2, 0 };
    assert(tree.mVisTime == gtVT);
    assert(tree.mTime2Node == gtT2N);

    cout << "OK!" << endl;
}
