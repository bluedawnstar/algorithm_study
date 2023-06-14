#include <functional>
#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

#include "segmentTreePersistentLazy.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int sumSlow(vector<int>& v, int L, int R) {
    int res = 0;
    while (L <= R)
        res += v[L++];
    return res;
}

static void updateSlow(vector<int>& v, int L, int R, int x) {
    while (L <= R)
        v[L++] += x;
}

static int lowerBoundSlow(vector<int>& v, int k) {
    int res = 0;
    for (int i = 0; i < int(v.size()); i++) {
        res += v[i];
        if (res >= k)
            return i;
    }
    return int(v.size());
}

static long long sumSlow(const vector<long long>& v, int L, int R) {
    long long res = 0;
    while (L <= R)
        res += v[L++];
    return res;
}

static void updateSlow(vector<long long>& v, int L, int R, int x) {
    while (L <= R)
        v[L++] += x;
}

void testSegmentTreePersistentLazy() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Lazy Persistent Segment Tree --------------------------------------" << endl;
    {
        const int N = 1000;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        PersistentSegmentTreeLazy<int> tree(in,
                                            [](int a, int b) { return a + b; },
                                            [](int x, int n) { return x * n; }, 0);
        int root = tree.getInitRoot();
        for (int i = 0; i < N; i++) {
            int R = RandInt32::get() % N;
            int sum = sumSlow(in, 0, R);

            auto ans = tree.lowerBound(root, [sum](int val) { return val >= sum; });
            int gt = lowerBoundSlow(in, sum);
            if (ans != gt) {
                cout << "[" << sum << "] ans = " << ans << ", gt = " << gt << endl;
                ans = tree.lowerBound(root, [sum](int val) { return val >= sum; });
            }
            assert(ans == gt);
        }
    }
    cout << "OK!" << endl;
    {
        const int N = 100;
        const int T = 100;
        vector<int> in(N);

        for (int i = 0; i < N; i++)
            in[i] = RandInt32::get() % 1000;

        PersistentSegmentTreeLazy<int> tree(in,
                                            [](int a, int b) { return a + b; },
                                            [](int x, int n) { return x * n; }, 0);
        vector<int> roots;
        int root = tree.getInitRoot();

        vector<vector<pair<int, int>>> gt(T, vector<pair<int, int>>(N));    // (sum, index)
        for (int j = 0; j < T; j++) {
            for (int i = 0; i < N; i++) {
                int R = RandInt32::get() % N;
                int sum = sumSlow(in, 0, R);
                gt[j][i].first = sum;
                gt[j][i].second = lowerBoundSlow(in, sum);
            }
            roots.push_back(root);

            int R = RandInt32::get() % N;
            int newVal = RandInt32::get() % 1000;
            if (j & 1) {
                root = tree.update(root, R, newVal);
                in[R] += newVal;
            } else {
                root = tree.set(root, R, newVal);
                in[R] = newVal;
            }
        }

        for (int j = 0; j < T; j++) {
            for (int i = 0; i < N; i++) {
                int sum = gt[j][i].first;

                auto ans = tree.lowerBound(roots[j], [sum](int val) { return val >= sum; });
                if (ans != gt[j][i].second) {
                    cout << "[" << sum << "] ans = " << ans << ", gt = " << gt << endl;
                    ans = tree.lowerBound(roots[j], [sum](int val) { return val >= sum; });
                }
                assert(ans == gt[j][i].second);
            }
        }
    }
    cout << "OK!" << endl;
    {
        const int N = 100;
        const int T = 1000;

        vector<vector<int>> vv(1);
        vv[0].resize(N);

        PersistentSegmentTreeLazy<int> tree(N, [](int a, int b) { return a + b; }, [](int a, int n) { return a * n; }, 0);

        int root = tree.getInitRoot();
        for (int i = 0; i < T; i++) {
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;
            if (L > R)
                swap(L, R);
            int x = RandInt32::get() % 1000;
            {
                int gt = sumSlow(vv.back(), L, R);
                int ans = tree.query(root, L, R);
                assert(gt == ans);
            }
            {
                int sum = tree.query(root, 0, L) + RandInt32::get() % 1000;
                int gt = lowerBoundSlow(vv.back(), sum);
                int ans = tree.lowerBound(root, [sum](int x) { return sum <= x; });
                assert(gt == ans);
            }

            vv.push_back(vv.back());
            updateSlow(vv.back(), L, R, x);

            root = tree.update(root, L, R, x);
        }
    }
    {
#ifdef _DEBUG
        const int T = 100;
        const int N = 1000;
#else
        const int T = 1000;
        const int N = 10000;
#endif
        int maxX = 100000;

        for (int t = 0; t < T; t++) {
            vector<vector<long long>> v(T + 1);
            v[0].assign(N, 0);

            PersistentSegmentTreeLazy<long long> tree(
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
