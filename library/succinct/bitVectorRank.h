#pragma once

struct BitVectorRank {
    static const int BIT_SIZE = sizeof(unsigned int) * 8;
    static const unsigned int BIT_ALL = (unsigned int)-1;

    static const int INDEX_MASK = 0x1F;
    static const int INDEX_SHIFT = 5;

    int mN;
    int mCount;
    vector<unsigned int> mV;
    vector<int> mRank;

    BitVectorRank() {
        //init(0);
    }

    explicit BitVectorRank(int size) {
        init(size);
    }

    void init(int size) {
        mN = size;
        mCount = 0;
        mV = vector<unsigned int>((size + BIT_SIZE - 1) / BIT_SIZE + 1);
        mRank = vector<int>(mV.size() + 1);
    }

    int size() const {
        return mN;
    }

    void set() {
        int n = mN >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            mV[i] = BIT_ALL;

        int r = mN & INDEX_MASK;
        if (r != 0)
            mV[n] = (1u << r) - 1u;
    }

    void set(int pos) {
        mV[pos >> INDEX_SHIFT] |= 1u << (pos & INDEX_MASK);
    }

    void reset() {
        fill(mV.begin(), mV.end(), 0u);
    }

    void reset(int pos) {
        mV[pos >> INDEX_SHIFT] &= ~(1u << (pos & INDEX_MASK));
    }

    void set(int pos, bool val) {
        if (val)
            set(pos);
        else
            reset(pos);
    }

    unsigned int get(int pos) const {
        return mV[pos >> INDEX_SHIFT] & (1u << (pos & INDEX_MASK));
    }

    bool test(int pos) const {
        return (mV[pos >> INDEX_SHIFT] & (1u << (pos & INDEX_MASK))) != 0;
    }

    //--- after set ---

    void buildRank() {
        mCount = 0;
        if (mN == 0)
            return;
        for (int i = 0; i < (int)mV.size(); i++) {
            mRank[i] = mCount;
            mCount += popcount(mV[i]);
        }
        mRank[mV.size()] = mCount;
    }

    int count() const {
        return mCount;
    }

    // inclusive [0, pos]
    int rank1(int pos) const {
        if (pos < 0)
            return 0;
        else if (pos >= mN - 1)
            return mCount;
        int idx = ++pos >> INDEX_SHIFT;
        int off = pos & INDEX_MASK;
        return mRank[idx] + popcount(mV[idx] & ((1u << off) - 1u));
    }

    // inclusive [left, right]
    int rank1(int left, int right) const {
        return rank1(right) - rank1(left - 1);
    }

    // inclusive [0, pos]
    int rank0(int pos) const {
        if (pos < 0)
            return 0;
        return pos + 1 - rank1(pos);
    }

    // inclusive [left, right]
    int rank0(int left, int right) const {
        return rank0(right) - rank0(left - 1);
    }

    static int popcount(unsigned int x) {
#ifndef __GNUC__
        return (int)__popcnt(x);
        /*
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        return ((x + (x >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
        */
#else
        return __builtin_popcount(x);
#endif
    }
};
