#pragma once

template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct SqrtDecompositionLazy {
    int N;
    int blockSize;
    int blockCount;

    vector<T> values;
    vector<T> blockValues;
    vector<T> blockLazy;
    vector<bool> blockLazyExist;

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
        blockSize = (int)sqrt(n);
        blockCount = (N + blockSize - 1) / blockSize;

        values.resize(n);
        blockValues.resize(blockCount);
        blockLazy.resize(blockCount);
        blockLazyExist.resize(blockCount);
    }

    void build(T value, int n) {
        init(n);
        fill(values.begin(), values.begin() + n, value);
        for (int i = 0; i < blockCount; i++)
            updateBlockValue(i);
    }

    void build(const T v[], int n) {
        init(n);
        copy(v, v + n, values.begin());
        for (int i = 0; i < blockCount; i++)
            updateBlockValue(i);
    }

    void build(const vector<T>& v) {
        build(&v[0], (int)v.size());
    }


    void update(int l, int r, T x) {
        int blockL = l / blockSize;
        int blockR = r / blockSize;
        int blockCL = (l + blockSize - 1) / blockSize;
        int blockCU = (r + 1) / blockSize;

        if (blockL == blockR) {
            if (getBlockSize(blockL) == (r - l + 1)) {
                blockValues[blockL] = blockOp(x, r - l + 1);

                blockLazy[blockL] = x;
                blockLazyExist[blockL] = true;
            } else {
                if (blockLazyExist[blockL]) {
                    updateValue(blockL * blockSize, l, blockLazy[blockL]);
                    updateValue(r + 1, min(N, (blockL + 1) * blockSize), blockLazy[blockL]);
                    blockLazyExist[blockL] = false;
                }
                updateValue(l, r + 1, x);
                updateBlockValue(blockL);
            }
        } else {
            if (blockL != blockCL) {
                if (blockLazyExist[blockL]) {
                    updateValue(blockL * blockSize, l, blockLazy[blockL]);
                    blockLazyExist[blockL] = false;
                }
                updateValue(l, blockCL * blockSize, x);
                updateBlockValue(blockL);
            }

            T blockX = blockOp(x, blockSize);

            while (blockCL < blockCU) {
                blockValues[blockCL] = blockX;

                blockLazy[blockCL] = x;
                blockLazyExist[blockCL] = true;
                blockCL++;
            }

            if (blockCU == blockR) {
                if (blockLazyExist[blockR]) {
                    updateValue(r + 1, min(N, (blockR + 1) * blockSize), blockLazy[blockR]);
                    blockLazyExist[blockR] = false;
                }
                updateValue(blockR * blockSize, r + 1, x);
                updateBlockValue(blockR);
            }
        }
    }

    T query(int l, int r) {
        T res = defaultValue;

        int blockL = l / blockSize;
        int blockR = r / blockSize;
        int blockCL = (l + blockSize - 1) / blockSize;
        int blockCU = (r + 1) / blockSize;

        if (blockL == blockR) {
            if (blockLazyExist[blockL])
                res = blockOp(blockLazy[blockL], r - l + 1);
            else
                res = mergeValue(l, r + 1);
        } else {
            if (blockL != blockCL) {
                if (blockLazyExist[blockL])
                    res = blockOp(blockLazy[blockL], blockCL * blockSize - l);
                else
                    res = mergeValue(l, blockCL * blockSize);
            }

            while (blockCL < blockCU) {
                res = mergeOp(res, blockValues[blockCL]);
                blockCL++;
            }

            if (blockCU == blockR) {
                if (blockLazyExist[blockR])
                    res = mergeOp(res, blockOp(blockLazy[blockR], r % blockSize + 1));
                else
                    res = mergeOp(res, mergeValue(blockR * blockSize, r + 1));
            }
        }

        return res;
    }

private:
    void updateBlockValue(int block) {
        blockValues[block] = defaultValue;

        int l = block * blockSize;
        int r = min(N, l + blockSize);
        while (l < r)
            blockValues[block] = mergeOp(blockValues[block], values[l++]);
    }

    T mergeValue(int l, int r) {
        T res = defaultValue;

        while (l < r)
            res = mergeOp(res, values[l++]);

        return res;
    }

    void updateValue(int l, int r, T x) {
        while (l < r)
            values[l++] = x;
    }

    int getBlockSize(int block) {
        return ((N / blockSize) == block) ? (N % blockSize) : blockSize;
    }
};
