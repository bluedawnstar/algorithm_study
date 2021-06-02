#pragma once

/*
  Integer Trie Forest for range-operations

  1. template parameters
    - KeyT     : key type for indexing
                 It must be an integer type (ex: int, long long, ...)
    - ValueT   : value type
    - MergeOp  : merge two values at internal nodes
    - UpdateOp : merge two values at terminal nodes if previous value of the key exists

  2. how to use
    1) build a tree
        IntTrieForest<int,int> trie(BIT_SIZE, [](int a, int b) { return ...; }, [](int a, int b) { return ...; });

        int root = trie.build(...);
        //...
        trie.insert(root, ...);
        //...
        int ans = trie.query(root, L, R, 0, [](int a, int b) { return ... });
*/
template <typename KeyT = int, typename ValueT = int,
          typename MergeOp = function<ValueT(ValueT,ValueT)>,
          typename UpdateOp = function<ValueT(ValueT, ValueT)>>
struct IntTrieForest {
    struct Node {
        int     next[2];
        ValueT  value;

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
    };

    int         bitN;
    MergeOp     mergeOp;
    UpdateOp    updateOp;
    vector<Node> nodes;

    IntTrieForest() : bitN(0) {
    }

    IntTrieForest(int bitN, MergeOp mergeOp, UpdateOp updateOp)
        : bitN(bitN), mergeOp(mergeOp), updateOp(updateOp) {
    }


    void init(int bitN, MergeOp mergeOp, UpdateOp updateOp) {
        this->bitN = bitN;
        this->mergeOp = mergeOp;
        this->updateOp = updateOp;
    }


    void clear() {
        nodes.clear();
    }


    //--- build a new trie

    int build() {
        return newNode();
    }

    // PRECONDITION: v must be sorted
    int build(const pair<KeyT,ValueT> v[], int n) {
        return dfsBuild(v, 0, n - 1, KeyT(1) << (bitN - 1));
    }

    // PRECONDITION: v must be sorted
    int build(const vector<pair<KeyT, ValueT>>& v) {
        return build(v.data(), int(v.size()));
    }


    //--- add

    void insert(int root, KeyT x, const ValueT& value) {
        bool inserted = false;

        int cur = root;
        for (int i = bitN - 1; i >= 0; i--) {
            int bit = int((x >> i) & 1);
            if (nodes[cur].next[bit] < 0) {
                int next = newNode();
                nodes[cur].next[bit] = next;
                inserted = true;
            }
            cur = nodes[cur].next[bit];
            if (inserted)
                nodes[cur].value = value;
            else if (i > 0)
                nodes[cur].value = mergeOp(nodes[cur].value, value);
            else
                nodes[cur].value = updateOp(nodes[cur].value, value);
        }
    }


    //--- find

    bool exist(int root, KeyT x) const {
        int cur = root;
        for (int i = bitN - 1; i >= 0 && cur >= 0; i--) {
            int bit = int((x >> i) & 1);
            cur = nodes[cur].next[bit];
        }
        return cur >= 0;
    }

    // O(bitN)
    pair<ValueT, bool> get(int root, KeyT x) const {
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


    //--- range query

    // inclusive, O(bitN)
    template <typename T, typename QueryMergeOp = function<T(T, ValueT)>>
    T query(int root, KeyT left, KeyT right, T initValue, QueryMergeOp queryMergeOp) const {
        T res = initValue;
        if (left > right)
            return res;

        KeyT L = 0;
        KeyT R = 0;

        int curL = root;
        int curR = root;
        for (int i = bitN - 1; i >= 0 && (curL >= 0 || curR >= 0); i--) {
            int bitL = int((left >> i) & 1);
            int bitR = int((right >> i) & 1);

            if (L == R) {
                curL = nodes[curL].next[bitL];
                curR = nodes[curR].next[bitR];
            } else {
                if (curL >= 0) {
                    if (!bitL && nodes[curL].next[1] >= 0)
                        res = queryMergeOp(res, nodes[nodes[curL].next[1]].value);
                    curL = nodes[curL].next[bitL];
                }
                if (curR >= 0) {
                    if (bitR && nodes[curR].next[0] >= 0)
                        res = queryMergeOp(res, nodes[nodes[curR].next[0]].value);
                    curR = nodes[curR].next[bitR];
                }
            }

            L = (L << 1) | KeyT(bitL);
            R = (R << 1) | KeyT(bitR);
        }

        if (curL >= 0)
            res = queryMergeOp(res, nodes[curL].value);
        if (curL != curR && curR >= 0)
            res = queryMergeOp(res, nodes[curR].value);

        return res;
    }


    //--- find min/max XOR

    // find a number to make a maximum xor value with k
    pair<KeyT, ValueT> findMaxXor(int root, KeyT k) const {
        KeyT res = 0;

        int cur = root;
        for (KeyT bit = KeyT(1) << (bitN - 1); bit; bit >>= 1) {
            if (nodes[cur].next[0] < 0) {
                cur = nodes[cur].next[1];
                res |= bit;
            } else if (nodes[cur].next[1] < 0) {
                cur = nodes[cur].next[0];
            } else {
                if (k & bit)
                    cur = nodes[cur].next[0];
                else {
                    cur = nodes[cur].next[1];
                    res |= bit;
                }
            }
        }

        return make_pair(res, nodes[cur].value);
    }

    // find a number to make a minimum xor value with k
    pair<KeyT, ValueT> findMinXor(int root, KeyT k) const {
        KeyT res = 0;

        int cur = root;
        for (KeyT bit = KeyT(1) << (bitN - 1); bit; bit >>= 1) {
            if (nodes[cur].next[0] < 0) {
                cur = nodes[cur].next[1];
                res |= bit;
            } else if (nodes[cur].next[1] < 0) {
                cur = nodes[cur].next[0];
            } else {
                if (!(k & bit))
                    cur = nodes[cur].next[0];
                else {
                    cur = nodes[cur].next[1];
                    res |= bit;
                }
            }
        }

        return make_pair(res, nodes[cur].value);
    }

private:
    int newNode() {
        int res = int(nodes.size());
        nodes.emplace_back(0);
        return res;
    }

    int dfsBuild(const pair<KeyT,ValueT> v[], int L, int R, KeyT bit) {
        int cur = newNode();

        if (bit == 0) {
            nodes[cur].value = v[L].second;
            return cur;
        }

        int mid;
        if ((v[L].first & bit) == (v[R].first & bit)) {
            mid = (v[L].first & bit) ? L : R + 1;
        } else {
            int i = L, j = R;
            while (i <= j) {
                int mid = (i + j) >> 1;
                if (v[mid].first & bit)
                    j = mid - 1;
                else
                    i = mid + 1;
            }
            mid = i;
        }

        if (L < mid) {
            auto nodeL = dfsBuild(v, L, mid - 1, bit >> 1);
            nodes[cur].next[0] = nodeL;
        }
        if (mid <= R) {
            auto nodeR = dfsBuild(v, mid, R, bit >> 1);
            nodes[cur].next[1] = nodeR;
        }

        return cur;
    }
};
