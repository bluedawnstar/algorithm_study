#include <vector>
#include <algorithm>

using namespace std;

#include "packedArray.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testPackedArray() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Packed Array ----------" << endl;

    int N = 1000000;
    int BIT = 20;

    PackedArray<unsigned int> arr(N * BIT);

    vector<int> v(N);
    for (int i = 0; i < N; i++) {
        v[i] = rand() * rand() % N;
    }

    for (int i = 0; i < N; i++)
        arr.set(i * BIT, BIT, v[i]);

    for (int i = 0; i < N; i++) {
        int x = (int)arr.get(i * BIT, BIT);
        if (x != v[i]) {
            cerr << "gt = " << v[i] << ", x = " << x << endl;
        }
        assert(x == v[i]);
    }

    cout << "OK!" << endl;
}
