#pragma once

// based on Graham Andrew

// The results is sorted by X, O(NlogN)
struct ConvexHull {
    vector<pair<int, int>> lower;   // (x0, y0), ..., (xi, yi), ..., (xn, yn)
    vector<pair<int, int>> upper;   // (x0, y0), ..., (xj, yj), ..., (xn, yn)

    // PRECONDITION: points must be sorted by X coordinate
    void buildSorted(const vector<pair<int, int>>& points) {
        int j = 0, k = 0, n = int(points.size());
        upper.resize(n);
        lower.resize(n);
        for (int i = 0; i < n; i++) {
            while (j >= 2 && cross(lower[j - 2], lower[j - 1], points[i]) <= 0)
                j--;
            while (k >= 2 && cross(upper[k - 2], upper[k - 1], points[i]) >= 0)
                k--;
            lower[j++] = points[i];
            upper[k++] = points[i];
        }
        upper.resize(k);
        lower.resize(j);
    }

    // PRECONDITION: points must be sorted by X coordinate
    void buildLowerSorted(const vector<pair<int, int>>& points) {
        int j = 0, n = int(points.size());
        lower.resize(n);
        for (int i = 0; i < n; i++) {
            while (j >= 2 && cross(lower[j - 2], lower[j - 1], points[i]) <= 0)
                j--;
            lower[j++] = points[i];
        }
        lower.resize(j);
    }

    // PRECONDITION: points must be sorted by X coordinate
    void buildUpperSorted(const vector<pair<int, int>>& points) {
        int k = 0, n = int(points.size());
        upper.resize(n);
        for (int i = 0; i < n; i++) {
            while (k >= 2 && cross(upper[k - 2], upper[k - 1], points[i]) >= 0)
                k--;
            upper[k++] = points[i];
        }
        upper.resize(k);
    }


    // O(NlogN)
    void build(vector<pair<int, int>>& points) {
        sort(points.begin(), points.end());
        buildSorted(points);
    }

    void buildLower(vector<pair<int, int>>& points) {
        sort(points.begin(), points.end());
        buildLowerSorted(points);
    }

    void buildUpper(vector<pair<int, int>>& points) {
        sort(points.begin(), points.end());
        buildUpperSorted(points);
    }


    // PRECONDITION: points must be sorted by X coordinate
    void mergeLowerSorted(const vector<pair<int, int>>& points) {
        mergeTo(lower, points);
        buildLowerSorted(lower);
    }

    // PRECONDITION: points must be sorted by X coordinate
    void mergeUpperSorted(const vector<pair<int, int>>& points) {
        mergeTo(upper, points);
        buildUpperSorted(upper);
    }

    void merge(const ConvexHull& rhs) {
        mergeLowerSorted(rhs.lower);
        mergeUpperSorted(rhs.upper);
    }


    long long calcMaxDiameter() const {
        int j = 0, k, m;
        k = int(lower.size()) - 1;
        m = int(upper.size()) - 1;
        long long dist = 0;
        while (j < m || k > 0) {
            dist = max(dist, squareDist(upper[j], lower[k]));
            if (j == m)
                k--;
            else if (k == 0)
                j++;
            else {
                if (1ll * (upper[j + 1].second - upper[j].second) * (lower[k].first - lower[k - 1].first) >
                    1ll * (lower[k].second - lower[k - 1].second) * (upper[j + 1].first - upper[j].first))
                    j++;
                else
                    k--;
            }
        }
        return dist;
    }

private:
    static long long cross(const pair<int, int>& P, const pair<int, int>& Q, const pair<int, int>& R) {
        return 1ll * (Q.first - P.first) * (R.second - P.second) - 1ll * (R.first - P.first) * (Q.second - P.second);
    }

    static long long squareDist(const pair<int, int>& L, const pair<int, int>& R) {
        long long x = L.first - R.first;
        long long y = L.second - R.second;
        return x * x + y * y;
    }

    static void mergeTo(vector<pair<int, int>>& dst, const vector<pair<int, int>>& src) {
        int i = int(dst.size()) - 1;
        int j = int(src.size()) - 1;

        dst.resize(dst.size() + src.size());

        int k = int(dst.size()) - 1;
        while (i >= 0 && j >= 0) {
            if (dst[i] > src[j])
                dst[k--] = dst[i--];
            else
                dst[k--] = src[j--];
        }

        if (j >= 0)
            memcpy(&dst[0], &src[0], sizeof(src[0]) * (j + 1));
    }
};
