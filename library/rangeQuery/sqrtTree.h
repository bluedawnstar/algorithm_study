#pragma once

// https://e-maxx-eng.appspot.com/data_structures/sqrt-tree.html

template <typename T, typename BinOp = function<T(T, T)>>
struct SqrtTree {
    int N;
    int H;
    vector<T> value;                // 

    vector<int> layers;             // layer -> log
    vector<int> onLayer;            // log -> layer

    vector<vector<T>> prefix;       // startright side, A[L..i]
    vector<vector<T>> suffix;       // left side, A[i..R]
    vector<vector<T>> between;      // left and right side, A[i..j] (L <= i <= j <= R)

    T       defaultValue;
    BinOp   mergeOp;

    explicit SqrtTree(BinOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
    }

    SqrtTree(const T a[], int n, BinOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a, n);
    }

    SqrtTree(const vector<T>& a, BinOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a);
    }

    // O(N*loglogN)
    void build(const T arr[], int n) {
        N = n;
        H = 0;
        while ((1 << H) < n)
            H++;

        value.assign(arr, arr + n);
        onLayer.assign(H + 1, 0);

        layers.clear();
        for (int i = H; i > 1; i = (i + 1) >> 1) {
            onLayer[i] = (int)layers.size();
            layers.push_back(i);
        }

        for (int i = H - 1; i >= 0; i--) {
            onLayer[i] = max(onLayer[i], onLayer[i + 1]);
        }

        prefix.assign(layers.size(), vector<T>(N));
        suffix.assign(layers.size(), vector<T>(N));
        between.assign(layers.size(), vector<T>(1 << H));

        buildSub(0, 0, N - 1);
    }

    void build(const vector<T>& v) {
        build(&v[0], (int)v.size());
    }

    //--- query

    // O(1), inclusive
    T query(int left, int right) const {
        if (left == right)
            return value[left];

        if (left + 1 == right)
            return mergeOp(value[left], value[right]);

        int layer = onLayer[sizeof(int) * 8 - clz(left ^ right)];
        int sizeLog = (layers[layer] + 1) >> 1;
        int countLog = layers[layer] >> 1;

        int lBound = (left >> layers[layer]) << layers[layer];
        int lBlock = ((left - lBound) >> sizeLog) + 1;
        int rBlock = ((right - lBound) >> sizeLog) - 1;

        T ans = suffix[layer][left];
        if (lBlock <= rBlock)
            ans = mergeOp(ans, between[layer][lBound + (lBlock << countLog) + rBlock]);

        return mergeOp(ans, prefix[layer][right]);
    }

private:
    void buildSub(int layer, int left, int right) {
        if (layer >= (int)layers.size())
            return;

        int sizeLog = (layers[layer] + 1) >> 1;
        int countLog = layers[layer] >> 1;
        int size = 1 << sizeLog;
        int count = 0;

        for (int L = left; L <= right; L += size) {
            count++;
            int R = min(L + size - 1, right);

            prefix[layer][L] = value[L];
            for (int i = L + 1; i <= R; i++) {
                prefix[layer][i] = mergeOp(prefix[layer][i - 1], value[i]);
            }
            
            suffix[layer][R] = value[R];
            for (int i = R - 1; i >= L; i--) {
                suffix[layer][i] = mergeOp(value[i], suffix[layer][i + 1]);
            }
            
            buildSub(layer + 1, L, R);
        }

        for (int i = 0; i < count; i++) {
            T ans = defaultValue;
            for (int j = i; j < count; j++) {
                T add = suffix[layer][left + (j << sizeLog)];
                ans = (i == j) ? add : mergeOp(ans, add);
                between[layer][left + (i << countLog) + j] = ans;
            }
        }
    }

    static int clz(int x) {
#ifndef __GNUC__
        return (int)__lzcnt((unsigned)x);
#else
        return __builtin_clz((unsigned)x);
#endif
    }
};

template <typename T, typename BinOp>
SqrtTree<T, BinOp> makeSqrtTree(const vector<T>& arr, BinOp op, T dfltValue = T()) {
    return SqrtTree<T, BinOp>(arr, op, dfltValue);
}

template <typename T, typename BinOp>
SqrtTree<T, BinOp> makeSqrtTree(const T arr[], int size, BinOp op, T dfltValue = T()) {
    return SqrtTree<T, BinOp>(arr, size, op, dfltValue);
}
