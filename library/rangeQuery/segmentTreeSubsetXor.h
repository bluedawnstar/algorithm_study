#pragma once

// in vector space
template <int maxBitSize, int mod = 1000000007>
struct SubsetXorSegmentTree {
    struct Node {
        int basis[maxBitSize];
        int basisCount;

        Node() {
            init();
        }

        void init() {
            memset(basis, 0, sizeof(basis));
            basisCount = 0;
        }

        bool insert(int x) {
            if (basisCount >= maxBitSize)
                return false;

            for (int i = 0, mask = 1; i < maxBitSize; i++, mask <<= 1) {
                if ((x & mask) == 0)
                    continue;

                if (!basis[i]) {
                    // If there is no basis vector with the i'th bit set,
                    //  then insert this vector into the basis
                    basis[i] = x;
                    ++basisCount;
                    return true;
                }
                x ^= basis[i];  // otherwise subtract the basis vector from this vector
            }

            return false;
        }

        void merge(const Node& a, const Node& b) {
            if (a.basisCount > b.basisCount) {
                memcpy(basis, a.basis, sizeof(basis));
                basisCount = a.basisCount;
                for (int i = 0; i < maxBitSize; i++) {
                    if (b.basis[i])
                        insert(b.basis[i]);
                }
            } else {
                memcpy(basis, b.basis, sizeof(basis));
                basisCount = b.basisCount;
                for (int i = 0; i < maxBitSize; i++) {
                    if (a.basis[i])
                        insert(a.basis[i]);
                }
            }
        }

        // the number of subsets whose XOR value is the target.
        bool checkXor(int target) const {
            for (int i = 0, mask = 1; i < maxBitSize; i++, mask <<= 1) {
                if ((target & mask) == 0)
                    continue;

                if (!basis[i])
                    return false;

                target ^= basis[i];
            }
            return true;
        }
    };

    int N;
    vector<int> pow2;

    vector<Node> nodes;
    Node emptyNode;

    SubsetXorSegmentTree() {
    }

    explicit SubsetXorSegmentTree(int n) {
        init(n);
    }

    SubsetXorSegmentTree(const int A[], int n) {
        build(A, n);
    }

    explicit SubsetXorSegmentTree(const vector<int>& A) {
        build(A);
    }

    //--- initialize & build

    void init(int n) {
        this->N = n;
        nodes = vector<Node>(n * 4);

        pow2.resize(n + 1);
        pow2[0] = 1;
        for (int i = 1; i <= n; i++)
            pow2[i] = int(2ll * pow2[i - 1] % mod);
    }

    void build(const int A[], int n) {
        init(n);
        dfsBuild(A, 1, 0, N - 1);
    }

    void build(const vector<int>& A) {
        build(A.data(), int(A.size()));
    }

    //--- update

    void update(int index, int x) {
        dfsUpdate(index, x, 1, 0, N - 1);
    }

    //--- query

    Node query(int left, int right) const {
        return dfsQuery(left, right, 1, 0, N - 1);
    }

    // the number of subsets whose XOR value is the target.
    // [CAUTION] if target is 0, then result includes empty set
    int countSubsetXor(int left, int right, int target) const {
        auto node = dfsQuery(left, right, 1, 0, N - 1);

        int res = 0;
        if (node.checkXor(target))
            res = pow2[(right - left + 1) - node.basisCount];
        return res;
    }

    // the number of distinct integers that can be represented using XOR over the subset of the given elements.
    int countDistinctXorIntegers(int left, int right) const {
        auto node = dfsQuery(left, right, 1, 0, N - 1);
        return pow2[node.basisCount];   // 2^basisCount
    }

    // the maximum possible XOR of the elements of some subset
    int getMaxSubsetXor(int left, int right) const {
        auto node = dfsQuery(left, right, 1, 0, N - 1);

        int res = 0;
        for (int i = maxBitSize - 1; i >= 0; i--) {
            if (!node.basis[i])
                continue;

            if (res & (1 << i))
                continue;

            res ^= node.basis[i];
        }

        return res;
    }

    // the Kth number from the set of all possible XOR of elements from a subset
    // 1 <= kth <= |set|
    int kthSubsetXor(int left, int right, int kth) const {
        auto node = dfsQuery(left, right, 1, 0, N - 1);

        int res = 0;

        int total = 1 << node.basisCount;
        for (int i = node.basisCount - 1; i >= 0; i--) {
            if (node.basis[i]) {
                int low = total >> 1;

                if ((low < kth && (res & (1 << i)) == 0) ||
                    (low >= kth && (res & (1 << i)) > 0))
                    res ^= node.basis[i];

                if (low < kth)
                    kth -= low;

                total >>= 1;
            }
        }

        return res;
    }

private:
    void dfsBuild(const int A[], int node, int left, int right) {
        if (left == right) {
            nodes[node].insert(A[left]);
            return;
        }
        int mid = (left + right) >> 1;
        dfsBuild(A, node * 2, left, mid);
        dfsBuild(A, node * 2 + 1, mid + 1, right);
        nodes[node].merge(nodes[node * 2], nodes[node * 2 + 1]);
    }

    Node dfsQuery(int left, int right, int node, int nodeLeft, int nodeRight) const {
        if (nodeLeft > right || nodeRight < left)
            return emptyNode;

        if (left <= nodeLeft && nodeRight <= right)
            return nodes[node];

        int mid = (nodeLeft + nodeRight) >> 1;

        Node res;
        res.merge(dfsQuery(left, right, node * 2, nodeLeft, mid),
                  dfsQuery(left, right, node * 2 + 1, mid + 1, nodeRight));
        return res;
    }

    void dfsUpdate(int index, int x, int node, int nodeLeft, int nodeRight) {
        if (nodeLeft > index || nodeRight < index)
            return;

        if (nodeLeft == index && index == nodeRight) {
            nodes[node].init();
            nodes[node].insert(x);
            return;
        }

        int mid = (nodeLeft + nodeRight) >> 1;
        dfsUpdate(index, x, node * 2, nodeLeft, mid);
        dfsUpdate(index, x, node * 2 + 1, mid + 1, nodeRight);
        nodes[node].merge(nodes[node * 2], nodes[node * 2 + 1]);
    }
};
