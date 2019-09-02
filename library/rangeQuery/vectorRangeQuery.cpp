#include <cassert>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "vectorRangeQuery.h"

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

static int minRange(vector<int>& v, int L, int R) {
    int res = v[L++];
    while (L <= R)
        res = min(res, v[L++]);
    return res;
}

static int maxRange(vector<int>& v, int L, int R) {
    int res = v[L++];
    while (L <= R)
        res = max(res, v[L++]);
    return res;
}

void testVectorRangeQuery() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Vector Range Query ----------" << endl;
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

        VectorRangeQuery<int> vrq([](int a, int b) { return a + b; }, 0, N + T);
        vrq.build(vec);

        for (int i = 0; i < T; i++) {
            int cmd = RandInt32::get() % 3;
            if (cmd == 0) {
                int j = RandInt32::get() % (int(vec.size()) + 1);
                int val = RandInt32::get() % 100;
                vec.insert(vec.begin() + j, val);
                vrq.insert(j, val);
            } else if (cmd == 1) {
                int j = RandInt32::get() % int(vec.size());
                vec.erase(vec.begin() + j);
                vrq.erase(j);
            } else {
                int L = RandInt32::get() % int(vec.size());
                int R = RandInt32::get() % int(vec.size());

                if (L > R)
                    swap(L, R);

                assert(vrq.get(L) == vec[L]);
                assert(vrq.get(R) == vec[R]);

                int gt = sumRange(vec, L, R);
                int ans = vrq.query(L, R);
                if (gt != ans)
                    cout << "Mismatched: " << gt << ", " << ans << endl;
                assert(gt == ans);

                vec[L] = RandInt32::get();
                vec[R] = RandInt32::get();
                vrq.update(L, vec[L]);
                vrq.update(R, vec[R]);
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

        VectorRangeQuery<int> vrq([](int a, int b) { return min(a, b); }, numeric_limits<int>::max(), N + T);
        vrq.build(vec);

        for (int i = 0; i < T; i++) {
            int cmd = RandInt32::get() % 3;
            if (cmd == 0) {
                int j = RandInt32::get() % (int(vec.size()) + 1);
                int val = RandInt32::get();
                vec.insert(vec.begin() + j, val);
                vrq.insert(j, val);
            } else if (cmd == 1) {
                int j = RandInt32::get() % int(vec.size());
                vec.erase(vec.begin() + j);
                vrq.erase(j);
            } else {
                int L = RandInt32::get() % int(vec.size());
                int R = RandInt32::get() % int(vec.size());

                if (L > R)
                    swap(L, R);

                assert(vrq.get(L) == vec[L]);
                assert(vrq.get(R) == vec[R]);

                int gt = minRange(vec, L, R);
                int ans = vrq.query(L, R);
                if (gt != ans)
                    cout << "Mismatched: " << gt << ", " << ans << endl;
                assert(gt == ans);

                vec[L] = RandInt32::get();
                vec[R] = RandInt32::get();
                vrq.update(L, vec[L]);
                vrq.update(R, vec[R]);
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

        VectorRangeQuery<int> vrq([](int a, int b) { return max(a, b); }, 0, N + T);
        vrq.build(vec);

        for (int i = 0; i < T; i++) {
            int cmd = RandInt32::get() % 3;
            if (cmd == 0) {
                int j = RandInt32::get() % (int(vec.size()) + 1);
                int val = RandInt32::get();
                vec.insert(vec.begin() + j, val);
                vrq.insert(j, val);
            } else if (cmd == 1) {
                int j = RandInt32::get() % int(vec.size());
                vec.erase(vec.begin() + j);
                vrq.erase(j);
            } else {
                int L = RandInt32::get() % int(vec.size());
                int R = RandInt32::get() % int(vec.size());

                if (L > R)
                    swap(L, R);

                assert(vrq.get(L) == vec[L]);
                assert(vrq.get(R) == vec[R]);

                int gt = maxRange(vec, L, R);
                int ans = vrq.query(L, R);
                if (gt != ans)
                    cout << "Mismatched: " << gt << ", " << ans << endl;
                assert(gt == ans);

                vec[L] = RandInt32::get();
                vec[R] = RandInt32::get();
                vrq.update(L, vec[L]);
                vrq.update(R, vec[R]);
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

        VectorRangeQuery<int> vrq([](int a, int b) { return a + b; }, 0, N + T);
        vrq.build(vec);

        PROFILE_START(0);

        int cnt = N;
        for (int i = 0; i < T; i++) {
            int cmd = RandInt32::get() % 3;
            if (cmd == 0) {
                int j = RandInt32::get() % (cnt + 1);
                int val = RandInt32::get() % 100;
                vrq.insert(j, val);
                cnt++;
            } else if (cmd == 1) {
                int j = RandInt32::get() % cnt;
                vrq.erase(j);
                cnt--;
            } else {
                int L = RandInt32::get() % cnt;
                int R = RandInt32::get() % cnt;

                if (L > R)
                    swap(L, R);

                int gt = vrq.query(L, R);
                if (gt < 0)
                    cout << "ERROR!" << endl;
            }
        }

        PROFILE_STOP(0);
    }

    cout << "OK!" << endl;
}
