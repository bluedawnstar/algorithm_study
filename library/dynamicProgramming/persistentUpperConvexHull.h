#pragma once

#include "../misc/jumpPointerSparseTable.h"

// Fully Persistent Convex Hull - lower envelope
template <typename T>
struct PersistentUpperConvexHull {
    vector<pair<T, T>>      points; // the right most point at each index
    vector<int>             length; // the number of convex hull points at each index
    vector<int>             next;   // index of next point in a upper or lower convex hull at each index

    JumpPointerSparseTable  index;

    PersistentUpperConvexHull(int N) : index(N) {
        reserve(N);
    }

    void reserve(int capacity) {
        points.reserve(capacity);
        next.reserve(capacity);
        length.reserve(capacity);
    }

    pair<T, T> get(int index) const {
        return points[index];
    }

    //PRECONDITION: x[i] < x[i + 1]
    // return convex ID (the index to points and next)
    int add(T x, T y) {
        return add((int)points.size() - 1, x, y);        
    }

    //PRECONDITION: x[i] < x[i + 1]
    // return convex ID (the index to points and next)
    // -1 <= convexID (old convex ID)
    int add(int convexID, T x, T y) {
        int idx = (int)points.size();
        points.emplace_back(x, y);
        next.push_back(convexID);

        if (idx == 0 || convexID < 0) {
            length.push_back(1);
            index.add(idx, -1);
            return idx;
        }

        auto base = make_pair(x, y);

        int lo = 0, hi = length[convexID] - 1;
        int loIdx = convexID;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            int a = index.query(convexID, mid);
            int b = next[a];

            if (cw(base, points[a], points[b])) {
                lo = mid + 1;
                loIdx = b;
            } else {
                hi = mid;
            }
        }

        next[idx] = loIdx;
        length.push_back(length[loIdx] + 1);

        index.add(idx, loIdx);

        return idx;
    }

    //PRECONDITION: x[i] < x[i + 1]
    // return convex ID (the index to points and next)
    // It's faster than add()
    int addLinear(T x, T y) {
        return addLinear((int)points.size() - 1, x, y);
    }

    //PRECONDITION: x[i] < x[i + 1]
    // return convex ID (the index to points and next)
    // It's faster than add()
    // -1 <= convexID (old convex ID)
    int addLinear(int convexID, T x, T y) {
        int idx = (int)points.size();
        points.emplace_back(x, y);
        next.push_back(convexID);

        if (idx == 0 || convexID < 0) {
            length.push_back(1);
            index.add(idx, -1);
            return idx;
        }

        auto base = make_pair(x, y);

        int loIdx = convexID;
        while (next[loIdx] >= 0 && cw(base, points[loIdx], points[next[loIdx]]))
            loIdx = next[loIdx];

        next[idx] = loIdx;
        length.push_back(length[loIdx] + 1);

        index.add(idx, loIdx);

        return idx;
    }

    // find contact between the convex hull and a line with a slope
    vector<int> queryWithSlope(int convexID, T dx, T dy) const {
        vector<int> res;

        int lo = 0, hi = length[convexID] - 1;
        int loIdx = convexID;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            int a = index.query(convexID, mid);
            int b = next[a];

            auto t = long long(points[b].second - points[a].second) * dx - long long(points[b].first - points[a].first) * dy;
            if (t == 0) {
                res.push_back(a);
                res.push_back(b);
                break;
            } else if (t > 0) {
                lo = mid + 1;
                loIdx = b;
            } else {
                hi = mid;
            }
        }
        if (res.empty())
            res.push_back(loIdx);

        return res;
    }

    // find contact between the convex hull and a line passing through a point
    vector<int> queryWithPoint(int convexID, T x, T y) const {
        vector<int> res;

        auto base = make_pair(x, y);

        int lo = 0, hi = length[convexID] - 1;
        int loIdx = convexID;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            int a = index.query(convexID, mid);
            int b = next[a];

            auto t = cross(base, points[a], points[b]);
            if (t == 0) {
                res.push_back(a);
                res.push_back(b);
                break;
            } else if (t < 0) {
                lo = mid + 1;
                loIdx = b;
            } else {
                hi = mid;
            }
        }
        if (res.empty())
            res.push_back(loIdx);

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
