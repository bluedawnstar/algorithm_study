#pragma once


/*
    Bit Trie Forest for Range-Sum
*/
template <typename T>
struct BitTrieForest {
    struct Node {
        int next[2];
        T value;

        Node() : value(0) {
            next[0] = next[1] = -1;
        }

        explicit Node(T value) : value(value) {
            next[0] = next[1] = -1;
        }

        void init(int value) {
            next[0] = next[1] = -1;
            this->sum = value;
        }

        void add(T x) {
            value += x;
        }
    };

    int bitN;
    vector<Node> nodes;

    BitTrieForest() : bitN(0) {
    }

    explicit BitTrieForest(int bitSize) : bitN(bitSize) {
    }

    void clear() {
        nodes.clear();
    }


    void init(int bitSize) {
        bitN = bitSize;
    }


    int createTrie() {
        return newNode();
    }

    // O(bitN)
    void insert(int root, int x, T value) {
        int cur = root;
        for (int i = bitN - 1; i >= 0; i--) {
            int bit = (x >> i) & 1;
            if (nodes[cur].next[bit] < 0) {
                int tmp = newNode();
                nodes[cur].next[bit] = tmp;
            }
            cur = nodes[cur].next[bit];
            nodes[cur].add(value);
        }
    }


    // O(bitN)
    pair<T, bool> get(int root, int x) {
        int cur = root;
        for (int i = bitN - 1; i >= 0; i--) {
            int bit = (x >> i) & 1;
            if (nodes[cur].next[bit] < 0)
                return make_pair(T(0), false);
            else
                cur = nodes[cur].next[bit];
        }
        return make_pair(nodes[cur].value, true);
    }

    // inclusive, (i <= x), O(bitN)
    T sumLessThanOrEqual(int root, int x) {
        int cur = root;
        T ans = 0;
        for (int i = bitN - 1; i >= 0; i--) {
            int bit = (x >> i) & 1;
            if (bit == 1 && nodes[cur].next[0] >= 0)
                ans += nodes[nodes[cur].next[0]].value;
            if (nodes[cur].next[bit] < 0)
                return ans;
            else
                cur = nodes[cur].next[bit];
        }
        ans += nodes[cur].value;
        return ans;
    }

    // inclusive, (x <= i), O(bitN)
    T sumGreaterThanOrEqual(int root, int x) {
        int cur = root;
        T ans = 0;
        for (int i = bitN - 1; i >= 0; i--) {
            int bit = (x >> i) & 1;
            if (bit == 0 && nodes[cur].next[1] >= 0)
                ans += nodes[nodes[cur].next[1]].value;
            if (nodes[cur].next[bit] < 0)
                return ans;
            else
                cur = nodes[cur].next[bit];
        }
        ans += nodes[cur].value;
        return ans;
    }

    // inclusive, (left <= i <= right), O(bitN)
    T sumRange(int root, int left, int right) {
        return sumLessThanOrEqual(root, right) - sumLessThanOrEqual(root, left - 1);
    }

private:
    int newNode() {
        int res = int(nodes.size());
        nodes.emplace_back(0);
        return res;
    }
};
