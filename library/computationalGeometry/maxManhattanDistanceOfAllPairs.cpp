#include <cmath>
#include <limits>
#include <numeric>
#include <array>
#include <vector>
#include <algorithm>

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

using namespace std;

#include "maxManhattanDistanceOfAllPairs.h"

// <Related Problems>
// https://www.codechef.com/problems/DDIMMST

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <set>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

template <typename T>
static T maxManhattanDistanceSlow(const pair<T,T> A[], int N) {
    T res = numeric_limits<T>::min();
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            res = max(res, abs(A[j].first - A[i].first) + abs(A[j].second - A[i].second));
        }
    }
    return res;
}

template <typename T>
static T maxManhattanDistanceSlow(const vector<T> A[], int dimension, int N) {
    T res = numeric_limits<T>::min();
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            T sum = 0;
            for (int k = 0; k < dimension; k++)
                sum += abs(A[j][k] - A[i][k]);
            res = max(res, sum);
        }
    }
    return res;
}

template <typename T, int D>
static T maxManhattanDistanceSlow(const array<T,D> A[], int N) {
    T res = numeric_limits<T>::min();
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            T sum = 0;
            for (int k = 0; k < D; k++)
                sum += abs(A[j][k] - A[i][k]);
            res = max(res, sum);
        }
    }
    return res;
}

void testMaxHanhattanDistanceOfAllPairs() {
    //return; //TODO: if you want to test functions of this file, make this line a comment.

    cout << "--- Max Hanhattan Distance of All Pairs ------------------" << endl;
    {
        const int T = 10;
        const int N = 100;
        const int X = 10000;
        for (int tt = 0; tt < T; tt++) {
            vector<pair<int, int>> A(N);
            for (int i = 0; i < N; i++) {
                A[i].first = RandInt32::get() % X;
                A[i].second = RandInt32::get() % X;
            }

            auto ansMax = MaxHanhattanDistanceOfAllPairs::maxDistance2D(A);
            auto gtMax = maxManhattanDistanceSlow(A.data(), N);

            if (ansMax != gtMax) {
                cout << "Mismatched at " << __LINE__ << " : " << ansMax << ", " << gtMax << endl;
            }
            assert(ansMax == gtMax);
        }
    }
    {
        const int T = 10;
        const int N = 100;
        const int D = 5;
        const int X = 10000;
        for (int tt = 0; tt < T; tt++) {
            vector<vector<int>> A(N, vector<int>(D));
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < D; j++)
                    A[i][j] = RandInt32::get() % X;
            }

            auto ansMax = MaxHanhattanDistanceOfAllPairs::maxDistance(A, D);
            auto gtMax = maxManhattanDistanceSlow(A.data(), D, N);

            if (ansMax != gtMax) {
                cout << "Mismatched at " << __LINE__ << " : " << ansMax << ", " << gtMax << endl;
            }
            assert(ansMax == gtMax);
        }
    }
    {
        const int T = 10;
        const int N = 100;
        const int D = 5;
        const int X = 10000;
        for (int tt = 0; tt < T; tt++) {
            vector<array<int,D>> A(N);
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < D; j++)
                    A[i][j] = RandInt32::get() % X;
            }

            auto ansMax = MaxHanhattanDistanceOfAllPairs::maxDistance(A);
            auto gtMax = maxManhattanDistanceSlow(A.data(), N);

            if (ansMax != gtMax) {
                cout << "Mismatched at " << __LINE__ << " : " << ansMax << ", " << gtMax << endl;
            }
            assert(ansMax == gtMax);
        }
    }

    cout << "OK!" << endl;
}
