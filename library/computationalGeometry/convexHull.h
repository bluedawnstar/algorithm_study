#pragma once

// O(HN)
template <typename T>
vector<Vec2D<T>> doJarvis(vector<Vec2D<T>>& Q) {
    int N = (int)Q.size();
    if (N < 3)
        return Q;

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
            T cr = cross(Q[p], Q[i], Q[q]);
            if ((isZero(cr) && Q[i].norm2(Q[p]) > Q[q].norm2(Q[p])) || cr > 0)
                q = i;
        }

        p = q;
    } while (p != start);

    return S;
}

// O(NlogN)
template <typename T>
vector<Vec2D<T>> doGrahamScan(vector<Vec2D<T>>& Q) {
    vector<Vec2D<T>> S;

    int N = (int)Q.size();
    if (N == 0)
        return S;

    // find staring point
    int start = 0;
    for (int i = 1; i < N; i++) {
        if (Q[i].y < Q[start].y || (Q[i].y == Q[start].y && Q[i].x < Q[start].x))
            start = i;
    }
    swap(Q[0], Q[start]);

    // sort points by angle from Q[0]
    Vec2D<T> p0 = Q[0];
    sort(Q.begin() + 1, Q.end(), [&p0](const Vec2D<T>& l, const Vec2D<T>& r) {
        T cr = cross(p0, l, r);
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
    for (int i = 3; i < (int)Q.size(); i++) {
        while (cross(S[S.size() - 2], S.back(), Q[i]) <= 0)
            S.pop_back();
        S.push_back(Q[i]);
    }

    return S;
}

// O(NlogN)
template <typename T>
vector<Vec2D<T>> doGrahamScanNoRemove(vector<Vec2D<T>>& Q, bool excludeBoundaryPoints = true) {
    vector<Vec2D<T>> S;

    int N = (int)Q.size();
    if (N == 0)
        return S;

    // find staring point
    int start = 0;
    for (int i = 1; i < N; i++) {
        if (Q[i].y < Q[start].y || (Q[i].y == Q[start].y && Q[i].x < Q[start].x))
            start = i;
    }
    swap(Q[0], Q[start]);

    // sort points by angle from Q[0]
    Vec2D<T> p0 = Q[0];
    sort(Q.begin() + 1, Q.end(), [&p0](const Vec2D<T>& l, const Vec2D<T>& r) {
        T cr = cross(p0, l, r);
        if (isZero(cr))
            return p0.norm2(l) < p0.norm2(r);
        return cr > 0;
    });

    if (Q.size() < 2)
        return Q;

    S.push_back(Q[0]);
    S.push_back(Q[1]);
    if (excludeBoundaryPoints) {
        for (int i = 2; i < (int)Q.size(); i++) {
            while (S.size() >= 2 && cross(S[S.size() - 2], S.back(), Q[i]) <= 0)
                S.pop_back();
            S.push_back(Q[i]);
        }
    } else {
        for (int i = 2; i < (int)Q.size(); i++) {
            while (S.size() >= 2 && cross(S[S.size() - 2], S.back(), Q[i]) < 0)
                S.pop_back();
            S.push_back(Q[i]);
        }
        for (int i = (int)Q.size() - 2; i >= 2; i--) {
            if (cross(S.back(), S[0], Q[i]) != 0)
                break;
            S.push_back(Q[i]);
        }
    }

    return S;
}


// O(NlogN)
template <typename T>
vector<Vec2D<T>> doGrahamAndrew(vector<Vec2D<T>>& Q) {
    int N = (int)Q.size();
    if (N <= 1)
        return Q;

    sort(Q.begin(), Q.end());

    int k = 0;
    vector<Vec2D<T>> res(N * 2);
    for (int i = 0; i < N; res[k++] = Q[i++])
        for (; k >= 2 && !cw(res[k - 2], res[k - 1], Q[i]); --k)
            ;

    for (int i = N - 2, t = k; i >= 0; res[k++] = Q[i--])
        for (; k > t && !cw(res[k - 2], res[k - 1], Q[i]); --k)
            ;

    res.resize(k - 1 - (res[0] == res[1]));
    return res;
}
