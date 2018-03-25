#pragma once

// The first 'node' number is 1, not 0
// Others('left', 'right', 'nodeLeft', 'nodeRight', 'index') are started from 0
// Lower envelope with line segment
// https://www.hackerrank.com/contests/university-codesprint-4/challenges/magic-value
struct SegmentTreeLineSegment2DMin {
    static const long long INF = 0x3f3f3f3f3f3f3f3fll;

    struct Line {
        long long m, b;     // f(x) = m * x + b

        Line() : m(0), b(INF) {
        }

        Line(long long m, long long b) : m(m), b(b) {
        }

        long long get(int x) const {
            return m * x + b;
        }

        int compare(const Line& rhs, int x) const {
            if (get(x) <= rhs.get(x))
                return 1;
            else if (m >= rhs.m)
                return 2;
            else
                return 0;
        }
    };

    int N;
    vector<Line> treeL;
    vector<Line> treeR;
    vector<Line> treeLazy;
    vector<bool> lazyExist;

    SegmentTreeLineSegment2DMin() : N(0) {
    }

    explicit SegmentTreeLineSegment2DMin(int size) {
        init(size);
    }


    void init(int size) {
        N = size;
        treeL = vector<Line>(size * 4);
        treeR = vector<Line>(size * 4);
        treeLazy = vector<Line>(size * 4);
        lazyExist.assign(size * 4, false);
    }


    // add a line segment
    // inclusive, O(NlogN)
    void add(int left, int right, long long m, long long b) {
        Line l(m, b);
        addSub(l, left, right, 1, 0, N - 1);
    }

    // query max f(x)
    // O(logN)
    long long query(int x) {
        return querySub(x, 1, 0, N - 1);
    }

private:
    void pushDown(int node, int nodeLeft, int nodeRight) {
        if (lazyExist[node]) {
            if (nodeLeft < nodeRight) {
                treeLazy[node << 1] = treeLazy[(node << 1) | 1] = treeLazy[node];
                lazyExist[node << 1] = lazyExist[(node << 1) | 1] = true;
            }

            treeL[node] = treeLazy[node];
            treeR[node] = treeLazy[node];

            lazyExist[node] = false;
        }
    }

    void addSub(Line& l, int left, int right, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return;

        pushDown(node, nodeLeft, nodeRight);
        if (nodeLeft == nodeRight) {
            if (l.get(nodeLeft) < treeLazy[node].get(nodeLeft)) {
                treeLazy[node] = l;
                lazyExist[node] = true;
                pushDown(node, nodeLeft, nodeRight);
            }
            return;
        }

        if (left <= nodeLeft && nodeRight <= right) {
            int cl = l.compare(treeL[node], nodeLeft);
            int cr = l.compare(treeR[node], nodeRight);
            if (cl == cr) {
                if (cl == 1) {
                    treeLazy[node] = l;
                    lazyExist[node] = true;
                    pushDown(node, nodeLeft, nodeRight);
                    return;
                }
            } else if (cl == 2 && cr == 0)
                return;
        }

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        addSub(l, left, right, (node << 1), nodeLeft, mid);
        addSub(l, left, right, (node << 1) | 1, mid + 1, nodeRight);

        treeL[node] = treeL[node << 1];
        treeR[node] = treeR[(node << 1) | 1];
    }

    long long querySub(int x, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft > nodeRight || x < nodeLeft || x > nodeRight)
            return INF;

        pushDown(node, nodeLeft, nodeRight);
        if (nodeLeft == nodeRight)
            return treeLazy[node].get(x);

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        if (x <= mid)
            return querySub(x, (node << 1), nodeLeft, mid);
        else
            return querySub(x, (node << 1) | 1, mid + 1, nodeRight);
    }
};
