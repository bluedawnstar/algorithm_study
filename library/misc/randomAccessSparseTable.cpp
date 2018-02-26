#include <vector>
#include <algorithm>

using namespace std;

#include "randomAccessSparseTable.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testRandomAccessSparseTable() {
    return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Random Access Sparse Table -------------" << endl;
    int N = 100;
    vector<int> in(N);
    for (int i = 0; i < N; i++)
        in[i] = i;
    random_shuffle(in.begin(), in.end());

    RandomAccessSparseTable rast(N);
    rast.add(in[0], -1);
    for (int i = 1; i < N; i++)
        rast.add(in[i], in[i - 1]);

    //cout << in << endl;
    for (int i = 0; i < N; i++) {
        for (int j = i; j >= 0; j--) {
            int ans = rast.query(in[i], i - j);
            int gt = in[j];
            if (ans != gt) {
                cout << "Wrong at (" << i << ", " << j << ") : gt = " << gt << ", ans = " << ans << endl;
            }
            assert(ans == gt);
        }
    }

    cout << "OK!" << endl;
}
