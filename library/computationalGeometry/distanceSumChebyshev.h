#pragma once

// Chebyshev distance = max(|x2 - x1, |y2 - y1|)
template <typename T = int, typename SumT = long long>
struct ChebyshevDistanceSum {
    int N;
    vector<T> X;        // y - x
    vector<T> Y;        // y + x
    vector<SumT> sumX;
    vector<SumT> sumY;

    void build(const vector<pair<T, T>>& points) {
        N = int(points.size());

        X = vector<T>(N);
        Y = vector<T>(N);
        for (int i = 0; i < N; i++) {
            X[i] = points[i].second - points[i].first;
            Y[i] = points[i].second + points[i].first;
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
        T rotX = y - x;
        T rotY = y + x;

        int cntDownX = int(lower_bound(X.begin(), X.end(), rotX) - X.begin());
        int cntDownY = int(lower_bound(Y.begin(), Y.end(), rotY) - Y.begin());

        SumT res = 0;
        res += SumT(cntDownX * 2 - N) * rotX + sumX[N] - sumX[cntDownX] * 2;
        res += SumT(cntDownY * 2 - N) * rotY + sumY[N] - sumY[cntDownY] * 2;

        return res / 2;
    }
};
