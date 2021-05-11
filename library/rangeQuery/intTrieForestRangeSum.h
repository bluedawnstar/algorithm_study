#pragma once

/*
  Integer Trie Forest for Range-sum

  1. Precondition
    - KeyT must be an integer type  (ex: int, long long, ...)
*/
template <typename KeyT = int, typename ValueT = int>
struct IntTrieForestRangeSum {
    struct Node {
        int next[2];    // (L, R)
        ValueT value;

        Node() : value(0) {
            next[0] = next[1] = -1;
        }

        explicit Node(ValueT value) : value(value) {
            next[0] = next[1] = -1;
        }

        void init(ValueT value) {
            next[0] = next[1] = -1;
            this->value = value;
        }

        void add(ValueT x) {
            value += x;
        }
    };

    int bitN;
    vector<Node> nodes;

    IntTrieForestRangeSum() : bitN(0) {
    }

    explicit IntTrieForestRangeSum(int bitSize) : bitN(bitSize) {
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
    void insert(int root, KeyT x, ValueT value) {
        int cur = root;
        for (int i = bitN - 1; i >= 0; i--) {
            int bit = int((x >> i) & 1);
            if (nodes[cur].next[bit] < 0) {
                int tmp = newNode();
                nodes[cur].next[bit] = tmp;
            }
            cur = nodes[cur].next[bit];
            nodes[cur].add(value);
        }
    }


    // O(bitN)
    pair<ValueT, bool> get(int root, KeyT x) {
        if (x & ~((KeyT(1) << bitN) - 1))
            return make_pair(ValueT(0), false);

        int cur = root;
        for (int i = bitN - 1; i >= 0; i--) {
            int bit = int((x >> i) & 1);
            if (nodes[cur].next[bit] < 0)
                return make_pair(ValueT(0), false);
            else
                cur = nodes[cur].next[bit];
        }
        return make_pair(nodes[cur].value, true);
    }

    // inclusive, (i <= x), O(bitN)
    ValueT sumLessThanOrEqual(int root, KeyT x) {
        if (x & ~((KeyT(1) << bitN) - 1))
            return 0;

        int cur = root;
        ValueT ans = 0;
        for (int i = bitN - 1; i >= 0; i--) {
            int bit = int((x >> i) & 1);
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
    ValueT sumGreaterThanOrEqual(int root, KeyT x) {
        if (x & ~((KeyT(1) << bitN) - 1))
            return 0;

        int cur = root;
        ValueT ans = 0;
        for (int i = bitN - 1; i >= 0; i--) {
            int bit = int((x >> i) & 1);
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
    ValueT sumRange(int root, KeyT left, KeyT right) {
        return sumLessThanOrEqual(root, right) - sumLessThanOrEqual(root, left - 1);
    }

private:
    int newNode() {
        int res = int(nodes.size());
        nodes.emplace_back(0);
        return res;
    }
};
