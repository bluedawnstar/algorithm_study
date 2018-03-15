#pragma once

template <typename T = int, typename D = long long>
struct MetricTree {
    static const T INF = 0x3f3f3f3f;
    static const T NINF = -0x3f3f3f3f;

    struct Rect {
        T x1, y1, x2, y2;
    };

    int N;
    vector<pair<T,T>> points;

    MetricTree() : N(0) {
    }

    MetricTree(const vector<pair<T,T>>& points) {
        build(points);
    }

    void build(const vector<pair<T,T>>& points) {
        N = (int)points.size();
        this->points = points;
        build(0, N);
    }


    // nearest neighbor
    // returns (point index, distance)
    pair<int, D> searchNearest(T x, T y) const {
        D bestDist = numeric_limits<D>::max();
        int bestNode = 0;
        searchNearest(0, N, x, y, bestDist, bestNode);
        return make_pair(bestNode, bestDist);
    }

    // nearest neighbor
    // returns (point, distance)
    pair<pair<T,T>, D> searchNearestPoint(T x, T y) const {
        D bestDist = numeric_limits<D>::max();
        int bestNode = 0;
        searchNearest(0, N, x, y, bestDist, bestNode);
        return make_pair(points[bestNode], bestDist);
    }

private:
    void build(int low, int high) {
        if (high - low <= 2)
            return;

        swap(points[low + rand() % (high - low)], points[low]);
        auto center = points[low++];

        int mid = low + (high - low) / 2;
        nth_element(points.begin() + low, points.begin() + mid, points.begin() + high,
            [center](const pair<T, T>& l, const pair<T, T>& r) {
            return distL2(center.first, center.second, l.first, l.second) < distL2(center.first, center.second, r.first, r.second);
        });

        build(low, mid);
        build(mid + 1, high);
    }

    void searchNearest(int low, int high, T x, T y, D& bestDist, int& bestNode) const {
        if (high - low <= 0)
            return;

        D d2 = distL2(x, y, points[low].first, points[low].second);
        if (bestDist > d2) {
            bestDist = d2;
            bestNode = low;
        }

        if (high - low <= 1)
            return;

        auto center = points[low++];

        int mid = low + (high - low) / 2;
        D dist2 = distL2(x, y, points[mid].first, points[mid].second);
        if (bestDist > dist2) {
            bestDist = dist2;
            bestNode = mid;
        }

        double R = sqrt(distL2(center.first, center.second, points[mid].first, points[mid].second));
        double r = sqrt(bestDist);
        double d = sqrt(d2);

        if (d <= R + r)
            searchNearest(low, mid, x, y, bestDist, bestNode);

        if (d >= R - r)
            searchNearest(mid + 1, high, x, y, bestDist, bestNode);
    }

    static D distL2(T x1, T y1, T x2, T y2) {
        D dx = x1 - x2;
        D dy = y1 - y2;
        return dx * dx + dy * dy;
    }
};