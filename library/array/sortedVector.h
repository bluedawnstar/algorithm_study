#pragma once

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

template <typename T>
struct SortedVector {
    struct Block {
        int idx;
        int cnt;
        vector<T> v;

        void init(int K) {
            v.resize(K);
        }

        void build(int i, int n, const T* data) {
            idx = i;
            cnt = n;

            memcpy(&v[0], data, sizeof(T) * n);
            sort(v.begin(), v.begin() + n);
        }

        int countLessOrEqual(int val) const {
            return int(upper_bound(v.begin(), v.begin() + cnt, val) - v.begin());
        }

        int countLess(int val) const {
            return int(lower_bound(v.begin(), v.begin() + cnt, val) - v.begin());
        }

        int countGreaterOrEqual(int val) const {
            return cnt - countLess(val);
        }

        int countGreater(int val) const {
            return cnt - countLessOrEqual(val);
        }

        int count(int val) const {
            return int(upper_bound(v.begin(), v.begin() + cnt, val) - lower_bound(v.begin(), v.begin() + cnt, val));
        }
    };

    int valueN;
    vector<T> values;

    int blockN;
    vector<Block> blocks;

    int maxN;       // max count
    int K;          // max block size
    int M;          // max block count

    explicit SortedVector(int maxN, int k = 0, int m = 0) : maxN(maxN), K(k), M(m), values(maxN) {
        if (K <= 0)
            K = int(ceil(sqrt(maxN)));
        if (M <= 0)
            M = ((maxN + K - 1) / K) * 4;

        blocks.resize(M);
        for (int i = 0; i < M; i++)
            blocks[i].init(K);

        valueN = 0;
        blockN = 0;
    }

    //--- build

    void build(const T* v, int n) {
        valueN = n;
        memcpy(&values[0], v, sizeof(T) * n);

        blockN = 0;
        for (int i = 0; i < n; i += K)
            blocks[blockN++].build(i, min(K, n - i), v + i);
    }

    void build(const vector<T>& v) {
        build(&v[0], (int)v.size());
    }

    // call rebuild() after K(=sqrt(N)) queries
    void rebuild() {
        vector<int> v(values.size());

        valueN = 0;
        for (int i = 0; i < blockN; i++) {
            memcpy(&v[valueN], &values[blocks[i].idx], blocks[i].cnt * sizeof(T));
            valueN += blocks[i].cnt;
        }

        swap(values, v);

        blockN = 0;
        for (int i = 0; i < valueN; i += K)
            blocks[blockN++].build(i, min(K, valueN - i), &values[i]);
    }

    //--- update

    void insert(int pos, T x) {
        check();

        int blk = split(pos);
        shiftR(blk);
        values[valueN] = x;
        blocks[blk].build(valueN, 1, &values[valueN]);
        valueN++;
    }

    void erase(int pos) {
        check();

        split(pos + 1);
        shiftL(split(pos));
    }

    //--- query

    int countLessOrEqual(int L, int R, T x) {
        check();

        int res = 0;
        for (L = split(L), R = split(R + 1); L < R; L++)
            res += blocks[L].countLessOrEqual(x);
        return res;
    }

    int countLess(int L, int R, T x) {
        check();

        int res = 0;
        for (L = split(L), R = split(R + 1); L < R; L++)
            res += blocks[L].countLess(x);
        return res;
    }

    int countGreaterOrEqual(int L, int R, T x) {
        check();

        int res = 0;
        for (L = split(L), R = split(R + 1); L < R; L++)
            res += blocks[L].countGreaterOrEqual(x);
        return res;
    }

    int countGreater(int L, int R, T x) {
        check();

        int res = 0;
        for (L = split(L), R = split(R + 1); L < R; L++)
            res += blocks[L].countGreater(x);
        return res;
    }

    int count(int L, int R, T x) {
        check();

        int res = 0;
        for (L = split(L), R = split(R + 1); L < R; L++)
            res += blocks[L].count(x);
        return res;
    }

private:
    void check() {
        if (blockN >= (int)blocks.size() - 2)
            rebuild();
    }

    // return the index of next block
    int split(int idx) {
        int blk = 0;
        while (blk < blockN && idx >= blocks[blk].cnt)
            idx -= blocks[blk++].cnt;

        if (idx) {
            int i = blocks[blk].idx;
            int cnt = blocks[blk].cnt;

            shiftR(blk);
            blocks[blk + 1].build(i + idx, cnt - idx, &values[i + idx]);
            blocks[blk].build(i, idx, &values[i]);
            blk++;
        }
        return blk;
    }

    void shiftR(int blk) {
        rotate(blocks.begin() + blk, blocks.begin() + blockN, blocks.begin() + blockN + 1);
        ++blockN;
    }
    void shiftL(int blk) {
        rotate(blocks.begin() + blk, blocks.begin() + blk + 1, blocks.begin() + blockN);
        --blockN;
    }
};
