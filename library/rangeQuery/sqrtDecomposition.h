#pragma once

template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct SqrtDecompositionLazy {
    int N;
    int blockSize;
    int blockCount;

    vector<T> values;
    vector<T> blockValues;

    enum LazyT {
        lzNone,
        lzSet,
        lzAdd
    };
    vector<LazyT> blockLazy;
    vector<T> blockLazyValues;

    T         defaultValue;
    MergeOp   mergeOp;
    BlockOp   blockOp;

    SqrtDecompositionLazy(MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        N = 0;
        blockSize = 0;
        blockCount = 0;
    }

    explicit SqrtDecompositionLazy(int n, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        init(n);
    }

    SqrtDecompositionLazy(T value, int n, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(value, n);
    }

    SqrtDecompositionLazy(const T v[], int n, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(v, n);
    }

    explicit SqrtDecompositionLazy(const vector<T>& v, MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        build(v);
    }


    void init(int n) {
        N = n;
        blockSize = int(sqrt(n));
        blockCount = (N + blockSize - 1) / blockSize;

        values.assign(blockCount * blockSize, defaultValue);
        blockValues.assign(blockCount, defaultValue);
        blockLazyValues.assign(blockCount, defaultValue);
        blockLazy.assign(blockCount, lzNone);
    }

    // O(n)
    void build(T value, int n) {
        init(n);
        fill(values.begin(), values.begin() + n, value);
        for (int i = 0; i < blockCount; i++)
            recalcBlockValue(i);
    }

    // O(n)
    void build(const T v[], int n) {
        init(n);
        copy(v, v + n, values.begin());
        for (int i = 0; i < blockCount; i++)
            recalcBlockValue(i);
    }

    void build(const vector<T>& v) {
        build(&v[0], int(v.size()));
    }


    // O(sqrt(n))
    void update(int l, int r, T x) {
        int blockL = l / blockSize;
        int blockR = r / blockSize;

        if (blockL == blockR)
            updateInBlock(l, r, x);
        else {
            updateInBlock(l, (blockL + 1) * blockSize - 1, x);
            for (int i = blockL + 1; i < blockR; i++)
                updateBlock(i, x);
            updateInBlock(blockR * blockSize, r, x);
        }
    }

    void add(int l, int r, T x) {
        int blockL = l / blockSize;
        int blockR = r / blockSize;

        if (blockL == blockR)
            addInBlock(l, r, x);
        else {
            addInBlock(l, (blockL + 1) * blockSize - 1, x);
            for (int i = blockL + 1; i < blockR; i++)
                addBlock(i, x);
            addInBlock(blockR * blockSize, r, x);
        }
    }

    // O(sqrt(n))
    T query(int l, int r) {
        int blockL = l / blockSize;
        int blockR = r / blockSize;

        T res = 0;

        if (blockL == blockR)
            res += queryInBlock(l, r);
        else {
            res += queryInBlock(l, (blockL + 1) * blockSize - 1);
            for (int i = blockL + 1; i < blockR; i++)
                res += blockValues[i];
            res += queryInBlock(blockR * blockSize, r);
        }

        return res;
    }

private:
    void applyBlock(int block) {
        if (blockLazy[block] == lzNone)
            return;

        int l = block * blockSize;
        int r = l + blockSize;
        T val = blockLazyValues[block];

        if (blockLazy[block] == lzSet) {
            while (l < r)
                values[l++] = val;
        } else {
            while (l < r)
                values[l++] += val;
        }

        blockLazy[block] = lzNone;
        blockLazyValues[block] = 0;
    }

    void recalcBlockValue(int block) {
        int l = block * blockSize;
        int r = l + blockSize;

        T val = defaultValue;
        while (l < r)
            val = mergeOp(val, values[l++]);

        blockValues[block] = val;
    }

    void updateBlock(int block, T val) {
        blockValues[block] = blockOp(val, blockSize);
        blockLazy[block] = lzSet;
        blockLazyValues[block] = val;
    }

    void addBlock(int block, T val) {
        blockValues[block] += val * blockSize;      //TODO: fix it
        if (blockLazy[block] != lzSet)
            blockLazy[block] = lzAdd;
        blockLazyValues[block] += val;
    }


    void updateInBlock(int l, int r, T val) {
        int block = l / blockSize;

        if (r - l + 1 == blockSize) {
            updateBlock(block, val);
            return;
        }

        applyBlock(block);
        while (l <= r)
            values[l++] = val;

        recalcBlockValue(block);
    }

    void addInBlock(int l, int r, T val) {
        int block = l / blockSize;

        if (r - l + 1 == blockSize) {
            addBlock(block, val);
            return;
        }

        applyBlock(block);
        while (l <= r)
            values[l++] += val;

        recalcBlockValue(block);
    }

    T queryInBlock(int l, int r) {
        int block = l / blockSize;

        if (r - l + 1 == blockSize)
            return blockValues[block];

        if (blockLazy[block] == lzSet)
            return blockOp(blockLazyValues[block], r - l + 1);

        applyBlock(block);

        T res = defaultValue;
        while (l <= r)
            res = mergeOp(res, values[l++]);

        return res;
    }
};
