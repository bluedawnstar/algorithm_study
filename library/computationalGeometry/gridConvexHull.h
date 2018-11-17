#pragma once


/*
  ex)

        *
        ....*
   *......*..
     ..*.........*
     .*........*
     *...
        .
        *
*/

template <typename T>
struct GridConvexHull {
    vector<pair<T, T>> L, R;
    T minX, maxX;
    T minY, maxY;

    GridConvexHull() {
    }

    explicit GridConvexHull(const vector<pair<T, T>>& points) {
        build(points);
    }

    // (x, y)
    void build(const vector<pair<T, T>>& points) {
        int N = int(points.size());

        minX = numeric_limits<T>::max();
        maxX = numeric_limits<T>::min();
        minY = numeric_limits<T>::max();
        maxY = numeric_limits<T>::min();

        vector<pair<T, T>> p = points;
        for (int i = 0; i < N; i++) {
            minX = min(minX, p[i].first);
            maxX = max(maxX, p[i].first);
            minY = min(minY, p[i].second);
            maxY = max(maxY, p[i].second);
        }

        auto compY = [](const pair<T, T>& l, const pair<T, T>& r) {
            return l.second < r.second;
        };
        sort(p.begin(), p.end(), compY);

        L.clear();
        R.clear();
        L.push_back(p[0]);
        R.push_back(p[0]);
        for (int i = 1; i < N; i++) {
            if (L.back().second == p[i].second) {
                if (L.back().first > p[i].first)
                    L.back() = p[i];
            } else {
                if (L.back().first >= p[i].first)
                    L.push_back(p[i]);
            }

            if (R.back().second == p[i].second) {
                if (R.back().first < p[i].first)
                    R.back() = p[i];
            } else {
                if (R.back().first <= p[i].first)
                    R.push_back(p[i]);
            }
        }

        for (int i = int(upper_bound(p.begin(), p.end(), L.back(), compY) - p.begin()); i < N; i++) {
            while (L.back().first > minX && L.back().first > p[i].first)
                L.pop_back();
            L.push_back(p[i]);
        }
        for (int i = int(upper_bound(p.begin(), p.end(), R.back(), compY) - p.begin()); i < N; i++) {
            while (R.back().first < maxX && R.back().first < p[i].first)
                R.pop_back();
            R.push_back(p[i]);
        }
    }

    // including bounding
    long long countInnerPoints() const {
        long long res = 0;

        int x = R[0].first;
        int y = R[0].second;
        for (int i = 1; i < int(R.size()); i++) {
            res += 1ll * (min(x, R[i].first) - minX + 1) * (R[i].second - y);
            x = R[i].first;
            y = R[i].second;
        }
        res += maxX - minX + 1; // it is required

        x = L[0].first;
        y = L[0].second;
        for (int i = 1; i < int(L.size()); i++) {
            res -= 1ll * (max(x, L[i].first) - minX) * (L[i].second - y);
            x = L[i].first;
            y = L[i].second;
        }

        return res;
    }
};
