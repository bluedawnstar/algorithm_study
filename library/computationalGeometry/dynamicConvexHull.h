#pragma once

#if 0
//template <typename T>
typedef int T;
struct DynamicConvexHull {
    vector<pair<int, int>> upper;   // ordered by x coordinate (clockwise)
    vector<pair<int, int>> lower;   // ordered by x coordinate (counter-clockwise)

    void buildWithMutablePoints(vector<pair<int, int>>& points) {
        int j = 0, k = 0, n = int(points.size());
        sort(points.begin(), points.end());
        upper.resize(n);
        lower.resize(n);
        for (int i = 0; i < n; i++) {
            while (j >= 2 && cross(lower[j - 2], lower[j - 1], points[i]) <= 0) // cw?
                j--;
            while (k >= 2 && cross(upper[k - 2], upper[k - 1], points[i]) >= 0) // ccw?
                k--;
            lower[j++] = points[i];
            upper[k++] = points[i];
        }
        upper.resize(k);
        lower.resize(j);
    }

    void build(vector<pair<int, int>> points) {
        buildWithMutablePoints(points);
    }


    /*
       :   D    :
       +  +--+  +
       : /    \ :
       :/      \:
     B |   A    +
       +        | C
       :\      /:
       : \    / :
       +  +--+  +
       :   E    :
    */
    enum AreaT {
        aNone = -1, // empty
        aLeft,      // B
        aRight,     // C
        aUpper,     // D
        aLower,     // E
        aInner,     // A
        aBoundary,  // A
    };
    AreaT hitTest(int x, int y) const {
        if (lower.empty())
            return aNone;

        if (x < lower.front().first)
            return aLeft;   // B
        else if (x > lower.back().first)
            return aRight;  // C

        if (lower.size() == 1) {
            if (y < lower.front().second)
                return aLower;
            else if (y > lower.front().second)
                return aUpper;
            else
                return aBoundary;
        }

        if (x == lower.front().first) {
            if (y < lower.front().second)
                return aLower;

            int y2 = (upper[0].first != upper[1].first) ? upper[0].second : upper[1].second;
            if (y > y2)
                return aUpper;
            else
                return aBoundary;
        } else if (x == lower.back().first) {
            if (y > upper.back().second)
                return aUpper;

            int n = int(lower.size());
            int y1 = (lower[n - 2].first != lower[n - 1].first) ? lower[n - 1].second : lower[n - 2].second;
            if (y < y1)
                return aLower;
            else
                return aBoundary;
        } else {
            //TODO: ...

        }
    }


    void add(int x, int y) {
        //TODO: ...
    }

    // return square distance
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
};

#else

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
        return (b.second - a.second) * (c.first - b.first) - (c.second - b.second) * (b.first - a.first);
    }

    static T squareDistL2(const Vec2D<T>& p1, const Vec2D<T>& p2) {
        return (p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second);
    }

#if 0
    static bool inside(vector<Vec2D<T>> poly, Vec2D<T> p) {
        Vec2D<T> mid{ 0, 0 };

        int N = int(poly.size());

        p.first *= N;
        p.second *= N;
        for (int i = 0; i < N; i++) {
            mid.first += poly[i].first;
            mid.second += poly[i].second;
            poly[i].first *= N;
            poly[i].second *= N;
        }

        for (int i = 0, j; i < N; i++) {
            j = (i + 1) % N;
            T x1 = poly[i].first, x2 = poly[j].first;
            T y1 = poly[i].second, y2 = poly[j].second;

            T dy = y1 - y2;
            T dx = x2 - x1;
            T d = x1 * y2 - y1 * x2;

            if ((dy * mid.first + dx * mid.second + d) * (dy * p.first + dx * p.second + d) < 0)
                return false;
        }

        return true;
    }
#else
    static bool inside(const vector<Vec2D<T>>& polygon, Vec2D<T> pt) {
        int crossCnt = 0;
        for (int i = 0; i < int(polygon.size()); i++) {
            int j = (i + 1) % int(polygon.size());
            if ((polygon[i].second > pt.second) != (polygon[j].second > pt.second)) {
                double atX = double(polygon[j].first - polygon[i].first) * (pt.second - polygon[i].second) /
                    double(polygon[j].second - polygon[i].second) + polygon[i].first;
                if (pt.first < atX)
                    crossCnt++;
                //else if (pt.first == atX) // at boundary
                //    return true;
            }
        }

        return (crossCnt & 1) != 0;
    }
#endif
};
#endif