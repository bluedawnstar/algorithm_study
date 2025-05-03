#include <functional>

using namespace std;

#include "mergeableHeap.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testMergeableHeap() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Mergeable Heap ------------------------" << endl;

    MergeableHeap<int> heap;
    assert(heap.empty());
    heap.push(3);
    assert(heap.top() == 3);
    heap.push(1);
    assert(heap.top() == 1);
    heap.push(2);
    assert(heap.top() == 1);

    assert(heap.pop() == 1);
    assert(heap.pop() == 2);
    assert(heap.pop() == 3);
    assert(heap.empty());

    cout << "OK!" << endl;
}
