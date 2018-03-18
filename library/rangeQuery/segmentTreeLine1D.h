#pragma once

struct SegmentTreeLine1D {
    int N;
    // (the number of lines occupying min covered subsegments, total length of min covered subsegments)
    vector<pair<int, int>> tree;
    vector<int> lazy;               // delta to add


    SegmentTreeLine1D() : N(0) {
    }

    explicit SegmentTreeLine1D(int n) {
        build(n);
    }


    void build(int n) {
        N = n;
        tree = vector<pair<int, int>>(n * 4);
        lazy = vector<int>(n * 4);

        buildSub(1, 0, n - 1);
    }


    // inclusive
    // delta is normally -1 or 1
    void add(int lineX1, int lineX2, int delta) {
        addSub(1, 0, N - 1, lineX1, lineX2, delta);
    }

    // return N - covered area
    int queryNotCovered() const {
        return tree[1].first == 0 ? tree[1].second : 0;
    }

    // total length of 'count' covered segments in (left, right)
    // inclusive
    int query(int left, int right, int count) {
        return querySub(1, 0, N - 1, left, right, count);
    }

    void forEachCovered(int left, int right, const function<void(int)>& f) {
        forEachCoveredSub(1, 0, N - 1, left, right, f);
    }

private:
    // inclusive
    void buildSub(int node, int left, int right) {
        if (left == right) {
            tree[node].second = 1;
        } else {
            int mid = left + (right - left) / 2;

            buildSub(2 * node, left, mid);
            buildSub(2 * node + 1, mid + 1, right);

            merge(node);
        }
    }

    void merge(int node) {
        if (tree[2 * node].first == tree[2 * node + 1].first) {
            tree[node].first = tree[2 * node].first;
            tree[node].second = tree[2 * node].second + tree[2 * node + 1].second;
        } else {
            tree[node] = min(tree[2 * node], tree[2 * node + 1]);
        }
    }

    // inclusive
    void addSub(int node, int left, int right, int lineX1, int lineX2, int delta) {
        if (left == lineX1 && right == lineX2) {
            addTo(node, delta);
        } else {
            push(node);

            int mid = left + (right - left) / 2;

            if (lineX2 <= mid)
                addSub(2 * node, left, mid, lineX1, lineX2, delta);
            else if (lineX1 > mid)
                addSub(2 * node + 1, mid + 1, right, lineX1, lineX2, delta);
            else {
                addSub(2 * node, left, mid, lineX1, mid, delta);
                addSub(2 * node + 1, mid + 1, right, mid + 1, lineX2, delta);
            }

            merge(node);
        }
    }

    void addTo(int node, int delta) {
        lazy[node] += delta;
        tree[node].first += delta;
    }

    void push(int node) {
        if (lazy[node] != 0) {
            addTo(2 * node, lazy[node]);
            addTo(2 * node + 1, lazy[node]);
            lazy[node] = 0;
        }
    }

    // inclusive
    int querySub(int node, int left, int right, int lineX1, int lineX2, int count) {
        if (left == lineX1 && right == lineX2) {
            return tree[node].first == count ? tree[node].second : 0;
        } else {
            push(node);

            int mid = left + (right - left) / 2;

            if (lineX2 <= mid)
                return querySub(2 * node, left, mid, lineX1, lineX2, count);
            else if (lineX1 > mid)
                return querySub(2 * node + 1, mid + 1, right, lineX1, lineX2, count);
            else
                return querySub(2 * node, left, mid, lineX1, mid, count)
                     + querySub(2 * node + 1, mid + 1, right, mid + 1, lineX2, count);
        }
    }

    void forEachCoveredSub(int node, int left, int right, int lineX1, int lineX2, const function<void(int)>& f) {
        if (left > right || lineX1 > right || lineX2 < left)
            return;

        if (left == lineX1 && right == lineX2) {
            if (tree[node].first > 0) {
                for (int i = left; i <= right; i++)
                    f(i);
                return;
            } else if (tree[node].first == 0 && tree[node].second == (right - left + 1))
                return;
        }

        if (left < right) {
            push(node);

            int mid = left + (right - left) / 2;
            if (lineX2 <= mid)
                forEachCoveredSub(2 * node, left, mid, lineX1, lineX2, f);
            else if (lineX1 > mid)
                forEachCoveredSub(2 * node + 1, mid + 1, right, lineX1, lineX2, f);
            else {
                forEachCoveredSub(2 * node, left, mid, lineX1, mid, f);
                forEachCoveredSub(2 * node + 1, mid + 1, right, mid + 1, lineX2, f);
            }
        }
    }
};
