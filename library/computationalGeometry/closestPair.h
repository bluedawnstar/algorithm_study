#pragma once

template <typename T>
struct ClosestPair {
    static const T INF = 0x3f3f3f3f;

    // O(N*(logN)^2)
    static vector<pair<T, T>> findClosestPair(vector<pair<T, T>>& points) {
        vector<pair<T, T>> res(2);

        sort(points.begin(), points.end(), [](const pair<T, T>& left, const pair<T, T>& right) {
            if (left.first != right.first)
                return left.first < right.first;
            return left.second < right.second;
        });

        findSub(points, 0, (int)points.size() - 1, res, INF);

        return res;
    }

private:
    // inclusive
    static T findSub(vector<pair<T, T>>& points, int left, int right, vector<pair<T, T>>& res, T minDist) {
        if (left == right)
            return INF;

        int mid = left + (right - left) / 2;
        T midX = points[mid].first;

        minDist = min(minDist, findSub(points, left, mid, res, minDist));
        minDist = min(minDist, findSub(points, mid + 1, right, res, minDist));

        sort(points.begin() + left, points.begin() + right + 1, [](const pair<T, T>& left, const pair<T, T>& right) {
            return left.second < right.second;
        });

        vector<int> t(right - left + 1);

        int size = 0;
        for (int i = left; i <= right; i++)
            if (abs(points[i].first - midX) < minDist)
                t[size++] = i;

        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; j++) {
                pair<T, T>& a = points[t[i]];
                pair<T, T>& b = points[t[j]];
                if (b.second - a.second >= minDist)
                    break;

                T dist = distL2(a, b);
                if (minDist > dist) {
                    minDist = dist;
                    res[0] = a;
                    res[1] = b;
                }
            }
        }

        return minDist;
    }

    static T distL2(const pair<T, T>& a, const pair<T, T>& b) {
        long long dx = a.first - b.first;
        long long dy = a.second - b.second;
        return dx * dx + dy * dy;
    }
};
