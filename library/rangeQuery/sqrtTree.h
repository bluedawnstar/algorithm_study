#pragma once

#include <vector>
#include <functional>

// ref: https://cp-algorithms.com/data_structures/sqrt-tree.html

template <typename T, typename MergeOp = function<T(T, T)>>
struct SqrtTree {
    int                 N;
    int                 blockSize;
    int                 blockCount;

    vector<T>           prefix;             // prefix array
    vector<T>           suffix;             // suffix array

    // disjoint sparse tables
    vector<int>         H;
    vector<vector<T>>   blockSpTable;       // disjoint sparse table of block values
    vector<vector<T>>   spTablesOfBlocks;   // disjoint sparse tables of each blocks

    MergeOp             mergeOp;
    T                   defaultValue;

    explicit SqrtTree(MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
    }

    SqrtTree(const T a[], int n, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a, n);
    }

    SqrtTree(const vector<T>& a, MergeOp op, T dfltValue = T())
        : mergeOp(op), defaultValue(dfltValue) {
        build(a);
    }


    // O(N*logN)
    void build(const T a[], int n) {
        N = n;

        int sqrtN = int(sqrt(n));
        blockSize = 1;
        while (blockSize < sqrtN)
            blockSize <<= 1;
        blockCount = (N + blockSize - 1) / blockSize;

        buildPrefixSuffix(a, n);
        buildSparseTable(a, n);
    }

    void build(const vector<T>& a) {
        build(&a[0], int(a.size()));
    }


    // O(1), inclusive
    T query(int left, int right) const {
        if (left == right)
            return spTablesOfBlocks[0][left];

        int blockL = left / blockSize;
        int blockR = right / blockSize;

        if (blockL == blockR) {
            int h = H[left ^ right];
            return mergeOp(spTablesOfBlocks[h][left], spTablesOfBlocks[h][right]);
        }

        T res = mergeOp(suffix[left], prefix[right]);
        if (blockL + 1 < blockR) {
            if (++blockL == --blockR)
                res = mergeOp(res, blockSpTable[0][blockL]);
            else {
                int h = H[blockL ^ blockR];
                res = mergeOp(res, mergeOp(blockSpTable[h][blockL], blockSpTable[h][blockR]));
            }
        }
        return res;
    }

    // O(sqrt(N))
    void update(int index, T x) {
        spTablesOfBlocks[0][index] = x;
        updatePrefixSuffix(index);
        updateSparseTable(index);
    }

