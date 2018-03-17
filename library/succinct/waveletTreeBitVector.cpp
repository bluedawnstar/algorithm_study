#include <limits>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

#include "waveletTree.h"
#include "waveletTreeBitVector.h"
#include "../succinct/waveletMatrix.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

static int countLTE(vector<int>& v, int L, int R, int K) {
    int res = 0;
    for (int i = L; i <= R; i++)
        res += (v[i] <= K);
    return res;
}

static int kth(vector<int>& v, int L, int R, int K) {
    vector<int> t(R - L + 1);
    for (int i = L; i <= R; i++)
        t[i - L] = v[i];
    sort(t.begin(), t.end());
    return t[K];
}

static int countK(vector<int>& v, int L, int R, int K) {
    int res = 0;
    for (int i = L; i <= R; i++) {
        if (v[i] == K)
            res++;
    }
    return res;
}

static int countK(vector<int>& v, int L, int R, int Klo, int Khi) {
    int res = 0;
    for (int i = L; i <= R; i++) {
        if (Klo <= v[i] && v[i] <= Khi)
            res++;
    }
    return res;
}

static void testSpeedTree() {
    cout << "--- Wavelet Tree - Speed Test ---" << endl;

    int N = 100000;
    int T = 1000000;

    vector<int> in(N);
    for (int j = 0; j < N; j++)
        in[j] = RandInt32::get();

    PROFILE_START(0);
    {
        WaveletTree<int> tree;
        tree.build(in);
        for (int i = 0; i < T; i++) {
            int K = RandInt32::get() % N;
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;

            if (L > R)
                swap(L, R);

            int ans = tree.countLessOrEqual(K);
            if (ans < 0)
                cerr << "I'll never be shown!" << endl;
        }
    }
    PROFILE_STOP(0);

    PROFILE_START(1);
    {
        WaveletTreeBitVector<int> tree;
        tree.build(in);
        for (int i = 0; i < T; i++) {
            int K = RandInt32::get() % N;
            int L = RandInt32::get() % N;
            int R = RandInt32::get() % N;

            if (L > R)
                swap(L, R);

            int ans = tree.countLessOrEqual(K);
            if (ans < 0)
                cerr << "I'll never be shown!" << endl;
        }
    }
    PROFILE_STOP(1);
}

static void testSpeedAll() {
    cout << "--- Wavelet Tree & Matrix - Speed Test ---" << endl;

    int N = 100000;
    int T = 1000000;

    vector<int> in(N);
    for (int j = 0; j < N; j++)
        in[j] = RandInt32::get();

    vector<tuple<int, int, int>> qryKth(T);
    for (int i = 0; i < T; i++) {
        int L = RandInt32::get() % N;
        int R = RandInt32::get() % N;

        if (L > R)
            swap(L, R);
        int K = RandInt32::get() % (R - L + 1);

        qryKth.emplace_back(L, R, K);
    }

    PROFILE_START(0);
    {
        WaveletTree<int> tree;
        tree.build(in);
        for (int i = 0; i < T; i++) {
            auto& t = qryKth[i];
            int ans = tree.kth(get<0>(t), get<1>(t), get<2>(t));
            if (ans < 0)
                cerr << "I'll never be shown!" << endl;
        }
    }
    PROFILE_STOP(0);

    PROFILE_START(1);
    {
        WaveletTreeBitVector<int> tree;
        tree.build(in);
        for (int i = 0; i < T; i++) {
            auto& t = qryKth[i];
            int ans = tree.kth(get<0>(t), get<1>(t), get<2>(t));
            if (ans < 0)
                cerr << "I'll never be shown!" << endl;
        }
    }
    PROFILE_STOP(1);

    PROFILE_START(2);
    {
        WaveletMatrix<int> matrix;
        matrix.build(in);
        for (int i = 0; i < T; i++) {
            auto& t = qryKth[i];
            int ans = matrix.kth(get<0>(t), get<1>(t), get<2>(t));
            if (ans < 0)
                cerr << "I'll never be shown!" << endl;
        }
    }
    PROFILE_STOP(2);
}

static void test(vector<int>& in, WaveletTreeBitVector<int>& tree, int N, int L, int R, int K) {
    // less than or equal to k
    {
        int gt = countLTE(in, 0, N - 1, K);
        int ans = tree.countLessOrEqual(K);
        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
    {
        int gt = countLTE(in, L, R, K);
        int ans = tree.countLessOrEqual(L, R, K);
        if (ans != gt) {
            cout << "(" << L << ", " << R << ") : GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
    // kth number
    {
        int K = RandInt32().get() % (R - L + 1);
        int gt = kth(in, L, R, K);
        int ans = tree.kth(L, R, K);
        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
    // count
    {
        int K = in[L + (R - L) / 2];

        int gt = countK(in, L, R, K);
        int ans = tree.count(L, R, K);
        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
    // count
    {
        int Klow = in[L + (R - L) / 3];
        int Khigh = in[L + (R - L) * 2 / 3];

        int gt = countK(in, L, R, Klow, Khigh);
        int ans = tree.count(L, R, Klow, Khigh);
        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
}

void testWaveletTreeBitVector() {
    return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Wavelet Tree with Bit Vector --------------------------------------" << endl;

    //testSpeedTree();
    testSpeedAll();

    int N = 100;
    int T = 10000;
    for (int i = 0; i < T; i++) {
        vector<int> in(N);
        for (int j = 0; j < N; j++)
            in[j] = RandInt32::get();
        //for (int j = 0; j < N; j++)
        //    in[j] = j;

        WaveletTreeBitVector<int> tree;
        tree.build(in);

        int K = RandInt32::get() % N;
        int L = RandInt32::get() % N;
        int R = RandInt32::get() % N;

        if (L > R)
            swap(L, R);

        test(in, tree, N, L, R, K);

        //------- swap ------
        {
            if (R == N - 1) {
                if (L == R)
                    --L, --R;
                else
                    --R;
            }
            tree.swap(R, in[R], in[R + 1]);
            swap(in[R], in[R + 1]);
            K = in[R];
        }
        test(in, tree, N, L, R, K);
    }

    cout << "OK!" << endl;
}
