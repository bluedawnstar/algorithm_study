#pragma once

// Lower envelope (simple version of PersistentUpperConvexHull)
template <typename T>
struct DynamicUpperConvexHull {
    vector<pair<T, T>>  points;

    DynamicUpperConvexHull(int N) {
        points.reserve(N + 1);
    }

    //PRECONDITION: x[i] < x[i + 1]
    void add(T x, T y) {
        if (points.empty()) {
            points.emplace_back(x, y);
            return;
        }

        auto base = make_pair(x, y);

        int lo = 0, hi = (int)points.size() - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (cw(base, points[mid + 1], points[mid]))
                hi = mid;
            else
                lo = mid + 1;
        }
        points.resize(lo + 1);
        points.emplace_back(x, y);
    }

    //PRECONDITION: x[i] < x[i + 1]
    // It's faster than add()
    void addLinear(T x, T y) {
        if (points.empty()) {
            points.emplace_back(x, y);
            return;
        }

        auto base = make_pair(x, y);
        while (points.size() > 1 && cw(base, points[points.size() - 1], points[points.size() - 2]))
            points.pop_back();

        points.emplace_back(x, y);
    }

    // find contact between the convex hull and a line with a slope
    // return points' indexes
    vector<int> queryWithSlope(T dx, T dy) const {
        vector<int> res;

        int lo = 0, hi = (int)points.size() - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;

            auto t = long long(points[mid].second - points[mid + 1].second) * dx - long long(points[mid].first - points[mid + 1].first) * dy;
            if (t == 0) {
                res.push_back(mid);
                res.push_back(mid + 1);
                break;
            } else if (t > 0)
                hi = mid;
            else
                lo = mid + 1;
        }
        if (res.empty())
            res.push_back(lo);

        return res;
    }

    // find contact between the convex hull and a line passing through a point
    // return points' indexes
    vector<int> queryWithPoint(T x, T y) const {
        vector<int> res;

        auto base = make_pair(x, y);

        int lo = 0, hi = (int)points.size() - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;

            auto t = cross(base, points[mid + 1], points[mid]);
            if (t == 0) {
                res.push_back(mid);
                res.push_back(mid + 1);
                break;
            } else if (t < 0)
                hi = mid;
            else
                lo = mid + 1;
        }
        if (res.empty())
            res.push_back(lo);

        return res;
    }

private:
    static long long cross(const pair<T, T>& base, const pair<T, T>& a, const pair<T, T>& b) {
        return long long(a.first - base.first) * long long(b.second - base.second) - long long(a.second - base.second) * long long(b.first - base.first);
    }

    static bool ccw(const pair<T, T>& base, const pair<T, T>& a, const pair<T, T>& b) {
        return cross(base, a, b) >= 0;
    }

    static bool cw(const pair<T, T>& base, const pair<T, T>& a, const pair<T, T>& b) {
        return cross(base, a, b) <= 0;
    }
};
