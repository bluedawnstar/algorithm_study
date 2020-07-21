#pragma once

// integer polygon
struct IntPolygon {
    static long long cross(const pair<int, int>& base, const pair<int, int>& a, const pair<int, int>& b) {
        return 1ll * (a.first - base.first) * (b.second - base.second) - 1ll * (a.second - base.second) * (b.first - base.first);
    }

    static bool ccw(const pair<int, int>& base, const pair<int, int>& a, const pair<int, int>& b) {
        return cross(base, a, b) > 0;
    }

    static bool cw(const pair<int, int>& base, const pair<int, int>& a, const pair<int, int>& b) {
        return cross(base, a, b) < 0;
    }

    // The result is counter-clockwise, O(NlogN)
    // The starting point of the result is the bottom of the leftmost
    static vector<pair<int, int>> convexhull(vector<pair<int, int>>& P) {
        int N = int(P.size());
        if (N <= 1)
            return { P[0] };

        vector<pair<int, int>> res(N * 2);

        int k = 0;
        for (int i = 0; i < N; i++) {
            while (k >= 2 && !ccw(res[k - 2], res[k - 1], P[i]))
                k--;
            res[k++] = P[i];
        }
        for (int i = N - 2, t = k; i >= 0; i--) {
            while (k > t && !ccw(res[k - 2], res[k - 1], P[i]))
                k--;
            res[k++] = P[i];
        }

        res.resize(k - 1 - (res[0] == res[1]));
        return res;
    }

    static vector<int> convexhullIndirect(vector<pair<int, int>>& P) {
        int N = int(P.size());
        if (N <= 1)
            return { 0 };

        vector<int> res(N * 2);

        int k = 0;
        for (int i = 0; i < N; i++) {
            while (k >= 2 && cw(P[res[k - 2]], P[res[k - 1]], P[i]))
                k--;
            res[k++] = i;
        }
        for (int i = N - 2, t = k; i >= 0; i--) {
            while (k > t && cw(P[res[k - 2]], P[res[k - 1]], P[i]))
                k--;
            res[k++] = i;
        }

        res.resize(k - 1 - (res[0] == res[1]));
        return res;
    }

    static vector<pair<int,int>> convexhullIncludingBoundary(vector<pair<int, int>>& P) {
        int N = int(P.size());
        if (N <= 1)
            return { P[0] };

        vector<pair<int,int>> res(N * 2);

        int k = 0;
        for (int i = 0; i < N; i++) {
            while (k >= 2 && cw(res[k - 2], res[k - 1], P[i]))
                k--;
            res[k++] = P[i];
        }
        for (int i = N - 2, t = k; i >= 0; i--) {
            while (k > t && cw(res[k - 2], res[k - 1], P[i]))
                k--;
            res[k++] = P[i];
        }

        res.resize(k - 1 - (res[0] == res[1]));
        return res;
    }

    static vector<int> convexhullIndirectIncludingBoundary(vector<pair<int, int>>& P) {
        int N = int(P.size());
        if (N <= 1)
            return { 0 };

        vector<int> res(N * 2);

        int k = 0;
        for (int i = 0; i < N; i++) {
            while (k >= 2 && cw(P[res[k - 2]], P[res[k - 1]], P[i]))
                k--;
            res[k++] = i;
        }
        for (int i = N - 2, t = k; i >= 0; i--) {
            while (k > t && cw(P[res[k - 2]], P[res[k - 1]], P[i]))
                k--;
            res[k++] = i;
        }

        res.resize(k - 1 - (res[0] == res[1]));
        return res;
    }

    //---

    static bool isInPolygon(const vector<pair<int, int>>& polygon, int x, int y) {
        int n = int(polygon.size());
        for (int i = 1; i < n; i++) {
            if (1ll * (y - polygon[i - 1].second) * (polygon[i].first - polygon[i - 1].first) <=
                1ll * (x - polygon[i - 1].first) * (polygon[i].second - polygon[i - 1].second))
                return false;
        }
        if (1ll * (y - polygon[n - 1].second) * (polygon[0].first - polygon[n - 1].first) <=
            1ll * (x - polygon[n - 1].first) * (polygon[0].second - polygon[n - 1].second))
            return false;
        return true;
    }

    static bool isInPolygonIncludingBoundary(const vector<pair<int, int>>& polygon, int x, int y) {
        int n = int(polygon.size());
        for (int i = 1; i < n; i++) {
            if (1ll * (y - polygon[i - 1].second) * (polygon[i].first - polygon[i - 1].first) <
                1ll * (x - polygon[i - 1].first) * (polygon[i].second - polygon[i - 1].second))
                return false;
        }
        if (1ll * (y - polygon[n - 1].second) * (polygon[0].first - polygon[n - 1].first) <
            1ll * (x - polygon[n - 1].first) * (polygon[0].second - polygon[n - 1].second))
            return false;
        return true;
    }

    //---

    static long long calcArea2(const vector<pair<int,int>>& polygon) {
        long long res = 0;

        if (polygon.size() > 2) {
            int last = int(polygon.size()) - 1;
            for (int i = 0; i < last; i++)
                res += 1ll * polygon[i].first * polygon[i + 1].second - 1ll * polygon[i + 1].first * polygon[i].second;
            res += 1ll * polygon[last].first * polygon[0].second - 1ll * polygon[0].first * polygon[last].second;
        }

        return res;
    }
};
