#include <cassert>
#include <limits>
#include <functional>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#include "quadTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testQuadTree() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "--- Quad-Tree ----------------------------------" << endl;
    {
        QuadTree<int> tr;

        tr.insert(0, 0);
        tr.insert(1, 0);
        tr.insert(2, 0);
        tr.insert(3, 0);

        assert(tr.count(0, 0, 3, 0) == 4);
    }

    cout << "OK!" << endl;
}
