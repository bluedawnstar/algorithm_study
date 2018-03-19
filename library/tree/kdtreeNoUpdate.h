#pragma once

template <typename T = int, int KD = 2>
struct KDTreeNoUpdate {
    vector<int>         tree;
    vector<vector<T>>   points;
    vector<pair<T, T>>  bound;  // [min, max)

    KDTreeNoUpdate() {
    }

    // O(N logN)
    void build(const vector<vector<T>>& in) {
        tree = vector<int>(in.size());
        points = in;
        bound = vector<pair<T, T>>(KD, make_pair(numeric_limits<T>::max(), numeric_limits<T>::min()));

        iota(tree.begin(), tree.end(), 0);
        for (int i = 0; i < (int)in.size(); i++) {
            for (int j = 0; j < KD; j++) {
                bound[j].first = min(bound[j].first, in[i][j]);
                bound[j].second = max(bound[j].second, in[i][j] + 1);
            }
        }

        _build(0, (int)tree.size(), 0);
    }

    // O(N logN)
    void build(const T in[][KD], int size) {
        tree = vector<int>(size);
        points = vector<vector<T>>(size, vector<T>(KD));
        bound = vector<pair<T, T>>(KD, make_pair(numeric_limits<T>::max(), numeric_limits<T>::min()));

        iota(tree.begin(), tree.end(), 0);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < KD; j++) {
                points[i][j] = in[i][j];
                bound[j].first = min(bound[j].first, in[i][j]);
                bound[j].second = max(bound[j].second, in[i][j] + 1);
            }
        }

        _build(0, (int)tree.size(), 0);
    }


    // O(logN)
    template <typename U>
    int searchNearest(const U& qry) {
        int res = -1;
        auto bnd = bound;
        double minDist = numeric_limits<double>::max();
        _searchNearest(0, (int)tree.size(), qry, 0, res, minDist, bnd);
        return res;
    }

    // O(logN), inclusive
    template <typename U>
    vector<int> searchRegion(const U& minPoint, const U& maxPoint) {
        vector<int> res;
        auto bnd = bound;
        _searchRegion(0, (int)tree.size(), 0, res, bnd, minPoint, maxPoint);
        return res;
    }

    // O(logN), inclusive
    template <typename U>
    vector<int> searchRegionFast(const U& minPoint, const U& maxPoint) {
        vector<int> res;
        // [min, max]
        vector<pair<T, T>> bound(KD);
        for (int i = 0; i < KD; i++) {
            bound[i].first = minPoint[i];
            bound[i].second = maxPoint[i] + 1;
        }
        _searchRegionFast(0, (int)tree.size(), 0, res, bound);
        return res;
    }

    // O(logN), inclusive
    template <typename U>
    int countRegion(const U& minPoint, const U& maxPoint) {
        auto bnd = bound;
        return _countRegion(0, (int)tree.size(), 0, bnd, minPoint, maxPoint);
    }

    // O(logN), inclusive
    template <typename U>
    int countRegionFast(const U& minPoint, const U& maxPoint) {
        // [min, max]
        vector<pair<T, T>> bound(KD);
        for (int i = 0; i < KD; i++) {
            bound[i].first = minPoint[i];
            bound[i].second = maxPoint[i] + 1;
        }
        return _countRegionFast(0, (int)tree.size(), 0, bound);
    }

