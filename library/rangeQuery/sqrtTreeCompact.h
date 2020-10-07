#pragma once

// https://e-maxx-eng.appspot.com/data_structures/sqrt-tree.html

template <typename T, typename MergeOp = function<T(T, T)>>
struct CompactSqrtTree {
    int N;
    int H;
    vector<T> value;                // 

    vector<int> layers;             // layer -> log
    vector<int> onLayer;            // log -> layer

    vector<vector<T>> prefix;       // right side, A[L..i]
    vector<vector<T>> suffix;       // left side, A[i..R]

    MergeOp mergeOp;
    T       defaultValue;

    explicit CompactSqrtTree(MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
    }

    CompactSqrtTree(int n, const T& val, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(n, val);
    }

    CompactSqrtTree(const T a[], int n, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a, n);
    }

    CompactSqrtTree(const vector<T>& a, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a);
    }


    // O(N*loglogN)
    void build(int n, const T& val) {
        N = n;
        value.assign(n, val);
        buildTree(n);
    }

    // O(N*loglogN)
    void build(const T arr[], int n) {
        N = n;
        value.assign(arr, arr + n);
        buildTree(n);
    }

    void build(const vector<T>& v) {
        build(&v[0], int(v.size()));
    }

    //--- update

    // O(sqrt(N)), inclusive
    void update(int index, const T& val) {
        value[index] = val;
        updateSub(0, 0, N - 1, index);
    }

    // O((K + sqrt(N))*loglogN), inclusive
    void update(int left, int right, const T& val) {
        for (int i = left; i <= right; i++)
            value[i] = val;
        updateSub(0, 0, N - 1, left, right);
    }

    //--- query

    // O(K/sqrt(N)) ~ O(sqrt(N)), inclusive
    T query(int left, int right) const {
        if (left == right)
            return value[left];

        if (left + 1 == right)
            return mergeOp(value[left], value[right]);

        int layer = onLayer[sizeof(int) * 8 - clz(left ^ right)];
        int sizeLog = (layers[layer] + 1) >> 1;

        int lBound = (left >> layers[layer]) << layers[layer];
        int lBlock = ((left - lBound) >> sizeLog) + 1;
        int rBlock = ((right - lBound) >> sizeLog) - 1;

        T ans = suffix[layer][left];
        while (lBlock <= rBlock)
            ans = mergeOp(ans, suffix[layer][lBound + (lBlock++ << sizeLog)]);

        return mergeOp(ans, prefix[layer][right]);
    }

private:
    void buildTree(int n) {
        H = 0;
        while ((1 << H) < n)
            H++;

        onLayer.assign(H + 1, 0);

        layers.clear();
        for (int i = H; i > 1; i = (i + 1) >> 1) {
            onLayer[i] = int(layers.size());
            layers.push_back(i);
        }

        for (int i = H - 1; i >= 0; i--) {
            onLayer[i] = max(onLayer[i], onLayer[i + 1]);
        }

        prefix.assign(layers.size(), vector<T>(n));
        suffix.assign(layers.size(), vector<T>(n));

        buildSub(0, 0, n - 1);
    }

    void buildPrefixSuffix(int layer, int left, int right) {
        prefix[layer][left] = value[left];
        for (int i = left + 1; i <= right; i++) {
            prefix[layer][i] = mergeOp(prefix[layer][i - 1], value[i]);
        }

        suffix[layer][right] = value[right];
        for (int i = right - 1; i >= left; i--) {
            suffix[layer][i] = mergeOp(value[i], suffix[layer][i + 1]);
        }
    }


    void buildSub(int layer, int left, int right) {
        if (layer >= int(layers.size()))
            return;

        int sizeLog = (layers[layer] + 1) >> 1;
        int size = 1 << sizeLog;

        for (int L = left; L <= right; L += size) {
            int R = min(L + size - 1, right);
            buildPrefixSuffix(layer, L, R);
            buildSub(layer + 1, L, R);
        }
    }

    void updateSub(int layer, int left, int right, int index) {
        if (layer >= int(layers.size()))
            return;

        int sizeLog = (layers[layer] + 1) >> 1;
        int size = 1 << sizeLog;

        int blockIndex = (index - left) / size;

        int L = left + blockIndex * size;
        int R = min(L + size - 1, right);
        buildPrefixSuffix(layer, L, R);
        updateSub(layer + 1, L, R, index);
    }

    void updateSub(int layer, int left, int right, int indexLeft, int indexRight) {
        if (layer >= int(layers.size()))
            return;

        int sizeLog = (layers[layer] + 1) >> 1;
        int size = 1 << sizeLog;

        int blockIndexL = (max(left, indexLeft) - left) / size;
        int blockIndexR = (min(right, indexRight) - left) / size;
        for (int L = left + blockIndexL * size, maxL = left + blockIndexR * size; L <= maxL; L += size) {
            int R = min(L + size - 1, right);
            buildPrefixSuffix(layer, L, R);
            updateSub(layer + 1, L, R, indexLeft, indexRight);
        }
    }

    static int clz(int x) {
        if (!x)
            return 32;
#ifndef __GNUC__
        return int(__lzcnt(unsigned(x)));
#else
        return __builtin_clz(unsigned(x));
#endif
    }
};

template <typename T, typename MergeOp>
CompactSqrtTree<T, MergeOp> makeCompactSqrtTree(int n, const T& val, MergeOp op, T dfltValue = T()) {
    return CompactSqrtTree<T, MergeOp>(n, val, op, dfltValue);
}

template <typename T, typename MergeOp>
CompactSqrtTree<T, MergeOp> makeCompactSqrtTree(const vector<T>& arr, MergeOp op, T dfltValue = T()) {
    return CompactSqrtTree<T, MergeOp>(arr, op, dfltValue);
}

template <typename T, typename MergeOp>
CompactSqrtTree<T, MergeOp> makeCompactSqrtTree(const T arr[], int size, MergeOp op, T dfltValue = T()) {
    return CompactSqrtTree<T, MergeOp>(arr, size, op, dfltValue);
}
