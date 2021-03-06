#include <cassert>
#include <functional>
#include <vector>
#include <algorithm>

using namespace std;

#include "vectorRangeCount.h"

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

static int countLess(vector<int>& v, int L, int R, int val) {
    int res = 0;
    while (L <= R) {
        if (v[L++] < val)
            res++;
    }
    return res;
}

static int countGreaterOrEqual(vector<int>& v, int L, int R, int val) {
    int res = 0;
    while (L <= R) {
        if (v[L++] >= val)
            res++;
    }
    return res;
}

static int countGreater(vector<int>& v, int L, int R, int val) {
    int res = 0;
    while (L <= R) {
        if (v[L++] > val)
            res++;
    }
    return res;
}

static int count(vector<int>& v, int L, int R, int val) {
    int res = 0;
    while (L <= R) {
        if (v[L++] == val)
            res++;
    }
    return res;
}

static pair<int,int> minmax(vector<int>& v, int L, int R) {
    pair<int, int> res{ v[L], v[L] };
    while (++L <= R) {
        res.first = min(res.first, v[L]);
        res.second = max(res.second, v[L]);
    }
    return res;
}

static int kth(vector<int>& v, int L, int R, int k) {
    vector<int> vec;
    vec.reserve(R - L + 1);
    while (L <= R)
        vec.push_back(v[L++]);
    sort(vec.begin(), vec.end());
    return vec[k];
}

void testVectorRangeCount() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Vector Range Count ----------" << endl;
    {
        int N = 20000;
        int T = 20000;
#ifdef _DEBUG
        N = 1000;
        T = 1000;
#endif

        vector<int> vec(N);
        for (int i = 0; i < N; i++)
            vec[i] = RandInt32::get();

        VectorRangeCount<int> vrc(N + T);
        vrc.build(vec);

        for (int i = 0; i < T; i++) {
            int cmd = RandInt32::get() % 3;
            if (cmd == 0) {
                int j = RandInt32::get() % (int(vec.size()) + 1);
                int val = RandInt32::get();
                vec.insert(vec.begin() + j, val);
                vrc.insert(j, val);
            } else if (cmd == 1) {
                int j = RandInt32::get() % int(vec.size());
                vec.erase(vec.begin() + j);
                vrc.erase(j);
            } else {
                int L = RandInt32::get() % int(vec.size());
                int R = RandInt32::get() % int(vec.size());
                int val = RandInt32::get();

                if (L > R)
                    swap(L, R);

                assert(vrc.get(L) == vec[L]);
                assert(vrc.get(R) == vec[R]);

                int gt = countLessOrEqual(vec, L, R, val);
                int ans = vrc.countLessOrEqual(L, R, val);
                if (gt != ans)
                    cout << "Mismatched: " << gt << ", " << ans << endl;
                assert(gt == ans);

                auto mm = minmax(vec, L, R);
                int k = RandInt32::get() % (R - L + 1);
                int gt2 = kth(vec, L, R, k);
                int ans2 = vrc.kth(L, R, k, mm.first, mm.second);
                if (gt2 != ans2)
                    cout << "Mismatched: " << gt2 << ", " << ans2 << endl;
                assert(gt2 == ans2);

                assert(countLess(vec, L, R, val) == vrc.countLess(L, R, val));
                assert(countGreaterOrEqual(vec, L, R, val) == vrc.countGreaterOrEqual(L, R, val));
                assert(countGreater(vec, L, R, val) == vrc.countGreater(L, R, val));
                assert(count(vec, L, R, val) == vrc.count(L, R, val));

                vec[L] = RandInt32::get();
                vec[R] = RandInt32::get();
                vrc.update(L, vec[L]);
                vrc.update(R, vec[R]);
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

        VectorRangeCount<int> vrc(N + T);
        vrc.build(vec);

        PROFILE_START(0);

        int cnt = N;
        for (int i = 0; i < T; i++) {
            int cmd = RandInt32::get() % 3;
            if (cmd == 0) {
                int j = RandInt32::get() % (cnt + 1);
                int val = RandInt32::get();
                vrc.insert(j, val);
                cnt++;
            } else if (cmd == 1) {
                int j = RandInt32::get() % cnt;
                vrc.erase(j);
                cnt--;
            } else {
                int L = RandInt32::get() % cnt;
                int R = RandInt32::get() % cnt;
                int val = RandInt32::get();

                if (L > R)
                    swap(L, R);

                int gt = vrc.countLessOrEqual(L, R, val);
                if (gt < 0)
                    cout << "ERROR!" << endl;
            }
        }

        PROFILE_STOP(0);
    }

    cout << "OK!" << endl;
}
