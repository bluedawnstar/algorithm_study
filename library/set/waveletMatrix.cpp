#include <limits>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

#include "waveletMatrix.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

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

static void test(vector<int>& in, WaveletMatrix<int>& matrix, int N, int L, int R) {
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

        int gt = countK(in, L, R, K);
        int ans = get<0>(matrix.count(L, R, K));
        if (ans != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans << endl;
        }
        assert(ans == gt);
    }
}

void testWaveletMatrix() {
    //return; //TODO: if you want to test a split function, make this line a comment.

    cout << "-- Wavelet Matrix --------------------------------------" << endl;

    int N = 100;
    int T = 1000;
    for (int i = 0; i < T; i++) {
        vector<int> in(N);
        for (int j = 0; j < N; j++)
            in[j] = rand() * rand();
        //for (int j = 0; j < N; j++)
        //    in[j] = j;

        WaveletMatrix<int> matrix;
        matrix.build(in);

        for (int j = 0; j < 10; j++) {
            int L = rand() % N;
            int R = rand() % N;

            if (L > R)
                swap(L, R);

            test(in, matrix, N, L, R);
        }
    }

    cout << "OK!" << endl;
}
