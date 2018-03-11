#include <functional>
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

static int findFirst(const vector<int>& A, int start, int R) {
    while (start > R)
        start = A[start];
    return start;
}

static int findLast(const vector<int>& A, int start, int R) {
    while (A[start] > R)
        start = A[start];
    return start;
}

void testRandomAccessSparseTable() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Random Access Sparse Table -------------" << endl;
    {
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
    }
    {
        static const int T = 500;
        int N = 10000;
        vector<int> in(N);
        in[0] = -1;
        for (int i = 1; i < N; i++)
            in[i] = rand() % i;

        RandomAccessSparseTable rast(N);
        rast.build(in);

        for (int i = N - 1; i >= 0; i--) {
            int ans1 = rast.findFirst(i, [](int x) {
                return x <= T;
            });
            int gt1 = findFirst(in, i, T);
            assert(gt1 == ans1);

            int ans2 = rast.findLast(i, [](int x) {
                return x > T;
            });
            int gt2 = findLast(in, i, T);
            assert(gt2 == ans2);
        }
    }

    cout << "OK!" << endl;
}
