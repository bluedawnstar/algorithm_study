#include <limits>
#include <tuple>
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

#include "waveletMatrixArrayIndirect.h"

// https://www.codechef.com/APRIL19A/problems/XORMIN
//   It can be solved by WaveletMatrixArrayIndirect<T>.

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"
#include "../common/rand.h"

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

static void test(vector<int>& in, WaveletMatrixArrayIndirect<int>& matrix, int N, int L, int R) {
    // get number
    {
        int K = RandInt32::get() % N;
        int gt = in[K];
        auto ans = matrix.get(K);
        if (ans.first != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans.first << endl;
            ans = matrix.get(K);
        }
        if (matrix.values[ans.second] != gt) {
            cout << "GT = " << gt << ", " << "ans = " << matrix.values[ans.second] << endl;
            ans = matrix.get(K);
        }
        assert(ans.first == gt);
        assert(matrix.values[ans.second] == gt);
    }
    // kth number
    {
        int K = RandInt32::get() % (R - L + 1);
        int gt = kth(in, L, R, K);
        auto ans = matrix.kth(L, R, K);
        if (ans.first != gt) {
            cout << "GT = " << gt << ", " << "ans = " << ans.first << endl;
            ans = matrix.kth(L, R, K);
        }
        assert(ans.first == gt);
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

void testWaveletMatrixArrayIndirect() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "-- Wavelet Matrix Array with Indirect Addressing ------------------------------" << endl;
#if 0
    {
        int N = 10000;
        int T = 1000;
#ifdef _DEBUG
        N = 100;
#endif
        for (int i = 0; i < T; i++) {
            vector<int> in(N);
            for (int j = 0; j < N; j++)
                in[j] = RandInt32::get() & 0xffffff;

            WaveletMatrixArrayIndirect<int> matrix;
            matrix.build(in);

            for (int j = 0; j < 10; j++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;

                if (L > R)
                    swap(L, R);

                test(in, matrix, N, L, R);
            }
        }
    }
#endif
    {
        int N = 10000;
        int T = 1000;
        int mask = 0xffffff;
#ifdef _DEBUG
        N = 100;
#endif
        for (int i = 0; i < T; i++) {
            vector<int> in(N);
            for (int j = 0; j < N; j++)
                in[j] = RandInt32::get() & mask;

            WaveletMatrixArrayIndirect<int> matrix;
            auto idx = matrix.build(in);

            //for (int j = 0; j < 100; j++) {
            for (int j = 0; j < 1000; j++) {
                int L = RandInt32::get() % N;
                int R = RandInt32::get() % N;

                if (L > R)
                    swap(L, R);

                int K = RandInt32::get() & mask;
                auto ans = matrix.findMaxXor(L, R, K);
                vector<int> ansV;

                if (ans.second.first > ans.second.second || ans.second.first < 0 || ans.second.second >= N)
                    cout << "ERROR!" << endl;

                // reordering
                for (int kk = ans.second.first; kk <= ans.second.second; kk++)
                    ansV.push_back(idx[kk]);

                int gt = -1;
                vector<int> gtV;
                for (int kk = L; kk <= R; kk++) {
                    int x = in[kk] ^ K;
                    if (gt < x) {
                        gt = x;
                        gtV.clear();
                        gtV.push_back(kk);
                    } else if (gt == x) {
                        gtV.push_back(kk);
                    }
                }
                gt ^= K;

                if (ans.first != gt) {
                    cout << "GT = " << gt << ", " << "ans = " << ans.first << endl;
                    //ans = matrix.findMaxXor(L, R, K);
                }
                if (ansV != gtV) {
                    cout << "GT = " << gtV << ", " << "ans = " << ansV << endl;
                    //ans = matrix.findMaxXor(L, R, K);
                }
                assert(ans.first == gt);
                assert(ansV == gtV);
            }
        }
    }
    cout << "OK!" << endl;
}
