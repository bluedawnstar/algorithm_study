#pragma once

// with SQRT-decomposition
struct BitSetRangeFlip {
#if 0
    typedef unsigned int ValueT;
    typedef int SignedValueT;

    static const int BIT_SIZE = sizeof(unsigned int) * 8;
    static const unsigned int BIT_ALL = 0xffffffffu;
    static const unsigned int BIT_ONE = 1u;

    static const int INDEX_MASK = 0x1F;
    static const int INDEX_SHIFT = 5;
#else
    typedef unsigned long long ValueT;
    typedef long long SignedValueT;

    static const int BIT_SIZE = sizeof(unsigned long long) * 8;
    static const unsigned long long BIT_ALL = 0xffffffffffffffffull;
    static const unsigned long long BIT_ONE = 1ull;

    static const int INDEX_MASK = 0x3F;
    static const int INDEX_SHIFT = 6;
#endif

    int N;
    vector<ValueT> values;

    int sqrtN;
    vector<bool> blockInv;
    vector<int> blockOne;

    explicit BitSetRangeFlip(int n) {
        init(n);
    }

    template <typename T>
    BitSetRangeFlip(const vector<T>& v, T bitMask) {
        init(int(v.size()));
        build(v, bitMask);
    }

    void init(int n) {
        sqrtN = int(sqrt(n));
        N = ((n + sqrtN - 1) / sqrtN) * sqrtN;

        values = vector<ValueT>((N + BIT_SIZE - 1) / BIT_SIZE);

        blockInv = vector<bool>(N / sqrtN);
        blockOne = vector<int>(N / sqrtN);
    }

    template <typename T>
    void build(const vector<T>& v, T bitMask) {
        for (int i = 0, idx = 0; i < N; i += BIT_SIZE) {
            ValueT x = 0;
            for (int j = 0; j < BIT_SIZE && idx < int(v.size()); j++, idx++) {
                if (v[idx] & bitMask)
                    x |= BIT_ONE << j;
            }
            values[i >> INDEX_SHIFT] = x;
        }

        for (int i = 0; i < int(blockOne.size()); i++)
            blockOne[i] = doCountOne(i * sqrtN, (i + 1) * sqrtN - 1);
    }

    //--- query

    bool test(int pos) const {
        bool res = (values[pos >> INDEX_SHIFT] & (BIT_ONE << (pos & INDEX_MASK))) != 0;
        if (blockInv[pos / sqrtN])
            res = !res;
        return res;
    }

    int countOne(int left, int right) {
        int lIdx = left / sqrtN;
        int rIdx = right / sqrtN;

        int res = 0;
        if (lIdx == rIdx) {
            if (left % sqrtN == 0 && (right + 1) % sqrtN == 0)
                res += blockInv[lIdx] ? (sqrtN - blockOne[lIdx]) : blockOne[lIdx];
            else {
                apply(lIdx);
                res += doCountOne(left, right);
            }
        } else {
            if (left % sqrtN == 0)
                res += blockInv[lIdx] ? (sqrtN - blockOne[lIdx]) : blockOne[lIdx];
            else {
                apply(lIdx);
                res += doCountOne(left, (lIdx + 1) * sqrtN - 1);
            }
            if ((right + 1) % sqrtN == 0)
                res += blockInv[rIdx] ? (sqrtN - blockOne[rIdx]) : blockOne[rIdx];
            else {
                apply(rIdx);
                res += doCountOne(rIdx * sqrtN, right);
            }
            for (int i = lIdx + 1; i < rIdx; i++) {
                res += blockInv[i] ? (sqrtN - blockOne[i]) : blockOne[i];
            }
        }
        return res;
    }

    void flip(int left, int right) {
        int lIdx = left / sqrtN;
        int rIdx = right / sqrtN;

        if (lIdx == rIdx) {
            if (left % sqrtN == 0 && (right + 1) % sqrtN == 0)
                blockInv[lIdx] = !blockInv[lIdx];
            else
                doFlip(left, right, lIdx);
        } else {
            if (left % sqrtN == 0)
                blockInv[lIdx] = !blockInv[lIdx];
            else
                doFlip(left, (lIdx + 1) * sqrtN - 1, lIdx);

            if ((right + 1) % sqrtN == 0)
                blockInv[rIdx] = !blockInv[rIdx];
            else
                doFlip(rIdx * sqrtN, right, rIdx);

            for (int i = lIdx + 1; i < rIdx; i++)
                blockInv[i] = !blockInv[i];
        }
    }

protected:
    int doCountOne(int left, int right) {
        int lIndex = left >> INDEX_SHIFT;
        int rIndex = right >> INDEX_SHIFT;
        int lOffset = left & INDEX_MASK;
        int rOffset = (right + 1) & INDEX_MASK;
        ValueT lMask = ~((BIT_ONE << lOffset) - BIT_ONE);
        ValueT rMask = rOffset ? (BIT_ONE << rOffset) - BIT_ONE : BIT_ALL;

        if (lIndex == rIndex)
            return popcount(values[lIndex] & lMask & rMask);

        int res = popcount(values[lIndex] & lMask);
        res += popcount(values[rIndex] & rMask);
        for (int i = lIndex + 1; i < rIndex; i++)
            res += popcount(values[i]);

        return res;
    }

    void doFlip(int left, int right, int blockId) {
        int lIndex = left >> INDEX_SHIFT;
        int rIndex = right >> INDEX_SHIFT;
        int lOffset = left & INDEX_MASK;
        int rOffset = (right + 1) & INDEX_MASK;
        ValueT lMask = ~((BIT_ONE << lOffset) - BIT_ONE);
        ValueT rMask = rOffset ? (BIT_ONE << rOffset) - BIT_ONE : BIT_ALL;

        if (lIndex == rIndex) {
            lMask &= rMask;
            blockOne[blockId] += popcount(~values[lIndex] & lMask) - popcount(values[lIndex] & lMask);
            values[lIndex] ^= lMask;
        } else {
            blockOne[blockId] += popcount(~values[lIndex] & lMask) - popcount(values[lIndex] & lMask);
            values[lIndex] ^= lMask;

            blockOne[blockId] += popcount(~values[rIndex] & rMask) - popcount(values[rIndex] & rMask);
            values[rIndex] ^= rMask;
            for (int i = lIndex + 1; i < rIndex; i++) {
                blockOne[blockId] += popcount(~values[i]) - popcount(values[i]);
                values[i] = ~values[i];
            }
        }
    }

    void apply(int blockId) {
        if (blockInv[blockId]) {
            doFlip(blockId * sqrtN, (blockId + 1) * sqrtN - 1, blockId);
            blockInv[blockId] = false;
        }
    }

    static int popcount(unsigned x) {
#ifndef __GNUC__
        return int(__popcnt(x));
#else
        return __builtin_popcount(x);
#endif
    }

    static int popcount(unsigned long long x) {
#if defined(_M_X64)
        return int(__popcnt64(x));
#elif defined(__GNUC__)
        return __builtin_popcountll(x);
#else
        return int(__popcnt(unsigned(x))) + int(__popcnt(unsigned(x >> 32)));
#endif
    }
};
