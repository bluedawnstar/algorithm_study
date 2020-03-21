#pragma once

// https://discuss.codechef.com/t/goodsegs-editorial/56718
// https://oi-wiki.org/ds/divide-combine/

struct DivideCombineTree {
    struct SparseTableMinMax {
        int N;
        vector<vector<pair<int, int>>> value;
        vector<int> H;

        SparseTableMinMax() {
        }

        void build(const int a[], int n) {
            this->N = n;

            H.resize(n + 1);
            H[1] = 0;
            for (int i = 2; i < int(H.size()); i++)
                H[i] = H[i >> 1] + 1;

            value.assign(H.back() + 1, vector<pair<int, int>>(n));
            for (int i = 0; i < n; i++) {
                value[0][i].first = a[i];
                value[0][i].second = a[i];
            }

            for (int i = 1; i < int(value.size()); i++) {
                auto& prev = value[i - 1];
                auto& curr = value[i];
                for (int v = 0; v < n; v++) {
                    if (v + (1 << (i - 1)) < n) {
                        curr[v].first = min(prev[v].first, prev[v + (1 << (i - 1))].first);
                        curr[v].second = max(prev[v].second, prev[v + (1 << (i - 1))].second);
                    } else {
                        curr[v] = prev[v];
                    }
                }
            }
        }

        void build(const vector<int>& a) {
            build(&a[0], int(a.size()));
        }

        // inclusive
        pair<int, int> query(int left, int right) const {
            right++;
            if (right <= left)
                return make_pair(INT_MAX, -INT_MIN);

            int k = H[right - left];
            const auto& mink = value[k];
            return make_pair(min(mink[left].first, mink[right - (1 << k)].first),
                max(mink[left].second, mink[right - (1 << k)].second));
        }
    };

    struct SegTreeMin {
        int N;
        vector<pair<int, int>> tree;    // (delta value of max - min, array index)
        vector<int> lazy;

        void init(int n) {
            N = n;
            tree = vector<pair<int, int>>(4 * n);
            lazy = vector<int>(4 * n);

            dfsInit(1, 0, n - 1);
        }

        // 0 <= left <= right < N, O(logN)
        void update(int left, int right, int x) {
            dfsUpdate(left, right, x, 1, 0, N - 1);
        }

        // 0 <= x < N, O(logN)
        int query(int x) {
            return dfsQuery(x, 1, 0, N - 1);
        }

        // (min value delta, first index)
        pair<int, int> query() {
            return tree[1];
        }

    private:
        void apply(int node, int x) {
            tree[node].first += x;
            lazy[node] += x;
        }

        void push(int node) {
            apply(2 * node, lazy[node]);
            apply(2 * node + 1, lazy[node]);
            lazy[node] = 0;
        }

        void combine(int node) {
            if (tree[2 * node].first <= tree[2 * node + 1].first)
                tree[node] = tree[2 * node];
            else
                tree[node] = tree[2 * node + 1];
        }

        void dfsInit(int node, int nodeLeft, int nodeRight) {
            if (nodeLeft == nodeRight) {
                tree[node].first = nodeLeft;
                tree[node].second = nodeLeft;
                return;
            }

            int mid = (nodeLeft + nodeRight) / 2;
            dfsInit(2 * node, nodeLeft, mid);
            dfsInit(2 * node + 1, mid + 1, nodeRight);
            combine(node);
        }

        void dfsUpdate(int left, int right, int x, int node, int nodeLeft, int nodeRight) {
            if (left <= nodeLeft && nodeRight <= right) {
                apply(node, x);
                return;
            }

            int mid = (nodeLeft + nodeRight) / 2;
            push(node);
            if (left <= mid)
                dfsUpdate(left, right, x, 2 * node, nodeLeft, mid);
            if (mid < right)
                dfsUpdate(left, right, x, 2 * node + 1, mid + 1, nodeRight);
            combine(node);
        }

        int dfsQuery(int index, int node, int nodeLeft, int nodeRight) {
            if (nodeLeft == nodeRight)
                return tree[node].first;

            int mid = (nodeLeft + nodeRight) / 2;
            push(node);
            if (index <= mid)
                return dfsQuery(index, 2 * node, nodeLeft, mid);
            else
                return dfsQuery(index, 2 * node + 1, mid + 1, nodeRight);
        }
    };

    struct Node {
        bool ok;                // combined node (L < R) and consecutive subarray (max[L..R] - min[L..R] == R - L)
        int L, R;               // array index
        vector<int> children;   // node indexes (reverse order)

        void addChild(int u) {
            children.push_back(u);
        }
    };

    SparseTableMinMax sptMinMax;
    SegTreeMin segTree;

    int N;

    int nodeCount;              // the number of nodes
    vector<Node> nodes;
    vector<int> indexToNode;    // array index to node (leaf node)

    int root;

