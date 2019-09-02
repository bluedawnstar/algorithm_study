using namespace std;

#include "fenwickTreeMultAddPersistent.h"

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

void testPersistentFenwickTreeMultAdd() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- PersistentFenwickTreeMultAdd --------------------------------------" << endl;

    PersistentFenwickTreeMultAdd<int> fenwick(100);

    int t1 = fenwick.addRange(10, 19, 1);
    int t2 = fenwick.addRange(30, 39, 2);

    int ans = fenwick.sum(t2, 5);
    cout << "fenwick.sum(5) = " << ans << endl;
    assert(ans == 0);

    ans = fenwick.sumRange(t2, 3, 24);
    cout << "fenwick.sum(3, 24) = " << ans << endl;
    assert(ans == 10);

    ans = fenwick.sumRange(t2, 12, 24);
    cout << "fenwick.sum(12, 24) = " << ans << endl;
    assert(ans == 8);

    ans = fenwick.sumRange(t2, 3, 34);
    cout << "fenwick.sum(3, 34) = " << ans << endl;
    assert(ans == 20);

    cout << "OK!" << endl;
}
