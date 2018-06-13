#include <vector>
#include <algorithm>

using namespace std;

#include "sortedVector.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int countLessOrEqual(vector<int>& v, int L, int R, int val) {
    int res = 0;
    while (L <= R) {
        if (v[L++] <= val)
            res++;
    }
    return res;
}

void testSortedVector() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Sorted Vector ----------" << endl;
    {
        int N = 100000;
        int T = 100000;
#ifdef _DEBUG
        N = 10000;
        T = 10000;
#endif

        vector<int> vec(N);
        for (int i = 0; i < N; i++)
            vec[i] = RandInt32::get();

        SortedVector<int> sv(N + T);
        sv.build(vec);

        for (int i = 0; i < T; i++) {
            int cmd = RandInt32::get() % 3;
            if (cmd == 0) {
                int i = RandInt32::get() % ((int)vec.size() + 1);
                int val = RandInt32::get();
                vec.insert(vec.begin() + i, val);
                sv.insert(i, val);
            } else if (cmd == 1) {
                int i = RandInt32::get() % (int)vec.size();
                vec.erase(vec.begin() + i);
                sv.erase(i);
            } else {
                int L = RandInt32::get() % (int)vec.size();
                int R = RandInt32::get() % (int)vec.size();
                int val = RandInt32::get();

                if (L > R)
                    swap(L, R);

                int gt = countLessOrEqual(vec, L, R, val);
                int ans = sv.countLessOrEqual(L, R, val);
                if (gt != ans)
                    cout << "Mismatched: " << gt << ", " << ans << endl;
                assert(gt == ans);
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
            vec[i] = RandInt32::get();

        SortedVector<int> sv(N + T);
        sv.build(vec);

        PROFILE_START(0);

        int cnt = N;
        for (int i = 0; i < T; i++) {
            int cmd = RandInt32::get() % 3;
            if (cmd == 0) {
                int i = RandInt32::get() % (cnt + 1);
                int val = RandInt32::get();
                sv.insert(i, val);
                cnt++;
            } else if (cmd == 1) {
                int i = RandInt32::get() % cnt;
                sv.erase(i);
                cnt--;
            } else {
                int L = RandInt32::get() % cnt;
                int R = RandInt32::get() % cnt;
                int val = RandInt32::get();

                if (L > R)
                    swap(L, R);

                int gt = sv.countLessOrEqual(L, R, val);
                if (gt < 0)
                    cout << "ERROR!" << endl;
            }
        }

        PROFILE_STOP(0);
    }

    cout << "OK!" << endl;
}