    // O(N*logN)
    void build(const int P[], int n) {
        init(n);
        sptMinMax.build(P, n);

        vector<int> stk;    // tree node indexes
        stk.reserve(n);

        vector<int> nextUpper(n), nextLower(n);
        for (int i = 0; i < n; i++) {
            for (nextUpper[i] = i - 1; nextUpper[i] >= 0 && P[nextUpper[i]] < P[i]; nextUpper[i] = nextUpper[nextUpper[i]])
                segTree.update(nextUpper[nextUpper[i]] + 1, nextUpper[i], P[i] - P[nextUpper[i]]);

            for (nextLower[i] = i - 1; nextLower[i] >= 0 && P[nextLower[i]] > P[i]; nextLower[i] = nextLower[nextLower[i]])
                segTree.update(nextLower[nextLower[i]] + 1, nextLower[i], P[nextLower[i]] - P[i]);

            int first = segTree.query().second;

            int u = allocNode();
            indexToNode.push_back(u);
            nodes[u].L = i;
            nodes[u].R = i;
            while (!stk.empty() && first <= nodes[stk.back()].L) {
                int j = stk.back();
                stk.pop_back();
                if (nodes[j].ok && isIntersected(j, u))
                    u = mergeTo(j, u);
                else if (isCombinedOK(j, u))
                    u = merge(j, u);
                else
                    u = merge(stk, j, u);
            }
            stk.push_back(u);
        }

        root = stk[0];
    }

    void build(const vector<int> P) {
        return build(P.data(), int(P.size()));
    }

    //--- query

    // 0 <= left <= right < N, O(1)
    bool isConsecutiveSubarray(int left, int right) const {
        auto r = sptMinMax.query(left, right);
        return (r.second - r.first) == (right - left);
    }

    // the number of unique intersected consecutive subarray pairs (exclude nested pairs)
    long long countIntersectedPairs(int minOverlappedLen) const {
        return dfsCountIntersectedPairs(root, minOverlappedLen);
    }

private:
    void init(int n) {
        N = n;
        root = -1;
        segTree.init(n);

        nodeCount = 0;
        nodes = vector<Node>(2 * n);
        indexToNode.reserve(n);
    }

    int allocNode() {
        int u = nodeCount++;
        if (u >= int(nodes.size()))
            nodes.resize(nodes.size() * 2);
        nodes[u].ok = false;
        return u;
    }

    // merge child to node (last consecutive subarray)
    int mergeTo(int node, int child) {
        nodes[node].R = nodes[child].R;
        nodes[node].addChild(child);
        return node;
    }

    // merge and make new consecutive subarray
    int merge(int nodeL, int nodeR) {
        int node = allocNode();
        nodes[node].ok = true;
        nodes[node].L = nodes[nodeL].L;
        nodes[node].R = nodes[nodeR].R;
        nodes[node].addChild(nodeL);
        nodes[node].addChild(nodeR);
        return node;
    }

    int merge(vector<int>& stk, int nodeL, int nodeR) {
        int node = allocNode();

        nodes[node].addChild(nodeR);
        nodes[node].addChild(nodeL);
        while (segTree.query(nodes[nodeL].L) > nodes[nodeR].R) {
            nodeL = stk.back();
            stk.pop_back();
            nodes[node].addChild(nodeL);
        }

        nodes[node].L = nodes[nodeL].L;
        nodes[node].R = nodes[nodeR].R;
        return node;
    }

    // nodeL is OK, nodeR is not OK
    bool isIntersected(int nodeL, int nodeR) {
        nodeL = nodes[nodeL].children[1];
        //return segTree.query(nodes[nodeL].L) <= nodes[nodeR].R;
        return isConsecutiveSubarray(nodes[nodeL].L, nodes[nodeR].R);
    }

    bool isCombinedOK(int nodeL, int nodeR) {
        //return segTree.query(nodes[nodeL].L) <= nodes[nodeR].R;
        return isConsecutiveSubarray(nodes[nodeL].L, nodes[nodeR].R);
    }

    //---

    long long dfsCountIntersectedPairs(int u, int minIntersectLen) const {
        long long ans = 0;

        if (nodes[u].ok) {
            vector<int> childLength;
            for (int v : nodes[u].children)
                childLength.push_back(nodes[v].R - nodes[v].L + 1);

            // update the start of the intersection
            // and find minimum end of the intersection
            for (int i = 0, j = 0, intersectedLen = 0; ; i++) {
                while (j < childLength.size() && intersectedLen < minIntersectLen)
                    intersectedLen += childLength[j++];
                if (intersectedLen < minIntersectLen)
                    break;

                // i * n + i * (n - 1) + i * (n - 2) + ... + i * 1 = i * n * (n + 1) / 2
                int n = int(childLength.size() - j);
                ans += 1ll * i * n * (n + 1) / 2;
                intersectedLen -= childLength[i];
            }
        }

        for (int v : nodes[u].children)
            ans += dfsCountIntersectedPairs(v, minIntersectLen);

        return ans;
    }
};
