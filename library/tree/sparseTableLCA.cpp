#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#include "sparseTableLCA.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"

void testSparseTableLCA() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Sparse Table LCA ----------------------------------------" << endl;
    
    vector<int> in(100);
    iota(in.begin(), in.end(), 1);
    in[99] = -1;

    SparseTableLCA spLCA(in);
    spLCA.build();

    assert(spLCA.climb(0, 100) == -1);
    assert(spLCA.climb(50, 50) == -1);
    assert(spLCA.climb(50, 49) == 99);
    assert(spLCA.climb(3, 20) == 23);

    cout << "OK!" << endl;
}
