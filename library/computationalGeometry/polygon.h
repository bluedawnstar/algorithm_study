#pragma once

//--------- Polygon ---------------------------------------

// return 'real area' x 2
template <typename T>
T calcArea2(const vector<Vec2D<T>>& polygon) {
    T res = 0;

    if (polygon.size() > 2) {
        int last = (int)polygon.size() - 1;
        for (int i = 0; i < last; i++)
            res += polygon[i].x * polygon[i + 1].y - polygon[i + 1].x * polygon[i].y;
        res += polygon[last].x * polygon[0].y - polygon[0].x * polygon[last].y;
    }

    return res;
}

template <typename T>
bool isInside(const vector<Vec2D<T>>& polygon, Vec2D<T> pt) {
    int crossCnt = 0;
    for (int i = 0; i < (int)polygon.size(); i++) {
        int j = (i + 1) % (int)polygon.size();
        if ((polygon[i].y > pt.y) != (polygon[j].y > pt.y)) {
            double atX = double(polygon[j].x - polygon[i].x) * (pt.y - polygon[i].y) /
                double(polygon[j].y - polygon[i].y) + polygon[i].x;
            if (pt.x < atX)
                crossCnt++;
            //else if (pt.x == atX) // at boundary
            //    return true;
        }
    }

    return (crossCnt & 1) != 0;
}

// returning intersection between a polygon and the left side of AB vector
template <typename T>
vector<Vec2D<double>> cutPolygon(const vector<Vec2D<T>>& polygon, Vec2D<double> a, Vec2D<double> b) {
    int N = (int)polygon.size();
    vector<bool> inside(N, false);
    for (int i = 0; i < N; i++)
        inside[i] = cross(a, b, polygon[i]) >= 0;

    vector<Vec2D<double>> res;
    for (int i = 0; i < N; i++) {
        int j = (i + 1) % N;
        if (inside[i])
            res.push_back(polygon[i]);

        if (inside[i] != inside[j]) {
            Vec2D<double> cross;
            lineIntersection(polygon[i], polygon[j], a, b, cross);
            res.push_back(cross);
        }
    }

    return res;
}

// Sutherland-Hodgeman algorithm
template <typename T>
vector<Vec2D<double>> clipPolygon(const vector<Vec2D<T>>& polygon, T x1, T y1, T x2, T y2) {
    if (x1 > x2)
        swap(x1, x2);
    if (y1 > y2)
        swap(y1, y2);

    Vec2D<double> a(x1, y1), b(x2, y1), c(x2, y2), d(x1, y2);
    vector<Vec2D<double>> res = cutPolygon(polygon, a, b);
    res = cutPoly(res, b, c);
    res = cutPoly(res, c, d);
    res = cutPoly(res, d, a);

    return res;
}

template <typename T>
bool isPolygonIntersect(const vector<Vec2D<T>>& p, const vector<Vec2D<T>>& q) {
    int N = (int)p.size();
    int M = (int)q.size();

    if (isInside(p, q[0]) || isInside(q, p[0]))
        return false;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (isSegmentIntersect(p[i], p[(i + 1) % N], q[j], q[(j + 1) % M]))
                return true;
        }
    }

    return false;
}
