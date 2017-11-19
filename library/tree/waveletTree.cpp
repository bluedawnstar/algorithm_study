#include <vector>
#include <algorithm>

using namespace std;

#include "waveletTree.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testWaveletTree() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Wavelet Tree --------------------------------------" << endl;

    int N = 1000;
    int T = 1000;
    for (int i = 0; i < T; i++) {
        vector<int> in(N);
        for (int j = 0; j < N; j++)
            in[j] = rand() % N;

        WaveletTree<int> tree;
        tree.build(in);

        int K = rand() % N;
        int L = rand() % N;
        int R = rand() % N;

        if (L > R)
            swap(L, R);

        int gt = 0;
        for (int j = L; j <= R; j++)
            gt += (in[j] <= K);

        int ans = tree.queryCountLessOrEqual(L, R, K);
        if (ans != gt) {
            cout << "(" << L << ", " << R << ") : GT = " << gt << ", " << "ans = " << ans << endl;
        }
        //assert(ans == gt);
    }

    cout << "OK!" << endl;
}
