#pragma once

// The result is counter-clockwise, O(HN)
// The starting point of the result is the bottom of the leftmost
template <typename T>
vector<Vec2D<T>> doJarvis(vector<Vec2D<T>>& Q) {
    int N = int(Q.size());
    if (N < 3) {
        if (N == 2 && Q[0] > Q[1])
            swap(Q[0], Q[1]);
        return Q;
    }

    vector<Vec2D<T>> S;

    // find starting point
    int start = 0;
    for (int i = 1; i < N; i++) {
        if (Q[i] < Q[start])
            start = i;
    }

    int p = start, q;
    do {
        S.push_back(Q[p]);

        q = (p + 1) % N;
        for (int i = 0; i < N; i++) {
            auto cr = cross(Q[p], Q[i], Q[q]);
            if ((isZero(cr) && Q[i].norm2(Q[p]) > Q[q].norm2(Q[p])) || cr > 0)
                q = i;
        }

        p = q;
    } while (p != start);

    return S;
}

// The result is counter-clockwise, O(NlogN)
// The starting point of the result is the leftmost of the bottom
template <typename T>
vector<Vec2D<T>> doGrahamScan(vector<Vec2D<T>>& Q) {
    vector<Vec2D<T>> S;

    int N = int(Q.size());
    if (N == 0)
        return S;

    // find staring point
    int start = 0;
    for (int i = 1; i < N; i++) {
        if (Q[i].second < Q[start].second || (Q[i].second == Q[start].second && Q[i].first < Q[start].first))
            start = i;
    }
    swap(Q[0], Q[start]);

    // sort points by angle from Q[0]
    Vec2D<T> p0 = Q[0];
    sort(Q.begin() + 1, Q.end(), [&p0](const Vec2D<T>& l, const Vec2D<T>& r) {
        auto cr = cross(p0, l, r);
        if (isZero(cr))
            return p0.norm2(l) < p0.norm2(r);
        return cr > 0;
    });

    // remove points of same angle
    int m = 1;
    for (int i = 1; i < N; i++) {
        while (i < N - 1 && isZero(cross(p0, Q[i], Q[i + 1])))
            i++;

        Q[m] = Q[i];
        m++;
    }
    Q.erase(Q.begin() + m, Q.end());

    if (Q.size() < 3)
        return Q;

    S.push_back(Q[0]);
    S.push_back(Q[1]);
    S.push_back(Q[2]);
    for (int i = 3; i < int(Q.size()); i++) {
        while (cross(S[S.size() - 2], S.back(), Q[i]) <= 0)
            S.pop_back();
        S.push_back(Q[i]);
    }

    return S;
}

// The result is counter-clockwise, O(NlogN)
// The starting point of the result is the leftmost of the bottom
template <typename T>
vector<Vec2D<T>> doGrahamScanNoRemove(vector<Vec2D<T>>& Q, bool excludeBoundaryPoints = true) {
    vector<Vec2D<T>> S;

    int N = int(Q.size());
    if (N == 0)
        return S;

    // find staring point
    int start = 0;
    for (int i = 1; i < N; i++) {
        if (Q[i].second < Q[start].second || (Q[i].second == Q[start].second && Q[i].first < Q[start].first))
            start = i;
    }
    swap(Q[0], Q[start]);

    // sort points by angle from Q[0]
    Vec2D<T> p0 = Q[0];
    sort(Q.begin() + 1, Q.end(), [&p0](const Vec2D<T>& l, const Vec2D<T>& r) {
        auto cr = cross(p0, l, r);
        if (isZero(cr))
            return p0.norm2(l) < p0.norm2(r);
        return cr > 0;
    });

    if (Q.size() < 2)
        return Q;

    S.push_back(Q[0]);
    S.push_back(Q[1]);
    if (excludeBoundaryPoints) {
        for (int i = 2; i < int(Q.size()); i++) {
            while (S.size() >= 2 && cross(S[S.size() - 2], S.back(), Q[i]) <= 0)
                S.pop_back();
            S.push_back(Q[i]);
        }
    } else {
        for (int i = 2; i < int(Q.size()); i++) {
            while (S.size() >= 2 && cross(S[S.size() - 2], S.back(), Q[i]) < 0)
                S.pop_back();
            S.push_back(Q[i]);
        }
        for (int i = int(Q.size()) - 2; i >= 2; i--) {
            if (cross(S.back(), S[0], Q[i]) != 0)
                break;
            S.push_back(Q[i]);
        }
    }

    return S;
}


// The result is counter-clockwise, O(NlogN)
// The starting point of the result is the bottom of the leftmost
template <typename T>
vector<Vec2D<T>> doGrahamAndrew(vector<Vec2D<T>>& Q) {
    int N = int(Q.size());
    if (N <= 1)
        return Q;

    sort(Q.begin(), Q.end());

    vector<Vec2D<T>> res(N * 2);

    int k = 0;
    for (int i = 0; i < N; i++) {
        while (k >= 2 && !ccw(res[k - 2], res[k - 1], Q[i]))
            k--;
        res[k++] = Q[i];
    }
    for (int i = N - 2, t = k; i >= 0; i--) {
        while (k > t && !ccw(res[k - 2], res[k - 1], Q[i]))
            k--;
        res[k++] = Q[i];
    }

    res.resize(k - 1 - (res[0] == res[1]));
    return res;
}

// The results is sorted by X, O(NlogN)
struct GrahamAndrew {
    static void convexHull(vector<pair<int, int>>& inP, vector<pair<int, int>>& outLower, vector<pair<int, int>>& outUpper) {
        int j = 0, k = 0, n = int(inP.size());
        sort(inP.begin(), inP.end());
        outUpper.resize(n);
        outLower.resize(n);
        for (int i = 0; i < n; i++) {
            while (j >= 2 && cross(outLower[j - 2], outLower[j - 1], inP[i]) <= 0)
                j--;
            while (k >= 2 && cross(outUpper[k - 2], outUpper[k - 1], inP[i]) >= 0)
                k--;
            outLower[j++] = inP[i];
            outUpper[k++] = inP[i];
        }
        outUpper.resize(k);
        outLower.resize(j);
    }

    static long long calcMaxDiameter(const vector<pair<int, int>>& inLower, const vector<pair<int, int>>& inUpper) {
        int j = 0, k, m;
        k = int(inLower.size()) - 1;
        m = int(inUpper.size()) - 1;
        long long dist = 0;
        while (j < m || k > 0) {
            dist = max(dist, squareDist(inUpper[j], inLower[k]));
            if (j == m)
                k--;
            else if (k == 0)
                j++;
            else {
                if (1ll * (inUpper[j + 1].second - inUpper[j].second) * (inLower[k].first - inLower[k - 1].first) >
                    1ll * (inLower[k].second - inLower[k - 1].second) * (inUpper[j + 1].first - inUpper[j].first))
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
