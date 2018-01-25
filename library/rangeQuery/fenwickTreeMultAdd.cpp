using namespace std;

#include "fenwickTreeMultAdd.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <memory.h>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testFenwickTreeMultAdd() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- FenwickTreeMultAdd -----------------------------------------" << endl;

    FenwickTreeMultAdd<int> fenwick(100);

    fenwick.addRange(10, 19, 1);
    fenwick.addRange(30, 39, 2);

    int ans = fenwick.sum(5);
    cout << "fenwick.sum(5) = " << ans << endl;
    assert(ans == 0);

    ans = fenwick.sumRange(3, 24);
    cout << "fenwick.sum(3, 24) = " << ans << endl;
    assert(ans == 10);

    ans = fenwick.sumRange(12, 24);
    cout << "fenwick.sum(12, 24) = " << ans << endl;
    assert(ans == 8);

    ans = fenwick.sumRange(3, 34);
    cout << "fenwick.sum(3, 34) = " << ans << endl;
    assert(ans == 20);

    cout << "OK!" << endl;
}
