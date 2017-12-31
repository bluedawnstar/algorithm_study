#pragma once

// <PRECONDITION>
//  - 'Polygon' must be a convex polygon.
//  - 'Polygon' must be stored by counter-clockwise order
//
// Rotating Calipers : O(N)
template <typename T>
inline double calcPolygonDiameter(const vector<Vec2D<T>>& polygon) {
    int N = (int)polygon.size();

    int left = min_element(polygon.begin(), polygon.end()) - polygon.begin();
    int right = max_element(polygon.begin(), polygon.end()) - polygon.begin();

    Vec2D<double> calipersA{ 0, 1 };
    double res = (polygon[right] - polygon[left]).norm();               // calculate 

    vector<Vec2D<double>> toNext(N);
    for (int i = 0; i < N; i++)
        toNext[i] = (polygon[(i + 1) % N] - polygon[i]).normalize();

    int a = left, b = right;
    while (a != right || b != left) {
        double cosThetaA = calipersA.dot(toNext[a]);
        double cosThetaB = -calipersA.dot(toNext[b]);
        if (cosThetaA > cosThetaB) { // thetaA < thetaB
            calipersA = toNext[a];
            a = (a + 1) % N;
        } else {
            calipersA = -toNext[b];
            b = (b + 1) % N;
        }
        res = max(res, (polygon[a] - polygon[b]).norm());               // calculate
    }

    return res;
}