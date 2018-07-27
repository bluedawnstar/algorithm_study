#pragma once

template <typename T>
struct DynamicConvexHull {
    // <PRECONDITION>
    //  - Convex hull points are stored in counter-clockwise order
    // 
    // O(H)
    // return true if convex hull is changed
    static bool addPoint(vector<Vec2D<T>>& hull, Vec2D<T> p) {
        if (inside(hull, p))
            return false;

        // point having minimum distance from the point p
        int idx = 0;
        int N = int(hull.size());
        for (int i = 1; i < N; i++) {
            if (squareDistL2(p, hull[i]) < squareDistL2(p, hull[idx]))
                idx = i;
        }

        // find the upper tangent
        int up = idx;
        while (orientation(p, hull[up], hull[(up + 1) % N]) >= 0)
            up = (up + 1) % N;

        // find the lower tangent
        int low = idx;
        while (orientation(p, hull[low], hull[(N + low - 1) % N]) <= 0)
            low = (N + low - 1) % N;

        vector<Vec2D<T>> res;

        int curr = up;
        res.push_back(hull[curr]);
        while (curr != low) {
            curr = (curr + 1) % N;
            res.push_back(hull[curr]);
        }
        res.push_back(p);

        swap(hull, res);

        return true;
    }

private:
    // checks whether the point crosses the convex hull or not
    static T orientation(const Vec2D<T>& a, const Vec2D<T>& b, const Vec2D<T>& c) {
        return (b.y - a.y) * (c.x - b.x) - (c.y - b.y) * (b.x - a.x);
    }

    static T squareDistL2(const Vec2D<T>& p1, const Vec2D<T>& p2) {
        return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
    }

    static bool inside(vector<Vec2D<T>> poly, Vec2D<T> p) {
        Vec2D<T> mid{ 0, 0 };

        int N = int(poly.size());

        p.x *= N;
        p.y *= N;
        for (int i = 0; i < N; i++) {
            mid.x += poly[i].x;
            mid.y += poly[i].y;
            poly[i].x *= N;
            poly[i].y *= N;
        }

        for (int i = 0, j; i < N; i++) {
            j = (i + 1) % N;
            T x1 = poly[i].x, x2 = poly[j].x;
            T y1 = poly[i].y, y2 = poly[j].y;

            T dy = y1 - y2;
            T dx = x2 - x1;
            T d = x1 * y2 - y1 * x2;

            if ((dy * mid.x + dx * mid.y + d) * (dy * p.x + dx * p.y + d) < 0)
                return false;
        }

        return true;
    }
};