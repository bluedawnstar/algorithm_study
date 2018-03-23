#pragma once

//--------- Generalized Binary Indexed Tree (Generalized Fenwick Tree) --------------------------------

// This data structure was invented by Youngman Ro. (youngman.ro@gmail.com, 2017/3)

template <typename T, typename BinOp = function<T(T, T)>>
struct GeneralizedBIT {
    int N;                  // 
    int M;                  // power of 2
    vector<T> tree;         // forward BIT
    vector<T> treeR;        // backward BIT

    T       defaultValue;
    BinOp   mergeOp;

    explicit GeneralizedBIT(BinOp op, T dflt = T())
        : N(0), mergeOp(op), defaultValue(dflt) {
    }

    GeneralizedBIT(int n, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        init(n);
    }

    GeneralizedBIT(const T value, int n, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(value, n);
    }

    GeneralizedBIT(const T arr[], int n, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(arr, n);
    }

    GeneralizedBIT(const vector<T>& v, BinOp op, T dflt = T())
        : mergeOp(op), defaultValue(dflt) {
        build(v);
    }


    void init(int n) {
        N = n;
        M = 1;
        while (M < n)
            M <<= 1;

        tree = vector<T>(M + 1, defaultValue);
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


    void clear() {
        fill(tree.begin(), tree.end(), defaultValue);
        fill(treeR.begin(), treeR.end(), defaultValue);
    }


    void add(int pos, T val) {
        update(pos, query(pos) + val);
    }

    void update(int pos, T val) {
        int i, j;

        i = j = pos | 1;
        if (pos & 1)
            treeR[i] = val;
        else
            tree[i] = val;

        int mask = 2;
        while (i < N && j > 0) {
            int half = mask >> 1;
            if ((i & mask) == 0) {
                i += i & -i;
                tree[i] = mergeOp(tree[i - half], treeR[i - half]);
            } else {
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
    T query(int left, int right) const {
        T res = defaultValue;
        if (left == 0) {
            for (int R = right + 1; 0 < R; R &= R - 1)
                res = mergeOp(res, tree[R]);
        } else {
            int R = right + 1;
            for (int next = R & (R - 1); left <= next; R = next, next = R & (R - 1))
                res = mergeOp(res, tree[R]);

            for (int L = left; L < R; L += L & -L)
                res = mergeOp(res, treeR[L]);
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
GeneralizedBIT<T, BinOp> makeGeneralizedBIT(int size, BinOp op, T dfltValue = T()) {
    return GeneralizedBIT<T, BinOp>(size, op, dfltValue);
}

template <typename T, typename BinOp>
GeneralizedBIT<T, BinOp> makeGeneralizedBIT(const T arr[], int size, BinOp op, T dfltValue = T()) {
    return GeneralizedBIT<T, BinOp>(arr, size, op, dfltValue);
}

template <typename T, typename BinOp>
GeneralizedBIT<T, BinOp> makeGeneralizedBIT(const vector<T>& v, BinOp op, T dfltValue = T()) {
    return GeneralizedBIT<T, BinOp>(v, op, dfltValue);
}
