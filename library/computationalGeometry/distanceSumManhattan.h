#pragma once

// Manhattan distance = |x2 - x1 + |y2 - y1|
template <typename T = int, typename SumT = long long>
struct HanhattanDistanceSum {
    int N;
    vector<T> X;
    vector<T> Y;
    vector<SumT> sumX;
    vector<SumT> sumY;

    void build(const vector<pair<T, T>>& points) {
        N = (int)points.size();

        X = vector<T>(N);
        Y = vector<T>(N);
        for (int i = 0; i < N; i++) {
            X[i] = points[i].first;
            Y[i] = points[i].second;
        }
        sort(X.begin(), X.end());
        sort(Y.begin(), Y.end());

        sumX = vector<SumT>(N + 1);
        sumY = vector<SumT>(N + 1);
        for (int i = 1; i <= N; i++) {
            sumX[i] = sumX[i - 1] + X[i - 1];
            sumY[i] = sumY[i - 1] + Y[i - 1];
        }
    }

    SumT query(T x, T y) const {
        int cntDownX = lower_bound(X.begin(), X.end(), x) - X.begin();
        int cntDownY = lower_bound(Y.begin(), Y.end(), y) - Y.begin();

        SumT res = 0;
        res += SumT(cntDownX * 2 - N) * x + sumX[N] - sumX[cntDownX] * 2;
        res += SumT(cntDownY * 2 - N) * y + sumY[N] - sumY[cntDownY] * 2;

        return res;
    }
};
