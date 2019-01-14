#pragma once

template <typename T>
struct DelaunayTriangulation {
    struct Triangle {
        int p[3];                               // index

        Triangle() {
            memset(p, 0, sizeof(p));
        }

        Triangle(int ai, int bi, int ci) {
            p[0] = ai;
            p[1] = bi;
            p[2] = ci;
        }

        Triangle(const Triangle& rhs) {
            operator =(rhs);
        }

        Triangle& operator =(const Triangle& rhs) {
            if (this != &rhs)
                memcpy(p, rhs.p, sizeof(p));
            return *this;
        }
    };

    vector<Vec2D<T>> points;
    vector<Triangle> triangles;

    explicit DelaunayTriangulation(T INF) {
        points.push_back(Vec2D<T>{ 0, INF });
        points.push_back(Vec2D<T>{ -INF, -INF });
        points.push_back(Vec2D<T>{ INF, -INF });

        triangles.push_back(Triangle(0, 1, 2));
    }

    // O(N)
    void add(T x, T y) {
        Vec2D<T> pt{ x, y };
        points.push_back(pt);

        int tri = searchCoverTriangle(pt);
        while (tri == -1) {
            for (int i = 0; i < 3; i++) {
                points[i] *= 2;         // extend super triangle's size
                points[i] *= 2;         // extend super triangle's size
            }
            tri = searchCoverTriangle(pt);
        }

        Triangle tr = triangles[tri];
        triangles.erase(triangles.begin() + tri);

        for (int i = 0; i < 3; i++)
            doTriangulation(tr.p[i], tr.p[(i + 1) % 3], int(points.size()) - 1);
    }

    vector<Triangle> getTriangles(bool excludeSuperTriangle = true) const {
        vector<Triangle> res;
        res.reserve(triangles.size());
        for (auto& tr : triangles) {
            if (excludeSuperTriangle && (tr.p[0] < 3 || tr.p[1] < 3 || tr.p[2] < 3))
                continue;
            res.push_back(tr);
        }
        return res;
    }

    pair<Vec2D<double>, double> getCircumcircleCenter(const Triangle& tr) const {
        return DelaunayTriangulation<T>::getCircumcircleCenter(points[tr.p[0]], points[tr.p[1]], points[tr.p[2]]);
    }

private:
    void doTriangulation(int edgeP1, int edgeP2, int pi) {
        auto adjacent = searchAdjacentTriangle(edgeP2, edgeP1); // (triangle index, edge order)
        if (adjacent.first != -1) {
            if (inCircumcircle(triangles[adjacent.first], points[pi])) {
                Triangle adjTri = triangles[adjacent.first];
                triangles.erase(triangles.begin() + adjacent.first);

                doTriangulation(adjTri.p[adjacent.second], adjTri.p[(2 + adjacent.second) % 3], pi);
                doTriangulation(adjTri.p[(1 + adjacent.second) % 3], adjTri.p[(2 + adjacent.second) % 3], pi);
                return;
            }
        }

        if (calcArea2(points[pi], points[edgeP1], points[edgeP2]) >= 0)
            triangles.push_back(Triangle(pi, edgeP1, edgeP2));
        else
            triangles.push_back(Triangle(pi, edgeP2, edgeP1));
    }

    bool isIn(const Triangle& tr, const Vec2D<T>& pt) const {
        if (calcArea2(pt, points[tr.p[0]], points[tr.p[1]]) < 0)
            return false;
        if (calcArea2(pt, points[tr.p[1]], points[tr.p[2]]) < 0)
            return false;
        if (calcArea2(pt, points[tr.p[2]], points[tr.p[0]]) < 0)
            return false;

        return true;
    }

    int searchCoverTriangle(const Vec2D<T>& pt) const {
        for (int i = int(triangles.size()) - 1; i >= 0; i--) {
            if (isIn(triangles[i], pt))
                return i;
        }
        return -1;
    }

    // return (triangle index, edge order)
    // edge order: 0-1: 0, 1-2: 1, 2-0: 2
    pair<int,int> searchAdjacentTriangle(int p1, int p2) const {
        for (int i = int(triangles.size()) - 1; i >= 0; i--) {
            for (int j = 0; j < 3; j++) {
                if (triangles[i].p[j] == p1 && triangles[i].p[(j + 1) % 3] == p2)
                    return make_pair(i, j);

                if (triangles[i].p[j] == p2 && triangles[i].p[(j + 1) % 3] == p1)
                    return make_pair(i, j);
            }
        }
        return make_pair(-1, -1);
    }

    bool inCircumcircle(const Triangle& tr, const Vec2D<T>& pt) const {
        return points[tr.p[0]].norm2() * calcArea2(points[tr.p[1]], points[tr.p[2]], pt)
             - points[tr.p[1]].norm2() * calcArea2(points[tr.p[0]], points[tr.p[2]], pt)
             + points[tr.p[2]].norm2() * calcArea2(points[tr.p[0]], points[tr.p[1]], pt)
             - pt.norm2() * calcArea2(points[tr.p[0]], points[tr.p[1]], points[tr.p[2]]) > 0;
    }


    //  > 0 : ccw, == 0 : line, < 0 : cw
    static double calcArea2(const Vec2D<T>& p1, const Vec2D<T>& p2, const Vec2D<T>& p3) {
        return (p2.first - p1.first) * (p3.second - p1.second) - (p2.second - p1.second) * (p3.first - p1.first);
    }

    // return (center, radius^2)
    static pair<Vec2D<double>, double> getCircumcircleCenter(const Vec2D<T>& p0, const Vec2D<T>& p1, const Vec2D<T>& p2) {
        pair<Vec2D<double>, double> res;

        double ax = p1.first - p0.first, ay = p1.second - p0.second;
        double bx = p2.first - p0.first, by = p2.second - p0.second;
        double t = (1.0 * p1.first * p1.first - 1.0 * p0.first * p0.first + 1.0 * p1.second * p1.second - 1.0 * p0.second * p0.second);
        double u = (1.0 * p2.first * p2.first - 1.0 * p0.first * p0.first + 1.0 * p2.second * p2.second - 1.0 * p0.second * p0.second);

        double s = 1.0 / (2.0 * (ax * by - ay * bx));

        res.first.first = ((p2.second - p0.second) * t + (p0.second - p1.second) * u) * s;
        res.first.second = ((p0.first - p2.first) * t + (p1.first - p0.first) * u) * s;

        double dx = p0.first - res.first.first;
        double dy = p0.second - res.first.second;
        
        res.second = dx * dx + dy * dy;

        return res;
    }
};
