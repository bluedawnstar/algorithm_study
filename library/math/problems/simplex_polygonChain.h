#pragma once

#include "../simplex.h"

// https://www.codechef.com/problems/POLCHAIN
// - https://discuss.codechef.com/t/polchain-editorial/53537

struct PolygonChainSolver {
    struct Polygon {
        vector<pair<int, int>> points;  // counterclockwise order

        void addPoint(int x, int y) {
            points.emplace_back(x, y);
        }

        double calcArea2() const {
            double res = 0;

            int N = int(points.size());
            if (N > 2) {
                int last = N - 1;
                for (int i = 0; i < last; i++)
                    res += 1.0 * points[i].first * points[i + 1].second - 1.0 * points[i + 1].first * points[i].second;
                res += 1.0 * points[last].first * points[0].second - 1.0 * points[0].first * points[last].second;
            }

            return abs(res);
        }
    };

    static double solve(const vector<Polygon>& polygons) {
        int N = int(polygons.size());

        vector<double> polygonArea;
        for (int i = 0; i < N; i++)
            polygonArea.emplace_back(polygons[i].calcArea2());

        vector<int> perm(N);
        iota(perm.begin(), perm.end(), 0);
        sort(perm.begin(), perm.end(), [&polygonArea](int a, int b) {
            return polygonArea[a] < polygonArea[b];
        });

        /*
            P = { P[0], P[1], P[2], ... }
            Q = { Q[0], Q[1], P[2], ... }

            v = (P[k].x - Q[j].x, P[k].y - Q[j].y)
            w = (Q[j+1].x - Q[j].x, Q[j+1].y - Q[j].y)

            v X w <= 0

            <=> (v.x + (R[i] - L[i]) - (R[i+1] - L[i+1]))*w.y - (v.y + (U[i] - D[i]) - (U[i+1] - D[i+1]))*w.x <= 0

            <=> (R[i] - R[i+1] - L[i] + L[i+1])*w.y - (U[i] - U[i+1] - D[i] + D[i+1])*w.x <= v.y * w.x - v.x * w.y

            <=>  R[i  ]*w.y - L[i  ]*w.y - U[i  ]*w.x + D[i  ]*w.x
               - R[i+1]*w.y + L[i+1]*w.y + U[i+1]*w.x - D[i+1]*w.x <= v.y * w.x - v.x * w.y

                N-1
            MIN SUM (R[i] + L[i] + U[i] + D[i])
                i=0

                    N-1
            = - MAX SUM (-R[i] - L[i] - U[i] - D[i])
                    i=0

           ------------------------

                N-1
            m = SUM size(polygon[i]) * size(polygon[i - 1])
                i=1

            A[m][4*N], b[m], c[4*N]

                    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15 ...  4*N-1
                   R0  L0  U0  D0  R1  L1  U1  D1  R2  L2  U2  D2  R4  L4  U4  D4 ...  DN-1
            A = |  wy -wy -wx  wx -wy  wy  wx -wx   0   0   0   0   0   0   0   0 ...   0 |  b = | vy * wx - vx * wy |
                |  wy -wy -wx  wx -wy  wy  wx -wx   0   0   0   0   0   0   0   0 ...   0 |      | vy * wx - vx * wy |
                |  ...   ...   ...   ...   ...    ...                                     |      |     ...           |
                |  wy -wy -wx  wx -wy  wy  wx -wx   0   0   0   0   0   0   0   0 ...   0 |      | vy * wx - vx * wy |
                |  wy -wy -wx  wx -wy  wy  wx -wx   0   0   0   0   0   0   0   0 ...   0 |      | vy * wx - vx * wy |
                |   0   0   0   0  wy -wy -wx  wx -wy  wy  wx -wx   0   0   0   0  ...  0 |      | vy * wx - vx * wy |
                |   0   0   0   0  wy -wy -wx  wx -wy  wy  wx -wx   0   0   0   0  ...  0 |      | vy * wx - vx * wy |
                |  ...   ...   ...   ...   ...    ...                                     |      |     ...           |
                |   0   0   0   0  wy -wy -wx  wx -wy  wy  wx -wx   0   0   0   0  ...  0 |      | vy * wx - vx * wy |
                |   0   0   0   0  wy -wy -wx  wx -wy  wy  wx -wx   0   0   0   0  ...  0 |      | vy * wx - vx * wy |
                |  ...   ...   ...   ...   ...    ...                                     |      |     ...           |
                |   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0 ... -wx |      | vy * wx - vx * wy |

                   0  1  2 ... m-2 m-1
            c = | -1 -1 -1 ...  -1  -1 |
        */

        vector<vector<double>> A;
        vector<double> b;
        for (int i = 1; i < N; i++) {
            auto& poly0 = polygons[perm[i - 1]];
            auto& poly = polygons[perm[i]];
            //constraints between p[i] and p[i+1]
            int size = int(poly.points.size());
            for (int j = 0; j < size; j++) {
                int wx = poly.points[(j + 1) % size].first - poly.points[j].first;
                int wy = poly.points[(j + 1) % size].second - poly.points[j].second;
                //process half-plane of side j of p[i+1]
                for (int k = 0; k < int(poly0.points.size()); k++) {
                    //make sure point k is in the half-plane
                    int vx = poly0.points[k].first - poly.points[j].first;
                    int vy = poly0.points[k].second - poly.points[j].second;
                    //add constraint for the point and half-plane
                    vector<double> a(4 * N);
                    a[4 * (i - 1)] = wy;
                    a[4 * (i - 1) + 1] = -wy;
                    a[4 * (i - 1) + 2] = -wx;
                    a[4 * (i - 1) + 3] = wx;
                    a[4 * (i)] = -wy;
                    a[4 * (i)+1] = wy;
                    a[4 * (i)+2] = wx;
                    a[4 * (i)+3] = -wx;
                    A.push_back(a);
                    b.push_back(vy * wx - vx * wy);
                }
            }
        }
        vector<double> c(4 * N, -1);

        vector<double> x;
        auto res = Simplex(A, b, c).solve(x);
        if (res == numeric_limits<double>::infinity() || res == -numeric_limits<double>::infinity())
            return -1;
        return -res;
    }

