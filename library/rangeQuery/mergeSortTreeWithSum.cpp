#include <limits>
#include <functional>
#include <iterator>
#include <vector>
#include <algorithm>

using namespace std;

#include "mergeSortTreeWithSum.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

static pair<int,int> countLTE(vector<int>& v, int L, int R, int K) {
    pair<int,int> res;
    for (int i = L; i <= R; i++) {
        if (v[i] <= K) {
            res.first++;
            res.second += v[i];
        }
    }
    return res;
}

static int kth(vector<int>& v, int L, int R, int K) {
    vector<int> t(R - L + 1);
    for (int i = L; i <= R; i++)
        t[i - L] = v[i];
    sort(t.begin(), t.end());
    return t[K];
}

static pair<int,int> countK(vector<int>& v, int L, int R, int K) {
    pair<int, int> res;
    for (int i = L; i <= R; i++) {
        if (v[i] == K) {
            res.first++;
            res.second += v[i];
        }
    }
    return res;
}

static pair<int,int> countK(vector<int>& v, int L, int R, int Klo, int Khi) {
    pair<int,int> res;
    for (int i = L; i <= R; i++) {
        if (Klo <= v[i] && v[i] <= Khi) {
            res.first++;
            res.second += v[i];
        }
    }
    return res;
}

static void test(vector<int>& in, MergeSortTreeWithSum<int,int>& tree, int N, int L, int R, int K) {
    // less than or equal to k
    {
        auto gt = countLTE(in, 0, N - 1, K);
        auto ans = tree.countLessThanOrEqual(K);
        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
    {
        auto gt = countLTE(in, L, R, K);
        auto ans = tree.countLessThanOrEqual(L, R, K);
        if (ans != gt) {
            cout << "(" << L << ", " << R << ") : GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
    // kth number
    {
        int K = RandInt32::get() % N;
        int gt = kth(in, 0, N - 1, K);
        int ans = tree.kth(K);
        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
    {
        int K = RandInt32::get() % (R - L + 1);
        int gt = kth(in, L, R, K);
        int ans = tree.kth(L, R, K, 0, numeric_limits<int>::max());
        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
    // count
    {
        int K = in[N / 2];

        auto gt = countK(in, 0, N - 1, K);
        auto ans = tree.count(K);
        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
    {
        int K = in[L + (R - L) / 2];

        auto gt = countK(in, L, R, K);
        auto ans = tree.count(L, R, K);
        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
    // count
    {
        int Klow = in[N / 3];
        int Khigh = in[N * 2 / 3];

        if (Klow > Khigh)
            swap(Klow, Khigh);

        auto gt = countK(in, 0, N - 1, Klow, Khigh);
        auto ans = tree.count(Klow, Khigh);
        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
    {
        int Klow = in[L + (R - L) / 3];
        int Khigh = in[L + (R - L) * 2 / 3];

        if (Klow > Khigh)
            swap(Klow, Khigh);

        auto gt = countK(in, L, R, Klow, Khigh);
        auto ans = tree.count(L, R, Klow, Khigh);
        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
}

void testMergeSortTreeWithSum() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Merge Sort Tree with Sum -------------------------------------------" << endl;

    int N = 100;
    int T = 100;
    for (int i = 0; i < T; i++) {
        vector<int> in(N);
        for (int j = 0; j < N; j++)
            in[j] = RandInt32::get();
        //for (int j = 0; j < N; j++)
        //    in[j] = j;

        MergeSortTreeWithSum<int,int> tree(in);
        for (int j = 0; j < T; j++) {
            int K = RandInt32::get() % N;
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;

            if (L > R)
                swap(L, R);

            test(in, tree, N, L, R, K);
        }
    }

    cout << "OK!" << endl;
}
