#pragma once

// https://e-maxx-eng.appspot.com/data_structures/sqrt-tree.html

#include "sqrtTree.h"

// SqrtTree with SqrtTree for Fast Update
template <typename T, typename MergeOp = function<T(T, T)>>
struct FastSqrtTree {
    int N;
    int H;
    vector<T> value;                // 

    vector<int> layers;             // layer -> log
    vector<int> onLayer;            // log -> layer

    vector<vector<T>> prefix;       // right side, A[L..i]
    vector<vector<T>> suffix;       // left side, A[i..R]
    vector<vector<T>> between;      // left and right side, A[i..j] (L <= i <= j <= R)

    int indexSize;
    SqrtTree<T, MergeOp> between0;  // left and right side, A[i..j] (L <= i <= j <= R)

    MergeOp mergeOp;
    T       defaultValue;

    explicit FastSqrtTree(MergeOp op, T dfltValue = T())
        : between0(op, dfltValue), mergeOp(op), defaultValue(dfltValue) {
    }

    FastSqrtTree(int n, const T& val, MergeOp op, T dfltValue = T())
        : between0(op, dfltValue), mergeOp(op), defaultValue(dfltValue) {
        build(n, val);
    }

    FastSqrtTree(const T a[], int n, MergeOp op, T dfltValue = T())
        : between0(op, dfltValue), mergeOp(op), defaultValue(dfltValue) {
        build(a, n);
    }

    FastSqrtTree(const vector<T>& a, MergeOp op, T dfltValue = T())
        : between0(op, dfltValue), mergeOp(op), defaultValue(dfltValue) {
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
        if (lBlock <= rBlock) {
            if (layer == 0)
                ans = mergeOp(ans, between0.query(lBlock, rBlock));
            else
                ans = mergeOp(ans, between[layer - 1][lBound + (lBlock << countLog) + rBlock]);
        }

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
        between.assign(max(0, (int)layers.size() - 1), vector<T>(size_t(1) << H));

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

    void buildBetween(int layer, int left, int right, int sizeLog, int countLog) {
        int count = (right - left) / (1 << sizeLog) + 1;

        for (int i = 0; i < count; i++) {
            T ans = defaultValue;
            for (int j = i; j < count; j++) {
                ans = mergeOp(ans, suffix[layer][left + (j << sizeLog)]);
                between[layer - 1][left + (i << countLog) + j] = ans;
            }
        }
    }

    void updateBetween(int layer, int left, int right, int blockIndex, int sizeLog, int countLog) {
        int rightIndex = (right - left) / (1 << sizeLog);

        for (int i = 0; i <= blockIndex; i++) {
            T ans = defaultValue;
            if (i < blockIndex)
                ans = between[layer - 1][left + (i << countLog) + blockIndex - 1];
            for (int j = blockIndex; j <= rightIndex; j++) {
                ans = mergeOp(ans, suffix[layer][left + (j << sizeLog)]);
                between[layer - 1][left + (i << countLog) + j] = ans;
            }
        }
    }

    void updateBetween(int layer, int left, int right, int blockIndexL, int blockIndexR, int sizeLog, int countLog) {
        int rightIndex = (right - left) / (1 << sizeLog);

        for (int i = 0; i <= blockIndexR; i++) {
            T ans = defaultValue;
            if (i < blockIndexL)
                ans = between[layer - 1][left + (i << countLog) + blockIndexL - 1];
            for (int j = max(i, blockIndexL); j <= rightIndex; j++) {
                ans = mergeOp(ans, suffix[layer][left + (j << sizeLog)]);
                between[layer - 1][left + (i << countLog) + j] = ans;
            }
        }
    }


    void buildSub(int layer, int left, int right) {
        if (layer >= int(layers.size()))
            return;

        int sizeLog = (layers[layer] + 1) >> 1;
        int countLog = layers[layer] >> 1;
        int size = 1 << sizeLog;

        for (int L = left; L <= right; L += size) {
            int R = min(L + size - 1, right);
            buildPrefixSuffix(layer, L, R);
            buildSub(layer + 1, L, R);
        }

        if (layer == 0) {
            indexSize = (N + size - 1) >> sizeLog;
            vector<T> v(indexSize);
            for (int i = 0; i < indexSize; i++)
                v[i] = suffix[0][i << sizeLog];
            between0.build(v);
        } else
            buildBetween(layer, left, right, sizeLog, countLog);
    }

    void updateSub(int layer, int left, int right, int index) {
        if (layer >= int(layers.size()))
            return;

        int sizeLog = (layers[layer] + 1) >> 1;
        int countLog = layers[layer] >> 1;
        int size = 1 << sizeLog;

        int blockIndex = (index - left) / size;

        int L = left + blockIndex * size;
        int R = min(L + size - 1, right);
        buildPrefixSuffix(layer, L, R);
        if (layer == 0)
            between0.update(blockIndex, suffix[0][blockIndex << sizeLog]);
        else
            updateBetween(layer, left, right, blockIndex, sizeLog, countLog);
        updateSub(layer + 1, L, R, index);
    }

    void updateSub(int layer, int left, int right, int indexLeft, int indexRight) {
        if (layer >= int(layers.size()))
            return;

        int sizeLog = (layers[layer] + 1) >> 1;
        int countLog = layers[layer] >> 1;
        int size = 1 << sizeLog;

        int blockIndexL = (max(left, indexLeft) - left) / size;
        int blockIndexR = (min(right, indexRight) - left) / size;
        for (int L = left + blockIndexL * size, maxL = left + blockIndexR * size; L <= maxL; L += size) {
            int R = min(L + size - 1, right);
            buildPrefixSuffix(layer, L, R);
            updateSub(layer + 1, L, R, indexLeft, indexRight);
        }

        if (layer == 0) {
            for (int i = blockIndexL; i <= blockIndexR; i++)
                between0.value[i] = suffix[0][i << sizeLog];
            between0.rebuild(blockIndexL, blockIndexR);
        } else
            updateBetween(layer, left, right, blockIndexL, blockIndexR, sizeLog, countLog);
    }

    static int clz(int x) {
#ifndef __GNUC__
        return int(__lzcnt(unsigned(x)));
#else
        return __builtin_clz(unsigned(x));
#endif
    }
};

template <typename T, typename MergeOp>
FastSqrtTree<T, MergeOp> makeFastSqrtTree(int n, const T& val, MergeOp op, T dfltValue = T()) {
    return FastSqrtTree<T, MergeOp>(n, val, op, dfltValue);
}

template <typename T, typename MergeOp>
FastSqrtTree<T, MergeOp> makeFastSqrtTree(const vector<T>& arr, MergeOp op, T dfltValue = T()) {
    return FastSqrtTree<T, MergeOp>(arr, op, dfltValue);
}

template <typename T, typename MergeOp>
FastSqrtTree<T, MergeOp> makeFastSqrtTree(const T arr[], int size, MergeOp op, T dfltValue = T()) {
    return FastSqrtTree<T, MergeOp>(arr, size, op, dfltValue);
}
