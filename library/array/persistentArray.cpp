#include <vector>
#include <algorithm>

using namespace std;

#include "persistentArray.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

void testPersistentArray() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Persistent Array ----------" << endl;

    int N = 100;
    int T = 1000;
    vector<vector<int>> in(T);
    
    PartiallyPersistentArray<int> pa(N, 0);
    in[0].resize(N);

    for (int i = 1; i < T; i++) {
        int j = rand() % N;
        int x = rand();
        in[i] = in[i - 1];
        in[i][j] = x;
        pa.set(j, x);
    }

    for (int i = 0; i < T; i++) {
        for (int j = 0; j < N; j++)
            assert(in[i][j] == pa.get(i, j));
    }

    cout << "OK!" << endl;
}
