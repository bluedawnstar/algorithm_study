#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>

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

    cout << "OK!" << endl;
}
