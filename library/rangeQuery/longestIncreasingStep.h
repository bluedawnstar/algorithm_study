#pragma once

// https://www.codechef.com/NOV20A/problems/UNSQUERS

// not LIS
struct LongestIncreasingStep {
    // O(N*logN)
    void init(const int A[], int n) {
        int root = segTree.build(n);
        roots.clear();

        vector<pair<int, int>> stk;
        stk.emplace_back(numeric_limits<int>::max(), -1);
        for (int i = 0; i < n; i++) {
            while (stk.back().first < A[i])
                stk.pop_back();
            root = segTree.add(root, stk.back().second + 1, i, 1);
            roots.push_back(root);

            stk.emplace_back(A[i], i);
        }
    }

    void init(const vector<int>& A) {
        init(A.data(), int(A.size()));
    }

    // O(logN)
    int query(int L, int R) {
        return segTree.query(roots[R], L, R);
    }

private:
    struct SimplePersistentSegmentTreeLazy {
        struct Node {
            int value;
            int L;
            int R;

            int lazy;
        };

        int             N;
        vector<Node>    nodes;
        int             initRoot;

        SimplePersistentSegmentTreeLazy()
            : N(0), nodes(), initRoot(-1) {
        }

        SimplePersistentSegmentTreeLazy(int n)
            : N(0), nodes(), initRoot(-1) {
            build(n);
        }

        // return root node index
        int build(int n) {
            nodes.clear();

            N = n;
            nodes.reserve(N * 4);
            initRoot = dfsBuild(0, N - 1);
            return initRoot;
        }

        //---

        int getInitRoot() const {
            return initRoot;
        }

        int add(int root, int left, int right, int val) {
            return dfsAdd(root, 0, N - 1, left, right, val);
        }

        int query(int root, int left, int right) {
            return dfsQuery(root, 0, N - 1, left, right);
        }

    private:
        int addNode(int value, int L, int R, int lazy) {
            nodes.push_back({ value, L, R, lazy });
            return int(nodes.size()) - 1;
        }

        int pushDown(int node, int nodeLeft, int nodeRight, int val) {
            if (val == 0)
                return node;

            int newValue = nodes[node].value;
            int lazy = nodes[node].lazy;
            int l = nodes[node].L;
            int r = nodes[node].R;

            if (nodeLeft == nodeRight) {
                newValue += val;
            } else {
                lazy += val;
                newValue += val;
            }
            return addNode(newValue, l, r, lazy);
        }

        int dfsBuild(int nodeLeft, int nodeRight) {
            int l = -1, r = -1;
            if (nodeLeft < nodeRight) {
                int mid = (nodeLeft + nodeRight) >> 1;
                l = dfsBuild(nodeLeft, mid);
                r = dfsBuild(mid + 1, nodeRight);
            }
            nodes.push_back({ 0, l, r, 0 });
            return int(nodes.size()) - 1;
        }

        int dfsAdd(int node, int nodeLeft, int nodeRight, int indexL, int indexR, int val) {
            if (indexR < nodeLeft || nodeRight < indexL)
                return node;

            int newValue = nodes[node].value;
            int l = nodes[node].L;
            int r = nodes[node].R;
            int lazy = nodes[node].lazy;

            if (nodeLeft == nodeRight) {
                newValue += val;
            } else {
                if (indexL <= nodeLeft && nodeRight <= indexR) {
                    lazy += val;
                    newValue += val;
                } else {
                    int mid = (nodeLeft + nodeRight) >> 1;
                    if (lazy != 0) {
                        l = pushDown(l, nodeLeft, mid, lazy);
                        r = pushDown(r, mid + 1, nodeRight, lazy);
                        lazy = 0;
                    }
                    l = dfsAdd(l, nodeLeft, mid, indexL, indexR, val);
                    r = dfsAdd(r, mid + 1, nodeRight, indexL, indexR, val);
                    newValue = max(nodes[l].value, nodes[r].value);
                }
            }
            return addNode(newValue, l, r, lazy);
        }

        int dfsQuery(int node, int nodeLeft, int nodeRight, int indexL, int indexR) {
            if (indexR < nodeLeft || nodeRight < indexL)
                return 0;

            if (indexL <= nodeLeft && nodeRight <= indexR)
                return nodes[node].value;

            int mid = (nodeLeft + nodeRight) >> 1;
            int l = nodes[node].L;
            int r = nodes[node].R;
            int lazy = nodes[node].lazy;
            if (lazy != 0) {
                l = pushDown(l, nodeLeft, mid, lazy);
                r = pushDown(r, mid + 1, nodeRight, lazy);

                nodes[node].value = max(nodes[l].value, nodes[r].value);
                nodes[node].L = l;
                nodes[node].R = r;
                nodes[node].lazy = 0;
            }

            return max(dfsQuery(l, nodeLeft, mid, indexL, indexR),
                dfsQuery(r, mid + 1, nodeRight, indexL, indexR));
        }
    };

    SimplePersistentSegmentTreeLazy segTree;
    vector<int> roots;
};