    static double solve2(const vector<Polygon>& polygons) {
        int N = int(polygons.size());

        vector<double> polygonArea;
        for (int i = 0; i < N; i++)
            polygonArea.emplace_back(polygons[i].calcArea2());

        vector<int> perm(N);
        iota(perm.begin(), perm.end(), 0);
        sort(perm.begin(), perm.end(), [&polygonArea](int a, int b) {
            return polygonArea[a] < polygonArea[b];
        });

        /*
            P = { P[0], P[1], P[2], ... }
            Q = { Q[0], Q[1], P[2], ... }

            v = (P[k].x - Q[j].x, P[k].y - Q[j].y)
            w = (Q[j+1].x - Q[j].x, Q[j+1].y - Q[j].y)

            v X w <= 0

            <=> (v.x + (R[i] - L[i]) - (R[i+1] - L[i+1]))*w.y - (v.y + (U[i] - D[i]) - (U[i+1] - D[i+1]))*w.x <= 0

            <=> (R[i] - R[i+1] - L[i] + L[i+1])*w.y - (U[i] - U[i+1] - D[i] + D[i+1])*w.x <= v.y * w.x - v.x * w.y

            <=>  R[i  ]*w.y - L[i  ]*w.y - U[i  ]*w.x + D[i  ]*w.x
               - R[i+1]*w.y + L[i+1]*w.y + U[i+1]*w.x - D[i+1]*w.x <= v.y * w.x - v.x * w.y

                N-1
            MIN SUM (R[i] + U[i] + L[i] + D[i])
                i=0

                    N-1
            = - MAX SUM (-R[i] - U[i] - L[i] - D[i])
                    i=0

           ------------------------

                      N-1
            m = 4*N + SUM size(polygon[i]) * size(polygon[i - 1])
                      i=1

            A[m][6*N], b[m], c[6*N]

                    0   1   2 ... N-2 N-1  N N+1 N+2   2N-2 2N-1 2N  ...      3N-2 3N-1 3N 3N+1 3N+2    ...    4N     ...       6N-1
                   R0  R1  R2 ...         U0  U1  U2 ...         L0  L1  L2 ...         D0  D1  D2 ...
            A = |  -1   0   0 ...  0   0   0   0   0 ...  0   0   1   0   0 ...  0   0   0   0   0 ...  0   0  -1  0  0 ...  0  0 |  b = | 0                 |
                |   1   0   0 ...  0   0   0   0   0 ...  0   0  -1   0   0 ...  0   0   0   0   0 ...  0   0  -1  0  0 ...  0  0 |      | 0                 |
                |   0  -1   0 ...  0   0   0   0   0 ...  0   0   0   1   0 ...  0   0   0   0   0 ...  0   0   0 -1  0 ...  0  0 |      | 0                 |
                |   0   1   0 ...  0   0   0   0   0 ...  0   0   0  -1   0 ...  0   0   0   0   0 ...  0   0   0 -1  0 ...  0  0 |      | 0                 |
                |   0   0  -1 ...  0   0   0   0   0 ...  0   0   0   0   1 ...  0   0   0   0   0 ...  0   0   0  0 -1 ...  0  0 |      | 0                 |
                |   0   0   1 ...  0   0   0   0   0 ...  0   0   0   0  -1 ...  0   0   0   0   0 ...  0   0   0  0 -1 ...  0  0 |      | 0                 |
                |     ...     ...          ...       ...          ...       ...          ...       ...          ...     ...       |      | ...               |
                |   0   0   0 ...  0   0   0  -1   0 ...  0   0   0   0   0 ...  0   0   0  -1   0 ...  0   0   0  0  0 ...  0  0 |      | 0                 |
                |   0   0   0 ...  0   0   0   1   0 ...  0   0   0   0   0 ...  0   0   0   1   0 ...  0   0   0  0  0 ...  0  0 |      | 0                 |
                |     ...     ...          ...       ...          ...       ...          ...       ...          ...     ...       |      | ...               |
                |   0   0   0 ...  0   0   0   0   0 ...  0  -1   0   0   0 ...  0   0   0   0   0 ...  0  -1   0  0  0 ...  0 -1 |      | 0                 |
                |   0   0   0 ...  0   0   0   0   0 ...  0   1   0   0   0 ...  0   0   0   0   0 ...  0   1   0  0  0 ...  0 -1 |      | 0                 |
                -------------------------------------------------------------------------------------------------------------------      ---------------------
                |  wy -wy   0 ...  0   0 -wx  wx   0 ...  0   0 -wy  wy   0 ...  0   0  wx -wx   0 ...  0   0   0  0  0 ...  0  0 |      | vy * wx - vx * wy | 4*N
                |   0  wy -wy ...  0   0   0 -wx  wx ...  0   0   0 -wy  wy ...  0   0   0  wx -wx ...  0   0   0  0  0 ...  0  0 |      | vy * wx - vx * wy |
                |   0   0  wy ...  0   0   0   0 -wx ...  0   0   0   0 -wy ...  0   0   0   0  wx ...  0   0   0  0  0 ...  0  0 |      | vy * wx - vx * wy |
                |     ...     ...          ...       ...          ...       ...          ...       ...          ...     ...       |      | ...               |
                |   0   0   0 ... wy -wy   0   0   0 ... -wx wx   0   0   0 ... -wy wy   0   0   0 ...  wx -wx  0  0  0 ...  0  0 |      | vy * wx - vx * wy | 6*N-1

                                    4N              6N-1
            c = | 0 0 0 0 0 ... 0 0 -1 -1 -1 ... -1 -1 |
        */

        vector<vector<double>> A;
        vector<double> b;
        vector<double> c(6 * N);
        fill(c.begin() + 4 * N, c.end(), -1);
        for (int i = 0; i < 2 * N; i++) {
            vector<double> a(6 * N);
            a[i] = -1;
            a[i + 2 * N] = 1;
            a[i + 4 * N] = -1;
            A.push_back(a);
            b.push_back(0);

            a[i] = 1;
            a[i + 2 * N] = -1;
            A.push_back(a);
            b.push_back(0);
        }

        for (int i = 1; i < N; i++) {
            auto& poly = polygons[perm[i]];
            int size = int(poly.points.size());
            for (int j = 0; j < size; j++) {
                int wx = poly.points[(j + 1) % size].first - poly.points[j].first;
                int wy = poly.points[(j + 1) % size].second - poly.points[j].second;
                int r = i - 1;
                for (auto& pt : polygons[perm[r]].points) {
                    int vx = pt.first - poly.points[j].first;
                    int vy = pt.second - poly.points[j].second;
                    vector<double> a(6 * N);
                    a[r] = wy;
                    a[r + N] = -wx;
                    a[r + 2 * N] = -wy;
                    a[r + 3 * N] = wx;
                    a[i] = -wy;
                    a[i + N] = wx;
                    a[i + 2 * N] = wy;
                    a[i + 3 * N] = -wx;
                    A.push_back(a);
                    b.push_back(vy * wx - vx * wy);
                }
            }
        }

        vector<double> x;
        auto res = Simplex(A, b, c).solve(x);
        if (res == numeric_limits<double>::infinity() || res == -numeric_limits<double>::infinity())
            return -1;
        return -res;
    }
};
