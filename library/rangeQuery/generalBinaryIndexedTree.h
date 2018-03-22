#pragma once

//--------- General Fenwick Tree (Binary Indexed Tree) --------------------------------

template <typename T, typename BinOp = function<T(T, T)>>
struct GeneralBIT {
    int N;                  // power of 2
    vector<T> tree;         // forward BIT
    vector<T> treeR;        // backward BIT

    T       defaultValue;
    BinOp   mergeOp;

    explicit GeneralBIT(BinOp op, T dflt = T())
        : N(0), mergeOp(op), defaultValue(dflt) {
    }

    GeneralBIT(int n, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        init(n);
    }

    GeneralBIT(const T value, int n, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(value, n);
    }

    GeneralBIT(const T arr[], int n, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(arr, n);
    }

    GeneralBIT(const vector<T>& v, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(v);
    }


    void init(int n) {
        N = 1;
        while (N < n)
            N <<= 1;

        tree = vector<T>(N + 1, defaultValue);
        treeR = vector<T>(N, defaultValue);
    }

    void build(T value, int n) {
        init(n);
        for (int i = 0; i < n; i++)
            initUpdate(i, value);
    }

    void build(const T arr[], int n) {
        init(n);
        for (int i = 0; i < n; i++)
            initUpdate(i, arr[i]);
    }

    void build(const vector<T>& v) {
        build(&v[0], (int)v.size());
    }


    void add(int pos, T val) {
        update(pos, query(pos) + val);
    }

    void update(int pos, T val) {
        int i, j;
        if (pos & 1) {
            i = j = pos;
            treeR[i] = val;
        } else {
            i = j = pos + 1;
            tree[i] = val;
        }

        int mask = 2;
        while (i < N && j > 0) {
            int half = mask >> 1;
            if ((i & mask) == 0) {
                i += i & -i;
                tree[i] = mergeOp(tree[i - half], treeR[i - half]);
            } 
            if (j & mask) {
                j &= j - 1;
                treeR[j] = mergeOp(tree[j + half], treeR[j + half]);
            }
            mask <<= 1;
        }
    }


    // inclusive
    T query(int pos) const {
        return (pos & 1) ? treeR[pos] : tree[pos + 1];
    }

    // inclusive
    T queryRange(int left, int right) const {
        T res = defaultValue;

        int R = right + 1;
        int nextR = R & (R - 1);
        while (0 < R && left <= nextR) {
            res = mergeOp(res, tree[R]);
            R = nextR;
            nextR = R & (R - 1);
        }

        int L = left;
        while (L < R) {
            res = mergeOp(res, treeR[L]);
            L += L & -L;
        }

        return res;
    }

private:
    void initUpdate(int pos, T val) {
        for (int i = pos + 1; i <= N; i += i & -i)
            tree[i] = mergeOp(tree[i], val);

        for (int i = pos; i > 0; i &= i - 1)
            treeR[i] = mergeOp(treeR[i], val);
    }
};

template <typename T, typename BinOp>
GeneralBIT<T, BinOp> makeGeneralBIT(int size, BinOp op, T dfltValue = T()) {
    return GeneralBIT<T, BinOp>(size, op, dfltValue);
}

template <typename T, typename BinOp>
GeneralBIT<T, BinOp> makeGeneralBIT(const T arr[], int size, BinOp op, T dfltValue = T()) {
    return GeneralBIT<T, BinOp>(arr, size, op, dfltValue);
}

template <typename T, typename BinOp>
GeneralBIT<T, BinOp> makeGeneralBIT(const vector<T>& v, BinOp op, T dfltValue = T()) {
    return GeneralBIT<T, BinOp>(v, op, dfltValue);
}
