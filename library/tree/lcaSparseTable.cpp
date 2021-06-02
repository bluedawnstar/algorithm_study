#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "lcaSparseTable.h"
#include "lcaSparseTableSimple.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"

void testLcaSparseTable() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- LCA with Sparse Table -----------------------------------" << endl;
    
    vector<int> parent(100), level(100);
    iota(parent.begin(), parent.end(), 1);
    parent[99] = -1;
    for (int i = 0; i < int(level.size()); i++)
        level[i] = int(level.size()) - i - 1;

    LcaSparseTable spLCA(parent, level);
    spLCA.build();

    assert(spLCA.climb(0, 100) == -1);
    assert(spLCA.climb(50, 50) == -1);
    assert(spLCA.climb(50, 49) == 99);
    assert(spLCA.climb(3, 20) == 23);

    cout << "OK!" << endl;
}