protected:
    void _build(int first, int last, int depth) {
        if (first >= last)
            return;

        int currD = depth % KD;

        int mid = first + (last - first) / 2;
        nth_element(tree.begin() + first, tree.begin() + mid, tree.begin() + last,
            [this, currD](int left, int right) {
                return points[left][currD] < points[right][currD];
            }
        );

        _build(first, mid, depth + 1);
        _build(mid + 1, last, depth + 1);
    }

    template <typename U>
    double calcDistance(int node, const U& point) {
        double res = 0.0;
        for (int i = 0; i < KD; i++) {
            res += double(points[node][i] - point[i]) * double(points[node][i] - point[i]);
        }
        return sqrt(res);
    }

    template <typename U>
    void _searchNearest(int first, int last, const U& qry, int depth, int& res, double& minDist, vector<pair<T, T>>& bound) {
        if (first >= last)
            return;

        if (res >= 0) {
            for (int i = 0; i < KD; i++) {
                if (qry[i] + minDist < bound[i].first || bound[i].second <= qry[i] - minDist)
                    return;
            }
        }

        int currD = depth % KD;
        int mid = first + (last - first) / 2;
        int node = tree[mid];

        auto dist = calcDistance(node, qry);
        if (dist < minDist) {
            minDist = dist;
            res = node;
        }

        if (qry[currD] < points[node][currD]) {
            auto temp = bound[currD];

            bound[currD].second = min(bound[currD].second, points[node][currD]);
            _searchNearest(first, mid, qry, depth + 1, res, minDist, bound);
            bound[currD].second = temp.second;

            if (points[node][currD] <= qry[currD] + minDist) {
                bound[currD].first = max(bound[currD].first, points[node][currD]);
                _searchNearest(mid + 1, last, qry, depth + 1, res, minDist, bound);
                bound[currD].first = temp.first;
            }
        } else {
            auto temp = bound[currD];

            bound[currD].first = max(bound[currD].first, points[node][currD]);
            _searchNearest(mid + 1, last, qry, depth + 1, res, minDist, bound);
            bound[currD].first = temp.first;

            if (qry[currD] - minDist < points[node][currD]) {
                bound[currD].second = min(bound[currD].second, points[node][currD]);
                _searchNearest(first, mid, qry, depth + 1, res, minDist, bound);
                bound[currD].second = temp.second;
            }
        }
    }

    template <typename U>
    void _searchRegion(int first, int last, int depth, vector<int>& res, vector<pair<T, T>>& bound, const U& minPoint, const U& maxPoint) {
        if (first >= last)
            return;

        for (int i = 0; i < KD; i++) {
            if (maxPoint[i] < bound[i].first || bound[i].second <= minPoint[i] || bound[i].second <= bound[i].first)
                return;
        }

        int currD = depth % KD;
        int mid = first + (last - first) / 2;
        int node = tree[mid];

        bool include = true;
        for (int i = 0; i < KD; i++) {
            if (bound[i].first < minPoint[i] || maxPoint[i] + 1 < bound[i].second) {
                include = false;
                break;
            }
        }
        if (include) {
            while (first < last) {
                res.push_back(tree[first++]);
            }
            return;
        }

        bool isIn = true;
        for (int i = 0; i < KD; i++) {
            if (points[node][i] < minPoint[i] || maxPoint[i] < points[node][i]) {
                isIn = false;
                break;
            }
        }
        if (isIn)
            res.push_back(node);

        if (minPoint[currD] <= points[node][currD]) {
            auto temp = bound[currD].second;

            bound[currD].second = min(bound[currD].second, points[node][currD] + 1);
            _searchRegion(first, mid, depth + 1, res, bound, minPoint, maxPoint);
            bound[currD].second = temp;
        }
        if (points[node][currD] <= maxPoint[currD]) {
            auto temp = bound[currD].first;

            bound[currD].first = max(bound[currD].first, points[node][currD]);
            _searchRegion(mid + 1, last, depth + 1, res, bound, minPoint, maxPoint);
            bound[currD].first = temp;
        }
    }

    void _searchRegionFast(int first, int last, int depth, vector<int>& res, vector<pair<T, T>>& bound) {
        if (first >= last)
            return;

        int currD = depth % KD;
        int mid = first + (last - first) / 2;
        int node = tree[mid];

        if (bound[currD].first >= bound[currD].second)
            return;

        if (bound[currD].first <= points[node][currD] && points[node][currD] < bound[currD].second) {
            bool isIn = true;
            for (int i = 0; i < KD; i++) {
                if (points[node][i] < bound[i].first || bound[i].second <= points[node][i]) {
                    isIn = false;
                    break;
                }
            }
            if (isIn)
                res.push_back(node);
        }

        if (bound[currD].first <= points[node][currD]) {
            auto temp = bound[currD].second;

            bound[currD].second = min(bound[currD].second, points[node][currD] + 1);
            _searchRegionFast(first, mid, depth + 1, res, bound);
            bound[currD].second = temp;
        }
        if (points[node][currD] < bound[currD].second) {
            auto temp = bound[currD].first;

            bound[currD].first = max(bound[currD].first, points[node][currD]);
            _searchRegionFast(mid + 1, last, depth + 1, res, bound);
            bound[currD].first = temp;
        }
    }

    template <typename U>
    int _countRegion(int first, int last, int depth, vector<pair<T, T>>& bound, const U& minPoint, const U& maxPoint) {
        if (first >= last)
            return 0;

        for (int i = 0; i < KD; i++) {
            if (maxPoint[i] < bound[i].first || bound[i].second <= minPoint[i] || bound[i].second <= bound[i].first)
                return 0;
        }

        int currD = depth % KD;
        int mid = first + (last - first) / 2;
        int node = tree[mid];

        bool include = true;
        for (int i = 0; i < KD; i++) {
            if (bound[i].first < minPoint[i] || maxPoint[i] + 1 < bound[i].second) {
                include = false;
                break;
            }
        }
        if (include)
            return last - first;

        int res = 0;

        bool isIn = true;
        for (int i = 0; i < KD; i++) {
            if (points[node][i] < minPoint[i] || maxPoint[i] < points[node][i]) {
                isIn = false;
                break;
            }
        }
        if (isIn)
            res++;

        if (minPoint[currD] <= points[node][currD]) {
            auto temp = bound[currD].second;

            bound[currD].second = min(bound[currD].second, points[node][currD] + 1);
            res += _countRegion(first, mid, depth + 1, bound, minPoint, maxPoint);
            bound[currD].second = temp;
        }
        if (points[node][currD] <= maxPoint[currD]) {
            auto temp = bound[currD].first;

            bound[currD].first = max(bound[currD].first, points[node][currD]);
            res += _countRegion(mid + 1, last, depth + 1, bound, minPoint, maxPoint);
            bound[currD].first = temp;
        }

        return res;
    }

    int _countRegionFast(int first, int last, int depth, vector<pair<T, T>>& bound) {
        if (first >= last)
            return 0;

        int currD = depth % KD;
        int mid = first + (last - first) / 2;
        int node = tree[mid];

        if (bound[currD].first >= bound[currD].second)
            return 0;

        int res = 0;

        if (bound[currD].first <= points[node][currD] && points[node][currD] < bound[currD].second) {
            bool isIn = true;
            for (int i = 0; i < KD; i++) {
                if (points[node][i] < bound[i].first || bound[i].second <= points[node][i]) {
                    isIn = false;
                    break;
                }
            }
            if (isIn)
                res++;
        }

        if (bound[currD].first <= points[node][currD]) {
            auto temp = bound[currD].second;

            bound[currD].second = min(bound[currD].second, points[node][currD] + 1);
            res += _countRegionFast(first, mid, depth + 1, bound);
            bound[currD].second = temp;
        }
        if (points[node][currD] < bound[currD].second) {
            auto temp = bound[currD].first;

            bound[currD].first = max(bound[currD].first, points[node][currD]);
            res += _countRegionFast(mid + 1, last, depth + 1, bound);
            bound[currD].first = temp;
        }

        return res;
    }
};
