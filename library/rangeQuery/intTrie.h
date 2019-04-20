#pragma once

template <typename T, typename IntT = int, typename GetOp = function<IntT(T)>, typename MergeOp = function<T(T,T)>>
struct IntTrie {
    struct Node {
        int L;
        int R;
        T   value;
    };

    int         bitSize;
    GetOp       getOp;
    MergeOp     mergeOp;

    int nodeN;
    vector<Node> nodes;

    IntTrie(int bitSize, GetOp getOp, MergeOp mergeOp)
        : bitSize(bitSize), getOp(getOp), mergeOp(mergeOp), nodeN(0) {
    }

    void clear() {
        nodeN = 0;
        nodes.clear();
    }

    //--- build a new trie

    // PRECONDITION: v must be sorted
    int build(const T v[], int n) {
        return dfsBuild(v, 0, n - 1, IntT(1) << (bitSize - 1));
    }

    // PRECONDITION: v must be sorted
    int build(const vector<T>& v) {
        return build(v.data(), int(v.size()));
    }

    //--- add

    int add(int root, const T& value) {
        bool inserted = false;

        if (root < 0) {
            root = alloc();
            nodes[root].L = -1;
            nodes[root].R = -1;
            inserted = true;
        }

        int x = root;
        for (IntT bit = IntT(1) << (bitSize - 1); bit; bit >>= 1) {
            int x2;
            if (getOp(value) & bit) {
                x2 = nodes[x].R;
                if (x2 < 0) {
                    x2 = alloc();
                    nodes[x2].L = -1;
                    nodes[x2].R = -1;
                    nodes[x].R = x2;
                    inserted = true;
                }
            } else {
                x2 = nodes[x].L;
                if (x2 < 0) {
                    x2 = alloc();
                    nodes[x2].L = -1;
                    nodes[x2].R = -1;
                    nodes[x].L = x2;
                    inserted = true;
                }
            }
            x = x2;
        }

        if (inserted)
            nodes[x].value = value;
        else
            nodes[x].value = mergeOp(nodes[x].value, value);

        return root;
    }

    bool exist(int root, IntT key) const {
        int x = root;
        for (IntT bit = IntT(1) << (bitSize - 1); bit && x >= 0; bit >>= 1) {
            if (key & bit)
                x = nodes[x].R;
            else
                x = nodes[x].L;
        }

        return x >= 0;
    }

    pair<IntT, T> findMaxXor(int root, IntT k) const {
        int x = root;

        IntT res = 0;
        for (IntT bit = IntT(1) << (bitSize - 1); bit; bit >>= 1) {
            if (nodes[x].L < 0) {
                x = nodes[x].R;
                res |= bit;
            } else if (nodes[x].R < 0) {
                x = nodes[x].L;
            } else {
                if (k & bit)
                    x = nodes[x].L;
                else {
                    x = nodes[x].R;
                    res |= bit;
                }
            }
        }

        return make_pair(res, nodes[x].value);
    }

private:
    int alloc() {
        if (nodeN >= int(nodes.size()))
            nodes.resize(nodeN == 0 ? 1024 : nodeN * 2);
        return nodeN++;
    }

    int dfsBuild(const T v[], int L, int R, IntT bit) {
        int x = alloc();

        if (bit == 0) {
            nodes[x].L = -1;
            nodes[x].R = -1;
            nodes[x].value = v[L];
        } else if ((getOp(v[L]) & bit) == (getOp(v[R]) & bit)) {
            if (getOp(v[L]) & bit) {
                auto nodeR = dfsBuild(v, L, R, bit >> 1);
                nodes[x].L = -1;
                nodes[x].R = nodeR;
            } else {
                auto nodeL = dfsBuild(v, L, R, bit >> 1);
                nodes[x].L = nodeL;
                nodes[x].R = -1;
            }
        } else {
            int i = L, j = R;
            while (i <= j) {
                int mid = (i + j) >> 1;
                if (getOp(v[mid]) & bit)
                    j = mid - 1;
                else
                    i = mid + 1;
            }

            auto nodeL = dfsBuild(v, L, i - 1, bit >> 1);
            auto nodeR = dfsBuild(v, i, R, bit >> 1);

            nodes[x].L = nodeL;
            nodes[x].R = nodeR;
        }

        return x;
    }
};
