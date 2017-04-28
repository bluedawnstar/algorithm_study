using namespace std;

#include "treeFenwickTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <memory.h>
#include <queue>
#include <stack>
#include <algorithm>
#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"

void testFenwickTree() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- FenwickTree -----------------------------------------" << endl;

    int freq[] = { 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9 };
    int n = sizeof(freq) / sizeof(freq[0]);

    FenwickTree<int> fenwick(n);

    fenwick.init(freq, n);

    int ans = fenwick.sum(5);
    cout << "fenwick.sum(5) = " << ans << endl;
    assert(ans == 12);

    fenwick.add(3, 6);
    cout << "after fenwick.add(3, 6)" << endl;

    ans = fenwick.sum(5);
    cout << "fenwick.sum(5) = " << ans << endl;
    assert(ans == 18);

    ans = fenwick.sumRange(1, 5);
    cout << "fenwick.rangeSum(1, 5) = " << ans << endl;
    assert(ans == 16);

    cout << "OK!" << endl;
}
