#include <functional>
#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreePersistentLazySimple.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

long long sumSlow(const vector<long long>& v, int L, int R) {
    long long res = 0;
    while (L <= R)
        res += v[L++];
    return res;
}

void updateSlow(vector<long long>& v, int L, int R, int x) {
    while (L <= R)
        v[L++] += x;
}

void testSegmentTreePersistentLazySimple() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Simple Lazy Persistent Segment Tree --------------------------------------" << endl;
    {
        int T = 1000;
        int N = 10000;
        int maxX = 100000;

#ifdef _DEBUG
        T = 100;
        N = 1000;
#endif

        for (int t = 0; t < T; t++) {
            vector<vector<long long>> v(T + 1);
            v[0].assign(N, 0);

            SimplePersistentSegmentTreeLazy<long long> tree(
                [](long long a, long long b) { return a + b; },
                [](long long x, int n) { return x * n; },
                0ll);
            vector<int> roots;
            roots.push_back(tree.build(N));

            for (int i = 0; i < T; i++) {
                int L = rand() % N;
                int R = rand() % N;
                if (L > R)
                    swap(L, R);

                //int history = i;
                int history = rand() % int(roots.size());

                int x = rand() % maxX + 1;

                auto ans = tree.query(roots[history], L, R);
                long long gt = sumSlow(v[history], L, R);
                if (ans != gt)
                    cout << "ERROR : mismatched!" << endl;
                assert(ans == gt);

                roots.push_back(tree.update(roots[history], L, R, x));
                v[i + 1] = v[history];
                updateSlow(v[i + 1], L, R, x);
            }
        }
    }

    cout << "OK!" << endl;
}