private:
    // O(N)
    void buildPrefixSuffix(const T a[], int n) {
        prefix.resize(N);
        suffix.resize(N);
        for (int i = 0; i < blockCount; i++) {
            int first = i * blockSize;
            int last = min(first + blockSize, N) - 1;

            prefix[first] = a[first];
            for (int j = first + 1; j <= last; j++)
                prefix[j] = mergeOp(prefix[j - 1], a[j]);

            suffix[last] = a[last];
            for (int j = last - 1; j >= first; j--)
                suffix[j] = mergeOp(suffix[j + 1], a[j]);
        }
    }

    void buildSparseTable(vector<vector<T>>& table) {
        int column = int(table[0].size());
        for (int h = 1, range = 4; h < int(table.size()); h++, range <<= 1) {
            int half = range >> 1;
            for (int i = half; i < column; i += range) {
                table[h][i - 1] = table[0][i - 1];
                for (int j = i - 2; j >= i - half; j--)
                    table[h][j] = mergeOp(table[h][j + 1], table[0][j]);

                table[h][i] = table[0][i];
                for (int j = i + 1; j < i + half; j++)
                    table[h][j] = mergeOp(table[h][j - 1], table[0][j]);
            }
        }
    }

    // O(N*logN)
    void buildSparseTable(const T a[], int n) {
        int blockN = 1;
        while (blockN < blockCount)
            blockN <<= 1;

        H.resize(max(blockN, blockSize));
        H[1] = 0;
        for (int i = 2; i < int(H.size()); i++)
            H[i] = H[i >> 1] + 1;

        //-- block sparse table - O(sqrt(N)*logN)
        blockSpTable.assign(H[blockN - 1] + 1, vector<T>(blockN, defaultValue));
        for (int i = 0; i < blockCount; i++)
            blockSpTable[0][i] = suffix[i * blockSize];

        buildSparseTable(blockSpTable);

        //-- sparse tables of blocks - O(N*logN)
        spTablesOfBlocks.assign(H[blockSize - 1] + 1, vector<T>(blockSize * blockCount, defaultValue));
        for (int i = 0; i < n; i++)
            spTablesOfBlocks[0][i] = a[i];

        buildSparseTable(spTablesOfBlocks);
    }


    // O(sqrt(N))
    void updatePrefixSuffix(int index) {
        int blockIndex = index / blockSize;

        int first = blockIndex * blockSize;
        int last = min(first + blockSize, N) - 1;

        prefix[index] = (index > first) ? mergeOp(prefix[index - 1], spTablesOfBlocks[0][index]) : spTablesOfBlocks[0][index];
        for (int j = index + 1; j <= last; j++)
            prefix[j] = mergeOp(prefix[j - 1], spTablesOfBlocks[0][j]);

        suffix[index] = (index < last) ? mergeOp(suffix[index + 1], spTablesOfBlocks[0][index]) : spTablesOfBlocks[0][index];
        for (int j = index - 1; j >= first; j--)
            suffix[j] = mergeOp(suffix[j + 1], spTablesOfBlocks[0][j]);
    }

    void updateSparseTable(vector<vector<T>>& table, int index) {
        for (int h = 1, range = 4; h < int(table.size()); h++, range <<= 1) {
            int half = range >> 1;
            int start = index & ~(range - 1);
            if ((index & half) == 0) { // suffix
                table[h][index] = (index + 1 < (start | half)) ? mergeOp(table[h][index + 1], table[0][index])
                                                               : table[0][index];
                for (int i = index - 1; i >= start; i--)
                    table[h][i] = mergeOp(table[h][i + 1], table[0][i]);
            } else { // prefix
                table[h][index] = (index > (start | half)) ? mergeOp(table[h][index - 1], table[0][index])
                                                           : table[0][index];
                for (int i = index + 1, next = start + range; i < next; i++)
                    table[h][i] = mergeOp(table[h][i - 1], table[0][i]);
            }
        }
    }

    // O(sqrt(N))
    void updateSparseTable(int index) {
        int blockIndex = index / blockSize;

        //-- block sparse table - O(sqrt(N))
        blockSpTable[0][blockIndex] = suffix[blockIndex * blockSize];
        updateSparseTable(blockSpTable, blockIndex);

        //-- sparse tables of blocks - O(sqrt(N))
        updateSparseTable(spTablesOfBlocks, index);
    }
};

template <typename T, typename MergeOp>
inline SqrtTree<T, MergeOp> makeSqrtTree(MergeOp op, T dfltValue = T()) {
    return SqrtTree<T, MergeOp>(op, dfltValue);
}

template <typename T, typename MergeOp>
inline SqrtTree<T, MergeOp> makeSqrtTree(const T arr[], int size, MergeOp op, T dfltValue = T()) {
    return SqrtTree<T, MergeOp>(arr, size, op, dfltValue);
}

template <typename T, typename MergeOp>
inline SqrtTree<T, MergeOp> makeSqrtTree(const vector<T>& arr, MergeOp op, T dfltValue = T()) {
    return SqrtTree<T, MergeOp>(arr, op, dfltValue);
}

/* example
    1) Min SqrtTree (RMQ)
        auto sqrtTree = makeSqrtTree<int>(v, [](int a, int b) { return min(a, b); }, INT_MAX);
        ...
        sqrtTree.query(left, right);

    2) Max SqrtTree
        auto sqrtTree = makeSqrtTree<int>(v, [](int a, int b) { return max(a, b); });
        ...
        sqrtTree.query(left, right);

    3) GCD SqrtTree
        auto sqrtTree = makeSqrtTree<int>(v, [](int a, int b) { return gcd(a, b); });
        ...
        sqrtTree.query(left, right);

    4) Sum SqrtTree
        auto sqrtTree = makeSqrtTree<int>(v, [](int a, int b) { return a + b; });
        ...
        sqrtTree.query(left, right);
*/
