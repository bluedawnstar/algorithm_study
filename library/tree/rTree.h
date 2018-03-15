#pragma once

template <typename T>
struct RTree {
    static const T INF = 0x3f3f3f3f;
    static const T NINF = -0x3f3f3f3f;

    struct Rect {
        T x1, y1, x2, y2;
    };

    int N;
    vector<Rect> rects;
    vector<Rect> bounds;

    RTree() : N(0) {
    }

    RTree(const vector<Rect>& rects) {
        build(rects);
    }

    void build(const vector<Rect>& rects) {
        N = (int)rects.size();
        this->rects = rects;
        bounds.assign(N, Rect{ INF, INF, NINF, NINF });
        build(0, N, true, this->rects);
    }


    // nearest neighbor
    // returns (rectangle index, distance)
    pair<int, double> searchNearest(T x, T y) const {
        double bestDist = DBL_MAX;
        int bestNode = 0;
        searchNearest(0, N, x, y, true, bestDist, bestNode);
        return make_pair(bestNode, bestDist);
    }

    // nearest neighbor
    // returns (rectangle, distance)
    pair<Rect, double> searchNearestRect(T x, T y) const {
        double bestDist = DBL_MAX;
        int bestNode = 0;
        searchNearest(0, N, x, y, true, bestDist, bestNode);
        return make_pair(rects[bestNode], bestDist);
    }

private:
    void build(int low, int high, bool divX, vector<Rect>& rects) {
        if (low >= high)
            return;

        static function<bool(const Rect&, const Rect&)> cmps[2] = {
            [](const Rect& l, const Rect& r) { return l.y1 + l.y2 < r.y1 + r.y2; },
            [](const Rect& l, const Rect& r) { return l.x1 + l.x2 < r.x1 + r.x2; }
        };

        int mid = (low + high) / 2;
        nth_element(rects.begin() + low, rects.begin() + mid, rects.begin() + high, cmps[divX]);

        for (int i = low; i < high; i++) {
            bounds[mid].x1 = min(bounds[mid].x1, min(rects[i].x1, rects[i].x2));
            bounds[mid].y1 = min(bounds[mid].y1, min(rects[i].y1, rects[i].y2));
            bounds[mid].x2 = max(bounds[mid].x2, max(rects[i].x1, rects[i].x2));
            bounds[mid].y2 = max(bounds[mid].y2, max(rects[i].y1, rects[i].y2));
        }

        build(low, mid, !divX, rects);
        build(mid + 1, high, !divX, rects);
    }

    void searchNearest(int low, int high, T x, T y, bool divX, double& bestDist, int& bestNode) const {
        if (low >= high)
            return;

        int mid = (low + high) >> 1;
        double distance = pointToRectL2(x, y, rects[mid]);
        if (bestDist > distance) {
            bestDist = distance;
            bestNode = mid;
        }

        T delta = divX ? 2 * x - rects[mid].x1 - rects[mid].x2 : 2 * y - rects[mid].y1 - rects[mid].y2;

        if (delta <= 0) {
            searchNearest(low, mid, x, y, !divX, bestDist, bestNode);
            if (mid + 1 < high) {
                int mid1 = (mid + 1 + high) / 2;

                double dist = divX ? getDist(x, bounds[mid1].x1, bounds[mid1].x2)
                                   : getDist(y, bounds[mid1].y1, bounds[mid1].y2);
                if (dist * dist < bestDist)
                    searchNearest(mid + 1, high, x, y, !divX, bestDist, bestNode);
            }
        } else {
            searchNearest(mid + 1, high, x, y, !divX, bestDist, bestNode);
            if (low < mid) {
                int mid1 = (low + mid) / 2;

                double dist = divX ? getDist(x, bounds[mid1].x1, bounds[mid1].x2)
                                   : getDist(y, bounds[mid1].y1, bounds[mid1].y2);
                if (dist * dist < bestDist)
                    searchNearest(low, mid, x, y, !divX, bestDist, bestNode);
            }
        }
    }

    static double getDist(T v, T min, T max) {
        if (v <= min)
            return min - v;
        if (v >= max)
            return v - max;
        return 0;
    }

    static double pointToRectL2(T x, T y, const Rect& r) {
        double dx = r.x2 - r.x1;
        double dy = r.y2 - r.y1;
        double px = x - r.x1;
        double py = y - r.y1;

        double squaredLength = dx * dx + dy * dy;
        double dotProduct = dx * px + dy * py;
        if (dotProduct <= 0 || squaredLength == 0)
            return px * px + py * py;

        if (dotProduct >= squaredLength)
            return (px - dx) * (px - dx) + (py - dy) * (py - dy);

        double q = (double)dotProduct / squaredLength;
        return (px - q * dx) * (px - q * dx) + (py - q * dy) * (py - q * dy);
    }
};