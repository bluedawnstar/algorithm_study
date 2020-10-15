#pragma once

// https://www.geeksforgeeks.org/maximum-manhattan-distance-between-a-distinct-pair-from-n-coordinates/?ref=rp

/*
  1. 2D

      |X[i] - X[j]| + |Y[i] - Y[j]|             (i != j)

        = max{( X[i] - X[j]) + ( Y[i] - Y[j]),
              ( X[i] - X[j]) + (-Y[i] + Y[j]),
              (-X[i] + X[j]) + ( Y[i] - Y[j]),
              (-X[i] + X[j]) + (-Y[i] + Y[j])}

        = max{( X[i] + Y[i]) - ( X[j] + Y[j]),
              ( X[i] - Y[i]) - ( X[j] - Y[j]),
              (-X[i] + Y[i]) - (-X[j] + Y[j]),
              (-X[i] - Y[i]) - (-X[j] - Y[j])}

        = max{ (X[i] + Y[i]) - (X[j] + Y[j]),
               (X[i] - Y[i]) - (X[j] - Y[j]),
              -(X[i] - Y[i]) + (X[j] - Y[j]),
              -(X[i] + Y[i]) + (X[j] + Y[j]) }

        = max{ (X[i] + Y[i]) - (X[j] + Y[j]),
               (X[j] + Y[j]) - (X[i] + Y[i]),
               (X[i] - Y[i]) - (X[j] - Y[j]),
               (X[j] - Y[j]) - (X[i] - Y[i]) }

        = max{ (X[i] + Y[i]) - (X[j] + Y[j]),
               (X[i] - Y[i]) - (X[j] - Y[j]) }
*/
struct MaxHanhattanDistanceOfAllPairs {
    // O(N)
    template <typename T>
    static T maxDistance2D(const pair<T, T> A[], int N) {
#if 0
        // O(NlogN)
        vector<T> add(N), sub(N);

        for (int i = 0; i < N; i++) {
            add[i] = A[i].first + A[i].second;
            sub[i] = A[i].first - A[i].second;
        }

        sort(add.begin(), add.end());
        sort(sub.begin(), sub.end());

        return max(add[N - 1] - add[0], sub[N - 1] - sub[0]);
#else
        T maxAdd = numeric_limits<T>::min();
        T minAdd = numeric_limits<T>::max();
        T maxSub = numeric_limits<T>::min();
        T minSub = numeric_limits<T>::max();

        for (int i = 0; i < N; i++) {
            auto add = A[i].first + A[i].second;
            auto sub = A[i].first - A[i].second;

            maxAdd = max(maxAdd, add);
            minAdd = min(minAdd, add);

            maxSub = max(maxSub, sub);
            minSub = min(minSub, sub);
        }

        return max(maxAdd - minAdd, maxSub - minSub);
#endif
    }

    template <typename T>
    static T maxDistance2D(const vector<pair<T, T>>& A) {
        return maxDistance2D(A.data(), int(A.size()));
    }

    //--- multi-dimension

    // O(2^(D-1)*N*logN)
    template <typename T>
    static T maxDistance(const vector<vector<T>>& A, int dimension) {
        int N = int(A.size());
        int K = 1 << (dimension - 1);
        vector<vector<T>> val(K, vector<T>(N));

        for (int i = 0; i < N; i++) {
            val[0][i] = accumulate(A[i].begin(), A[i].end(), T(0));
            for (int j = 1; j < K; j++) {
                int idx = ctz(j);
                val[j][i] = val[j & (j - 1)][i] - A[i][idx] * 2;
            }
        }

        for (int j = 0; j < K; j++)
            sort(val[j].begin(), val[j].end());

        T res = numeric_limits<T>::min();
        for (int j = 0; j < K; j++)
            res = max(res, val[j][N - 1] - val[j][0]);

        return res;
    }

    template <typename T, int D>
    static T maxDistance(const vector<array<T,D>>& A) {
        int N = int(A.size());
        int K = 1 << (D - 1);
        vector<vector<T>> val(K, vector<T>(N));

        for (int i = 0; i < N; i++) {
            val[0][i] = 0;
            for (int j = 0; j < D; j++)
                val[0][i] += A[i][j];

            for (int j = 1; j < K; j++) {
                int idx = ctz(j);
                val[j][i] = val[j & (j - 1)][i] - A[i][idx] * 2;
            }
        }

        for (int j = 0; j < K; j++)
            sort(val[j].begin(), val[j].end());

        T res = numeric_limits<T>::min();
        for (int j = 0; j < K; j++)
            res = max(res, val[j][N - 1] - val[j][0]);

        return res;
    }

private:
    // counting trailing zeros
    static int ctz(int x) {
        if (!x)
            return 32;
#ifndef __GNUC__
        return int(_tzcnt_u32(unsigned(x)));
#else
        return __builtin_ctz(unsigned(x));
#endif
    }
};
