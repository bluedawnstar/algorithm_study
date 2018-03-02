#pragma once

// Li Chao Segment Tree
// https://e-maxx-eng.appspot.com/geometry/convex_hull_trick.html
struct SegmentTreeLine2DArrayMax {
    static const long long INF = 0x3f3f3f3f3f3f3f3fll;

    struct Line {
        long long m, b;             // f(x) = m * x + b

        long long get(int x) const {
            return 1ll * m * x + b;
        }
    };

    int N;
    vector<Line> tree;

    // inclusive
    SegmentTreeLine2DArrayMax(int size) : N(size), tree(size * 4) {
    }

    void add(long long m, long long b) {
        Line l{ m, b };
        addSub(l, 1, 0, N - 1);
    }

    long long query(int x) const {
        return querySub(x, 1, 0, N - 1);
    }

private:
    void addSub(Line& l, int node, int left, int right) {
        if (left > right)
            return;

        if (left == right) {
            tree[node] = l;
            return;
        }

        long long trl = tree[node].get(left);
        long long trr = tree[node].get(right);
        long long vl = l.get(left);
        long long vr = l.get(right);

        if (trl >= vl && trr >= vr)
            return;

        if (trl < vl && trr < vr) {
            tree[node] = l;
            return;
        }

        int mid = left + (right - left) / 2;
        if (trl < vl)
            swap(tree[node], l);

        if (tree[node].get(mid) > l.get(mid))
            addSub(l, (node << 1) | 1, mid + 1, right);
        else {
            swap(tree[node], l);
            addSub(l, (node << 1), left, mid);
        }
    }

    long long querySub(int x, int node, int left, int right) const {
        if (left > right || left > x || right < x)
            return -INF;

        if (left == right)
            return tree[node].get(x);

        int mid = left + (right - left) / 2;
        if (x <= mid)
            return max(tree[node].get(x), querySub(x, (node << 1), left, mid));
        else
            return max(tree[node].get(x), querySub(x, (node << 1) | 1, mid + 1, right));
    }
};
