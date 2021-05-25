#pragma once

struct LineSegmentIntegralTreeLazy {
    struct Node {
        int     x;              // x
        int     width;          // width
        // for terminal node
        int     lineA;          // y = a * x + b
        int     lineB;

        long long sum;
        int     lazy;

        void init(int x, int width, int a, int b) {
            this->x = x;
            this->width = width;
            this->lineA = a;
            this->lineB = b;
            sum = 0;
            lazy = 0;
        }

        void initSum() {
            if (lineA == 0)
                sum = 1ll * width * lineB;
            else {
                long long h = min(1ll * lineA * x + lineB, 1ll * lineA * (x + width - 1) + lineB);
                sum = width * h + abs(lineA) * long long(width - 1) * width / 2;
            }
        }

        //---

        long long sumLeft(int xx) {
            int w = xx - x + 1;
            if (lineA == 0)
                return 1ll * w * lineB;
            else {
                long long h = min(1ll * lineA * x + lineB, 1ll * lineA * xx + lineB);
                return w * h + abs(lineA) * long long(w - 1) * w / 2;
            }
        }

        long long sumRight(int xx) {
            int w = x + width - xx;
            if (lineA == 0)
                return 1ll * w * lineB;
            else {
                long long h = min(1ll * lineA * xx + lineB, 1ll * lineA * (x + width - 1) + lineB);
                return w * h + abs(lineA) * long long(w - 1) * w / 2;
            }
        }

        //---

        void addLeaf(int delta) {
            lineB += delta;
            sum += 1ll * width * delta;
        }

        void addLazy(int delta) {
            lazy += delta;
            sum += 1ll * width * delta;
        }
    };

    int N;                      // the size of array
    vector<Node> tree;
    vector<int> leaves;

    LineSegmentIntegralTreeLazy() : N(0) {
    }

    LineSegmentIntegralTreeLazy(const tuple<int, int, int, int> v[], int n) {
        build(v, n);
    }

    LineSegmentIntegralTreeLazy(const vector<tuple<int, int, int, int>>& v) {
        build(v.data(), int(v.size()));
    }


    // v = { (x, width, lineA, lineB), ... }
    void build(const tuple<int, int, int, int> v[], int n) {
        N = n;
        tree = vector<Node>(n * 4);
        leaves = vector<int>(n);

        buildSub(v, 0, n - 1, 1);
    }

    void build(const vector<tuple<int, int, int, int>>& v) {
        build(v.data(), int(v.size()));
    }

    //---

    // inclusive
    void add(int left, int right, int delta) {
        addSub(left, right, delta, 1, 0, N - 1);
    }

    // inclusive
    long long query(int left, int right) {
        return querySub(left, right, 1, 0, N - 1);
    }


    // K >= 1
    int findKthX(long long K) {
        if (tree[1].sum < K)
            return -1;
        return findKthXSub(K, 1, 0, N - 1);
    }

private:
    // inclusive
    void buildSub(const tuple<int, int, int, int> v[], int left, int right, int node) {
        if (left == right) {
            tree[node].init(get<0>(v[left]), get<1>(v[left]), get<2>(v[left]), get<3>(v[left]));
            tree[node].initSum();
            leaves[left] = node;
            return;
        }

        int mid = left + (right - left) / 2;
        int nodeL = node * 2;
        int nodeR = nodeL + 1;
        buildSub(v, left, mid, nodeL);
        buildSub(v, mid + 1, right, nodeR);
        tree[node].init(tree[nodeL].x, tree[nodeL].width + tree[nodeR].width, 0, 0);
        tree[node].sum = tree[nodeL].sum + tree[nodeR].sum;
    }

    void pushDown(int delta, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight) {
            tree[node].addLeaf(delta);
            return;
        }
        tree[node].addLazy(delta);
    }

    void addSub(int left, int right, int delta, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return;

        if (nodeLeft == nodeRight) {
            tree[node].addLeaf(delta);
            return;
        }

        if (left <= nodeLeft && nodeRight <= right) {
            tree[node].addLazy(delta);
            return;
        }

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        int nodeL = node * 2;
        int nodeR = nodeL + 1;
        if (tree[node].lazy) {
            pushDown(tree[node].lazy, nodeL, nodeLeft, mid);
            pushDown(tree[node].lazy, nodeR, mid + 1, nodeRight);
            tree[node].lazy = 0;
        }

        addSub(left, right, delta, nodeL, nodeLeft, mid);
        addSub(left, right, delta, nodeR, mid + 1, nodeRight);
        tree[node].sum = tree[nodeL].sum + tree[nodeR].sum;
    }

    long long querySub(int left, int right, int node, int nodeLeft, int nodeRight) {
        if (right < nodeLeft || nodeRight < left)
            return 0;

        if (left <= nodeLeft && nodeRight <= right)
            return tree[node].sum;

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        int nodeL = node * 2;
        int nodeR = nodeL + 1;
        if (tree[node].lazy) {
            pushDown(tree[node].lazy, nodeL, nodeLeft, mid);
            pushDown(tree[node].lazy, nodeR, mid + 1, nodeRight);
            tree[node].lazy = 0;
        }

        return querySub(left, right, nodeL, nodeLeft, mid) +
            querySub(left, right, nodeR, mid + 1, nodeRight);
    }

    int findKthXSub(long long K, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft == nodeRight) {
            int lo = tree[node].x;
            int hi = tree[node].x + tree[node].width - 1;
            while (lo <= hi) {
                int mid = lo + ((hi - lo) >> 1);
                if (tree[node].sumLeft(mid) < K)
                    lo = mid + 1;
                else
                    hi = mid - 1;
            }
            return lo;
        }

        int mid = nodeLeft + (nodeRight - nodeLeft) / 2;
        int nodeL = node * 2;
        int nodeR = nodeL + 1;
        if (tree[node].lazy) {
            pushDown(tree[node].lazy, nodeL, nodeLeft, mid);
            pushDown(tree[node].lazy, nodeR, mid + 1, nodeRight);
            tree[node].lazy = 0;
        }

        if (K <= tree[nodeL].sum)
            return findKthXSub(K, nodeL, nodeLeft, mid);
        else
            return findKthXSub(K - tree[nodeL].sum, nodeR, mid + 1, nodeRight);
    }
};
