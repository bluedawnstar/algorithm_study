#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

#include "vectorRangeSum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int sumRange(vector<int>& v, int L, int R) {
    int res = 0;
    while (L <= R)
        res += v[L++];
    return res;
}

void testVectorRangeSum() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Vector Range Sum ----------" << endl;
    {
        int N = 100000;
        int T = 100000;
#ifdef _DEBUG
        N = 10000;
        T = 10000;
#endif

        vector<int> vec(N);
        for (int i = 0; i < N; i++)
            vec[i] = RandInt32::get() % 100;

        VectorRangeSum<int> vrs(N + T);
        vrs.build(vec);

        for (int i = 0; i < T; i++) {
            int cmd = RandInt32::get() % 3;
            if (cmd == 0) {
                int j = RandInt32::get() % (int(vec.size()) + 1);
                int val = RandInt32::get() % 100;
                vec.insert(vec.begin() + j, val);
                vrs.insert(j, val);
            } else if (cmd == 1) {
                int j = RandInt32::get() % int(vec.size());
                vec.erase(vec.begin() + j);
                vrs.erase(j);
            } else {
                int L = RandInt32::get() % int(vec.size());
                int R = RandInt32::get() % int(vec.size());

                if (L > R)
                    swap(L, R);

                assert(vrs.get(L) == vec[L]);
                assert(vrs.get(R) == vec[R]);

                int gt = sumRange(vec, L, R);
                int ans = vrs.sum(L, R);
                if (gt != ans)
                    cout << "Mismatched: " << gt << ", " << ans << endl;
                assert(gt == ans);

                vec[L] = RandInt32::get();
                vec[R] = RandInt32::get();
                vrs.update(L, vec[L]);
                vrs.update(R, vec[R]);
            }
        }
    }
    {
        int N = 100000;
        int T = 100000;
#ifdef _DEBUG
        N = 10000;
        T = 10000;
#endif

        vector<int> vec(N);
        for (int i = 0; i < N; i++)
            vec[i] = RandInt32::get() % 100;

        VectorRangeSum<int> vrs(N + T);
        vrs.build(vec);

        PROFILE_START(0);

        int cnt = N;
        for (int i = 0; i < T; i++) {
            int cmd = RandInt32::get() % 3;
            if (cmd == 0) {
                int j = RandInt32::get() % (cnt + 1);
                int val = RandInt32::get() % 100;
                vrs.insert(j, val);
                cnt++;
            } else if (cmd == 1) {
                int j = RandInt32::get() % cnt;
                vrs.erase(j);
                cnt--;
            } else {
                int L = RandInt32::get() % cnt;
                int R = RandInt32::get() % cnt;

                if (L > R)
                    swap(L, R);

                int gt = vrs.sum(L, R);
                if (gt < 0)
                    cout << "ERROR!" << endl;
            }
        }

        PROFILE_STOP(0);
    }

    cout << "OK!" << endl;
}
