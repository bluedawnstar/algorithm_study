#pragma once

// merge sort tree for 2D convex hull
// <How to Use>
//   1. call build() to build merge sort tree for convex hull
//   2. call getLower() & getUpper() to get lower and upper points in a range
//   3. call hullLowerSorted() & hullUpperSorted() to get lower and upper convex hull in a range
struct MergeSortTreeConvexHull {
    int N;                                  // power of 2
    int realN;                              // real count
    vector<vector<pair<int, int>>> treeL;   // { (x0, y0), ..., (xi, yi), ..., (xn, yn) }
    vector<vector<pair<int, int>>> treeU;   // { (x0, y0), ..., (xj, yj), ..., (xn, yn) }

    void build(const pair<int,int> points[], int n) {
        realN = n;
        N = 1;
        while (N < realN)
            N <<= 1;

        treeL = vector<vector<pair<int, int>>>(N * 2);
        treeU = vector<vector<pair<int, int>>>(N * 2);
        for (int i = 0; i < realN; i++) {
            treeL[N + i].resize(1);
            treeU[N + i].resize(1);
            treeL[N + i][0] = treeU[N + i][0] = points[i];
        }

        for (int i = N - 1; i >= 1; i--) {
            {
                auto& L = treeL[i * 2];
                auto& R = treeL[i * 2 + 1];
                int len = int(L.size() + R.size());

                treeL[i].resize(len);
                merge(L.begin(), L.end(), R.begin(), R.end(), treeL[i].begin());
                if (len > 2)
                    hullLowerSorted(treeL[i]);
            }
            {
                auto& L = treeU[i * 2];
                auto& R = treeU[i * 2 + 1];
                int len = int(L.size() + R.size());

                treeU[i].resize(len);
                merge(L.begin(), L.end(), R.begin(), R.end(), treeU[i].begin());
                if (len > 2)
                    hullUpperSorted(treeU[i]);
            }
        }
    }

    void build(const vector<pair<int, int>>& points) {
        build(&points[0], int(points.size()));
    }


    vector<pair<int,int>> getLower(int L, int R) const {
        vector<pair<int, int>> res;
        for (L += N, R += N; L <= R; L >>= 1, R >>= 1) {
            if (L & 1)
                mergeTo(res, treeL[L++]);
            if ((R & 1) == 0)
                mergeTo(res, treeL[R--]);
        }
        return res;
    }

    vector<pair<int,int>> getUpper(int L, int R) const {
        vector<pair<int, int>> res;
        for (L += N, R += N; L <= R; L >>= 1, R >>= 1) {
            if (L & 1)
                mergeTo(res, treeU[L++]);
            if ((R & 1) == 0)
                mergeTo(res, treeU[R--]);
        }
        return res;
    }


    void getLower(vector<pair<int, int>>& dst, int L, int R) const {
        for (L += N, R += N; L <= R; L >>= 1, R >>= 1) {
            if (L & 1)
                mergeTo(dst, treeL[L++]);
            if ((R & 1) == 0)
                mergeTo(dst, treeL[R--]);
        }
    }

    void getUpper(vector<pair<int, int>>& dst, int L, int R) const {
        for (L += N, R += N; L <= R; L >>= 1, R >>= 1) {
            if (L & 1)
                mergeTo(dst, treeU[L++]);
            if ((R & 1) == 0)
                mergeTo(dst, treeU[R--]);
        }
    }


    // PRECONDITION: points must be sorted by X coordinate
    static void hullLowerSorted(vector<pair<int, int>>& points) {
        int j = 0, n = int(points.size());
        for (int i = 0; i < n; i++) {
            while (j >= 2 && cross(points[j - 2], points[j - 1], points[i]) <= 0)
                j--;
            points[j++] = points[i];
        }
        points.resize(j);
    }

    // PRECONDITION: points must be sorted by X coordinate
    static void hullUpperSorted(vector<pair<int, int>>& points) {
        int k = 0, n = int(points.size());
        for (int i = 0; i < n; i++) {
            while (k >= 2 && cross(points[k - 2], points[k - 1], points[i]) >= 0)
                k--;
            points[k++] = points[i];
        }
        points.resize(k);
    }

    static void hullLower(vector<pair<int, int>>& points) {
        sort(points.begin(), points.end());
        hullLowerSorted(points);
    }

    static void hullUpper(vector<pair<int, int>>& points) {
        sort(points.begin(), points.end());
        hullUpperSorted(points);
    }


    static long long calcMaxDist(const vector<pair<int,int>>& hullLower, const vector<pair<int, int>>& hullUpper) {
        int j = 0;
        int k = int(hullLower.size()) - 1;
        int m = int(hullUpper.size()) - 1;
        long long dist = 0;
        while (j < m || k > 0) {
            dist = max(dist, squareDist(hullUpper[j], hullLower[k]));
            if (j == m)
                k--;
            else if (k == 0)
                j++;
            else {
                if (1ll * (hullUpper[j + 1].second - hullUpper[j].second) * (hullLower[k].first - hullLower[k - 1].first) >
                    1ll * (hullLower[k].second - hullLower[k - 1].second) * (hullUpper[j + 1].first - hullUpper[j].first))
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

// https://www.codechef.com/JAN19A/problems/MXDIST/
// qry = { (left index of input points, right index of input points, an index of 'ans'), ... }
inline void solveRangeMaxDist(MergeSortTreeConvexHull& tree, vector<long long>& ans, vector<tuple<int, int, int>>& qry) {
    // MO's algorithm
    int blockN = int(sqrt(qry.size()));
    sort(qry.begin(), qry.end(), [blockN](const auto& l, const auto& r) {
        if (get<0>(l) / blockN != get<0>(r) / blockN)
            return get<0>(l) / blockN < get<0>(r) / blockN;
        return get<1>(l) < get<1>(r);
    });

    vector<pair<int, int>> lower, upper;

    int prevL = -1, prevR = -1;
    int L = -1, R = -1, idx;
    for (auto& it : qry) {
        tie(L, R, idx) = it;

        if (prevR < L || R < prevL) {
            // new range
            lower = tree.getLower(L, R);
            upper = tree.getUpper(L, R);
        } else if (prevL < L || R < prevR) {
            // delete points from left or right
            lower = tree.getLower(L, R);
            upper = tree.getUpper(L, R);
        } else {
            // add points to left and right
            if (L < prevL) {
                tree.getLower(lower, L, prevL - 1);
                tree.getUpper(upper, L, prevL - 1);
            }
            if (prevR < R) {
                tree.getLower(lower, prevR + 1, R);
                tree.getUpper(upper, prevR + 1, R);
            }
        }

        MergeSortTreeConvexHull::hullLowerSorted(lower);
        MergeSortTreeConvexHull::hullUpperSorted(upper);
        ans[idx] = MergeSortTreeConvexHull::calcMaxDist(lower, upper);

        prevL = L;
        prevR = R;
    }
}
