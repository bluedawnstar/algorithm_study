#pragma once

struct WeightedMedian {
    // 0 < W[i]
    // return (median, min weighted-distance-sum), O(NlogN)
    template <typename T>
    static pair<T, T> findWeightedMedian(const vector<T>& X, const vector<T>& W) {
        int N = int(X.size());
        T x0 = *min_element(X.begin(), X.end());

        vector<pair<T,T>> D(N);
        for (int i = 0; i < N; i++) {
            D[i].first = abs(X[i] - x0);
            D[i].second = W[i];
        }
        sort(D.begin(), D.end());

        vector<T> sumW(N + 1);
        vector<T> sumDW(N + 1);
        for (int i = 0; i < N; i++) {
            sumW[i + 1] = sumW[i] + D[i].second;
            sumDW[i + 1] = sumDW[i] + D[i].first * D[i].second;
        }

        T median, minDistSum = numeric_limits<T>::max();
        for (int i = 1; i < N; i++) {
            T w = sumW[i] * 2 - sumW[N];
            T dw = sumDW[N] - sumDW[i] * 2;
            T med = (w >= 0) ? D[i - 1].first : D[i].first;
            T dist = med * w + dw;
            if (dist < minDistSum) {
                minDistSum = dist;
                median = med;
            }
        }
        {
            T dist = D[0].first * -sumW[N] + sumDW[N];
            if (dist < minDistSum) {
                minDistSum = dist;
                median = D[0].first;
            }
        }
        {
            T dist = D[N - 1].first * sumW[N] - sumDW[N];
            if (dist < minDistSum) {
                minDistSum = dist;
                median = D[N - 1].first;
            }
        }

        return make_pair(median + x0, minDistSum);
    }
};
