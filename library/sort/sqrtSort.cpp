#include <vector>
#include <algorithm>

#include "hilbertOrder.h"

using namespace std;

// It sorts queries for MO's algorithm (Query Square Root Decomposition)
// This function sorts (L, R) pairs.
template <typename T>
void sortSqrt(vector<pair<T, T>>& vec, const int blockSize) {
    sort(vec.begin(), vec.end(), [blockSize](const auto& x, const auto& y) {
        if (x.first / blockSize != y.first / blockSize)
            return x.first / blockSize < y.first / blockSize;
        return x.second < y.second;
    });
}

// It sorts queries for MO's algorithm (Query Square Root Decomposition)
// This function sorts ((L, R), value) pairs.
template <typename T, typename U>
void sortSqrt(vector<pair<pair<T, T>, U>>& vec, const int blockSize) {
    sort(vec.begin(), vec.end(), [blockSize](const auto& x, const auto& y) {
        if (x.first.first / blockSize != y.first.first / blockSize)
            return x.first.first / blockSize < y.first.first / blockSize;
        return x.first.second < y.first.second;
    });
}

//--- better performance

// It sorts queries for MO's algorithm (Query Square Root Decomposition)
// This function sorts (L, R) pairs.
template <typename T>
void sortSqrtFast(vector<pair<T, T>>& vec, const int blockSize) {
    sort(vec.begin(), vec.end(), [blockSize](const auto& x, const auto& y) {
        if (x.first / blockSize != y.first / blockSize)
            return x < y;
        return ((x.first / blockSize) & 1) ? (x.second < y.second) : (x.second > y.second);
    });
}

// It sorts queries for MO's algorithm (Query Square Root Decomposition)
// This function sorts ((L, R), value) pairs.
template <typename T, typename U>
void sortSqrtFast(vector<pair<pair<T, T>, U>>& vec, const int blockSize) {
    sort(vec.begin(), vec.end(), [blockSize](const auto& x, const auto& y) {
        if (x.first.first / blockSize != y.first.first / blockSize)
            return x.first < y.first;
        return ((x.first.first / blockSize) & 1) ? (x.first.second < y.first.second) : (x.first.second > y.first.second);
    });
}


/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/rand.h"

void testSortSqrt() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- sort SQRT (Query Square Root Decomposition) ---------" << endl;
    {
        vector<pair<int, int>> v;
        vector<pair<pair<int, int>, int>> v2;
        int MAXN = 100;
        int queryN = 50;
        for (int i = 0; i < queryN; i++) {
            int l = RandInt32::get() % MAXN + 1;
            int r = RandInt32::get() % MAXN + 1;
            if (l > r)
                swap(l, r);
            v.push_back(make_pair(l, r)); v2.push_back(make_pair(make_pair(l, r), i));
        }

        sortSqrt(v, int(sqrt(MAXN)));
        sortSqrt(v2, int(sqrt(MAXN)));

        for (int i = 0; i < queryN; i++) {
            assert(v[i] == v2[i].first);
            cout << v[i] << endl;
        }
    }
    {
        vector<pair<int, int>> v;
        vector<pair<pair<int, int>, int>> v2;
        int MAXN = 100;
        int queryN = 50;
        for (int i = 0; i < queryN; i++) {
            int l = RandInt32::get() % MAXN + 1;
            int r = RandInt32::get() % MAXN + 1;
            if (l > r)
                swap(l, r);
            v.push_back(make_pair(l, r)); v2.push_back(make_pair(make_pair(l, r), i));
        }

        sortSqrtFast(v, int(sqrt(MAXN)));
        sortSqrtFast(v2, int(sqrt(MAXN)));

        for (int i = 0; i < queryN; i++) {
            assert(v[i] == v2[i].first);
            cout << v[i] << endl;
        }
    }
}
