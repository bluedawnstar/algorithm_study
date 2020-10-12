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
};
