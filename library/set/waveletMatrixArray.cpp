#include <limits>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

#include "waveletMatrixArray.h"
#include "waveletMatrix.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static int countLess(vector<int>& v, int L, int R, int K) {
    int res = 0;
    for (int i = L; i <= R; i++)
        res += (v[i] < K);
    return res;
}

static int countGreater(vector<int>& v, int L, int R, int K) {
    int res = 0;
    for (int i = L; i <= R; i++)
        res += (v[i] > K);
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

static void testSpeed() {
    cout << "--- Wavelet Matrix - Speed Test ---" << endl;

    int N = 100000;
    int T = 1000000;

    vector<int> in(N);
    for (int j = 0; j < N; j++)
        in[j] = rand() * rand();

    vector<tuple<int, int, int>> qryKth(T);
    for (int i = 0; i < T; i++) {
        int L = rand() % N;
        int R = rand() % N;

        if (L > R)
            swap(L, R);
        int K = rand() % (R - L + 1);

        qryKth.emplace_back(L, R, K);
    }

    PROFILE_START(0);
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
    PROFILE_STOP(0);

    PROFILE_START(1);
    {
        WaveletMatrixArray<int> matrix;
        matrix.build(in);
        for (int i = 0; i < T; i++) {
            auto& t = qryKth[i];
            int ans = matrix.kth(get<0>(t), get<1>(t), get<2>(t));
            if (ans < 0)
                cerr << "I'll never be shown!" << endl;
        }
    }
    PROFILE_STOP(1);
}

static void test(vector<int>& in, WaveletMatrixArray<int>& matrix, int N, int L, int R) {
    // get number
    {
        int K = rand() % N;
        int gt = in[K];
        int ans = matrix.get(K);
        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
            ans = matrix.get(K);
        }
        assert(ans == gt);
    }
    // kth number
    {
        int K = rand() % (R - L + 1);
        int gt = kth(in, L, R, K);
        int ans = matrix.kth(L, R, K);
        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
            ans = matrix.kth(L, R, K);
        }
        assert(ans == gt);
    }
    // count
    {
        int K = in[L + (R - L) / 2];

        auto ans = matrix.countEx(L, R, K);

        int gt = countK(in, L, R, K);
        if (get<0>(ans) != gt) {
            cout << "GT = " << gt << ", " << "ans = " << get<0>(ans) << endl;
        }
        assert(get<0>(ans) == gt);

        gt = countLess(in, L, R, K);
        if (get<1>(ans) != gt) {
            cout << "GT = " << gt << ", " << "ans = " << get<1>(ans) << endl;
        }
        assert(get<1>(ans) == gt);

        gt = countGreater(in, L, R, K);
        if (get<2>(ans) != gt) {
            cout << "GT = " << gt << ", " << "ans = " << get<2>(ans) << endl;
        }
        assert(get<2>(ans) == gt);
    }
    // count with range
    {
        int Klow = in[L + (R - L) / 3];
        int Khigh = in[L + (R - L) * 2 / 3];

        if (Klow > Khigh)
            swap(Klow, Khigh);

        int gt = countK(in, L, R, Klow, Khigh);
        int ans = matrix.count(L, R, Klow, Khigh);

        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
}

void testWaveletMatrixArray() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Wavelet Matrix Array --------------------------------------" << endl;

    int N = 100;
    int T = 1000;
    for (int i = 0; i < T; i++) {
        vector<int> in(N);
        for (int j = 0; j < N; j++)
            in[j] = rand() * rand();

        WaveletMatrixArray<int> matrix;
        matrix.build(in);

        for (int j = 0; j < 10; j++) {
            int L = rand() % N;
            int R = rand() % N;

            if (L > R)
                swap(L, R);

            test(in, matrix, N, L, R);
        }
    }

    testSpeed();

    cout << "OK!" << endl;
}
