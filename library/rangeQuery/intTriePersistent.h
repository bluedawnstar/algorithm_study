#pragma once

/*
   <How To Use>

    1. build tries
       vector<int> roots(N + 1);
       PersistentIntTrie<int> trie(BIT_SIZE, ...);

       trie.clear();

       roots[0] = -1;
       for (int i = 0; i < N; i++)
           roots[i + 1] = trie.add(roots[i], values[i]);

    2. query in range [L, R]
       bool exists = trie.exist(roots[L], roots[R + 1], x);
       auto result = trie.findMaxXor(roots[L], roots[R + 1], k);
*/
// Related problems
// https://www.codechef.com/problems/XORMIN

template <typename T, typename IntT = int, typename GetOp = function<IntT(T)>, typename MergeOp = function<T(T,T)>>
struct PersistentIntTrie {
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

    PersistentIntTrie(int bitSize, GetOp getOp, MergeOp mergeOp)
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

    // if there is no node, then set root as -1
    int add(int root, const T& value) {
        return dfsAdd(root, value, IntT(1) << (bitSize - 1));
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

    // check if key exists in [L, R]
    // - rootL = root of time (L - 1)
    // - rootR = root of time R
    bool exist(int rootL, int rootR, IntT key) const {
        int xL = rootL;
        int xR = rootR;
        for (IntT bit = IntT(1) << (bitSize - 1); bit && xL < xR; bit >>= 1) {
            if (key & bit) {
                xL = (xL >= 0) ? nodes[xL].R : -1;
                xR = (xR >= 0) ? nodes[xR].R : -1;
            } else {
                xL = (xL >= 0) ? nodes[xL].L : -1;
                xR = (xR >= 0) ? nodes[xR].L : -1;
            }
        }
        return xL < xR;
    }

    // find a number to make a maximum xor value with k
    pair<IntT, T> findMaxXor(int root, IntT k) const {
        int x = root;

        IntT res = 0;
        for (IntT bit = IntT(1) << (bitSize - 1); bit; bit >>= 1) {
            if ((nodes[x].L < 0) || (nodes[x].R >= 0 && !(k & bit))) {
                x = nodes[x].R;
                res |= bit;
            } else {
                x = nodes[x].L;
            }
        }

        return make_pair(res, nodes[x].value);
    }

    // find a number to make a minimum xor value with k
    pair<IntT, T> findMinXor(int root, IntT k) const {
        int x = root;

        IntT res = 0;
        for (IntT bit = IntT(1) << (bitSize - 1); bit; bit >>= 1) {
            if ((nodes[x].L < 0) || (nodes[x].R >= 0 && (k & bit))) {
                x = nodes[x].R;
                res |= bit;
            } else {
                x = nodes[x].L;
            }
        }

        return make_pair(res, nodes[x].value);
    }

    // find a number to make a maximum xor value with k in [L, R]
    // - rootL = root of time (L - 1)
    // - rootR = root of time R
    pair<IntT, T> findMaxXor(int rootL, int rootR, IntT k) const {
        int xL = rootL;
        int xR = rootR;

        IntT res = 0;
        for (IntT bit = IntT(1) << (bitSize - 1); bit; bit >>= 1) {
            int xLL = (xL >= 0) ? nodes[xL].L : -1;
            int xLR = (xL >= 0) ? nodes[xL].R : -1;
            int xRL = (xR >= 0) ? nodes[xR].L : -1;
            int xRR = (xR >= 0) ? nodes[xR].R : -1;
            if (xLL >= xRL || (xLR < xRR && !(k & bit))) {
                xL = xLR;
                xR = xRR;
                res |= bit;
            } else {
                xL = xLL;
                xR = xRL;
            }
        }

        return make_pair(res, nodes[xR].value);
    }

    // find a number to make a minimum xor value with k in [L, R]
    // - rootL = root of time (L - 1)
    // - rootR = root of time R
    pair<IntT, T> findMinXor(int rootL, int rootR, IntT k) const {
        int xL = rootL;
        int xR = rootR;

        IntT res = 0;
        for (IntT bit = IntT(1) << (bitSize - 1); bit; bit >>= 1) {
            int xLL = (xL >= 0) ? nodes[xL].L : -1;
            int xLR = (xL >= 0) ? nodes[xL].R : -1;
            int xRL = (xR >= 0) ? nodes[xR].L : -1;
            int xRR = (xR >= 0) ? nodes[xR].R : -1;
            if (xLL >= xRL || (xLR < xRR && (k & bit))) {
                xL = xLR;
                xR = xRR;
                res |= bit;
            } else {
                xL = xLL;
                xR = xRL;
            }
        }

        return make_pair(res, nodes[xR].value);
    }

private:
    int alloc() {
        if (nodeN >= int(nodes.size()))
            nodes.resize(nodeN == 0 ? 1024 : nodeN * 2);

        nodes[nodeN].L = -1;
        nodes[nodeN].R = -1;

        return nodeN++;
    }

    int dfsBuild(const T v[], int L, int R, IntT bit) {
        int x = alloc();

        if (bit == 0) {
            nodes[x].value = v[L];
            return x;
        }

        int mid;
        if ((getOp(v[L]) & bit) == (getOp(v[R]) & bit)) {
            mid = (getOp(v[L]) & bit) ? L : R + 1;
        } else {
            int i = L, j = R;
            while (i <= j) {
                int mid = (i + j) >> 1;
                if (getOp(v[mid]) & bit)
                    j = mid - 1;
                else
                    i = mid + 1;
            }
            mid = i;
        }

        if (L < mid) {
            auto nodeL = dfsBuild(v, L, mid - 1, bit >> 1);
            nodes[x].L = nodeL;
        }
        if (mid <= R) {
            auto nodeR = dfsBuild(v, mid, R, bit >> 1);
            nodes[x].R = nodeR;
        }

        return x;
    }

    int dfsAdd(int x, const T& value, IntT bit) {
        int x2 = alloc();

        if (bit == 0) {
            if (x < 0)
                nodes[x2].value = value;
            else
                nodes[x2].value = mergeOp(nodes[x].value, value);
        } else {
            if (getOp(value) & bit) {
                auto nodeR = dfsAdd(x >= 0 ? nodes[x].R : -1, value, bit >> 1);
                nodes[x2].L = (x >= 0) ? nodes[x].L : -1;
                nodes[x2].R = nodeR;
            } else {
                auto nodeL = dfsAdd(x >= 0 ? nodes[x].L : -1, value, bit >> 1);
                nodes[x2].L = nodeL;
                nodes[x2].R = (x >= 0) ? nodes[x].R : -1;
            }
        }

        return x2;
    }
};
