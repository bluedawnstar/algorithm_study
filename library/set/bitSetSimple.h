#pragma once

// BitSet class without mBitCnt member variable
struct BitSetSimple {
    static const int BIT_SIZE = sizeof(unsigned) * 8;
    static const unsigned BIT_ALL = (unsigned)-1;
    static const unsigned BIT_ONE = 1u;

    static const int INDEX_MASK = 0x1F;
    static const int INDEX_SHIFT = 5;

    int mN;

    unsigned mEndMask;
    vector<unsigned> mV;

    void init(int N) {
        mN = N;

        int r = N % BIT_SIZE;
        mEndMask = r ? ((BIT_ONE << r) - BIT_ONE) : BIT_ALL;
        mV = vector<unsigned>((N + BIT_SIZE - 1) / BIT_SIZE);
    }

    int size() const {
        return mN;
    }

    int count() const {
        int res = 0;
        for (int i = 0; i < (int)mV.size(); i++)
            res += popCount(mV[i]);
        return res;
    }

    bool all() const {
        return count() >= mN;
    }

    bool any() const {
        return count() > 0;
    }

    bool none() const {
        return count() <= 0;
    }

    bool test(int pos) const {
        return (mV[pos >> INDEX_SHIFT] & (BIT_ONE << (pos & INDEX_MASK))) != 0;
    }

    BitSetSimple& set() {
        int n = mN >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            mV[i] = BIT_ALL;
        if (mEndMask + 1 != 0)
            mV[n] = mEndMask;
        return *this;
    }

    BitSetSimple& set(int pos, bool value = true) {
        int idx = pos >> INDEX_SHIFT;
        int off = pos & INDEX_MASK;
        if (value) {
            if ((mV[idx] & (BIT_ONE << off)) == 0)
                mV[idx] |= BIT_ONE << off;
        } else {
            if (mV[idx] & (BIT_ONE << off))
                mV[idx] &= ~(BIT_ONE << off);
        }
        return *this;
    }

    BitSetSimple& reset() {
        int n = (mN + BIT_SIZE - 1) >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            mV[i] = 0;
        return *this;
    }

    BitSetSimple& reset(int pos) {
        int idx = pos >> INDEX_SHIFT;
        int off = pos & INDEX_MASK;
        if (mV[idx] & (BIT_ONE << off))
            mV[idx] &= ~(BIT_ONE << off);

        return *this;
    }

    BitSetSimple& flip() {
        int n = mN >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            mV[i] ^= BIT_ALL;
        if (mEndMask + 1 != 0)
            mV[n] ^= mEndMask;
        return *this;
    }

    BitSetSimple& flip(int pos) {
        int idx = pos >> INDEX_SHIFT;
        int off = pos & INDEX_MASK;
        if ((mV[idx] & (BIT_ONE << off)) == 0)
            mV[idx] |= BIT_ONE << off;
        else
            mV[idx] &= ~(BIT_ONE << off);
        return *this;
    }

    bool operator ==(const BitSetSimple& rhs) const {
        int n1 = (mN + BIT_SIZE - 1) >> INDEX_SHIFT;
        int n2 = (rhs.mN + BIT_SIZE - 1) >> INDEX_SHIFT;
        if (n1 != n2)
            return false;

        for (int i = 0; i < n1; i++) {
            if (mV[i] != rhs.mV[i])
                return false;
        }
        return true;
    }

    bool operator !=(const BitSetSimple& rhs) const {
        return !operator ==(rhs);
    }

    BitSetSimple& operator |=(const BitSetSimple& rhs) {
        int n1 = (mN + BIT_SIZE - 1) >> INDEX_SHIFT;
        int n2 = (rhs.mN + BIT_SIZE - 1) >> INDEX_SHIFT;

        int n = min(n1, n2);
        for (int i = 0; i < n; i++)
            mV[i] |= rhs.mV[i];

        return *this;
    }

    BitSetSimple& operator &=(const BitSetSimple& rhs) {
        int n1 = (mN + BIT_SIZE - 1) >> INDEX_SHIFT;
        int n2 = (rhs.mN + BIT_SIZE - 1) >> INDEX_SHIFT;

        int n = min(n1, n2);
        for (int i = 0; i < n; i++)
            mV[i] &= rhs.mV[i];

        return *this;
    }

    BitSetSimple& operator ^=(const BitSetSimple& rhs) {
        int n1 = (mN + BIT_SIZE - 1) >> INDEX_SHIFT;
        int n2 = (rhs.mN + BIT_SIZE - 1) >> INDEX_SHIFT;

        int n = min(n1, n2);
        for (int i = 0; i < n; i++)
            mV[i] ^= rhs.mV[i];

        return *this;
    }

    BitSetSimple operator |(const BitSetSimple& rhs) {
        BitSetSimple bs = *this;
        bs.init(mN);

        int n = mN >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            bs.mV[i] = mV[i] | rhs.mV[i];
        if (mEndMask + 1 != 0)
            bs.mV[n] = (mV[n] | rhs.mV[n]);

        return bs;
    }

    BitSetSimple operator &(const BitSetSimple& rhs) {
        BitSetSimple bs = *this;
        bs.init(mN);

        int n = mN >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            bs.mV[i] = mV[i] & rhs.mV[i];
        if (mEndMask + 1 != 0)
            bs.mV[n] = mV[n] & rhs.mV[n];

        return bs;
    }

    BitSetSimple operator ^(const BitSetSimple& rhs) {
        BitSetSimple bs = *this;
        bs.init(mN);

        int n = mN >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            bs.mV[i] = mV[i] ^ rhs.mV[i];
        if (mEndMask + 1 != 0)
            bs.mV[n] = mV[n] ^ rhs.mV[n];

        return bs;
    }

    BitSetSimple operator ~() const {
        BitSetSimple bs = *this;
        bs.init(mN);

        int n = mN >> INDEX_SHIFT;
        for (int i = 0; i < n; i++)
            bs.mV[i] = ~mV[i];
        if (mEndMask + 1 != 0)
            bs.mV[n] = ~mV[n] & mEndMask;

        return bs;
    }

    BitSetSimple& operator <<=(int n) {
        if (n <= 0)
            return *this;

        if (n >= mN) {
            fill(mV.begin(), mV.end(), 0);
            return *this;
        }

        int d = n >> INDEX_SHIFT;
        int r = n & INDEX_MASK;

        if (r == 0) {
            int t = (mN - 1) >> INDEX_SHIFT;
            int s = t - d;

            while (s >= 0)
                mV[t--] = mV[s--];
            while (t >= 0)
                mV[t--] = 0;
        } else {
            int t = (mN - 1) >> INDEX_SHIFT;
            int s1 = t - d;
            int s0 = s1 - 1;

            while (s0 >= 0)
                mV[t--] = (mV[s1--] << r) | (mV[s0--] >> (BIT_SIZE - r));
            if (s1 >= 0)
                mV[t--] = (mV[s1--] << r);
            while (t >= 0)
                mV[t--] = 0;
        }
        if (mEndMask + 1 != 0)
            mV[mN >> INDEX_SHIFT] &= mEndMask;

        return *this;
    }

    BitSetSimple& operator >>=(int n) {
        if (n <= 0)
            return *this;

        if (n >= mN) {
            fill(mV.begin(), mV.end(), 0);
            return *this;
        }

        int d = n >> INDEX_SHIFT;
        int r = n & INDEX_MASK;

        int VN = (int)mV.size();

        if (r == 0) {
            int t = 0;
            int s = t + d;

            while (s < VN)
                mV[t++] = mV[s++];
            while (t < VN)
                mV[t++] = 0;
        } else {
            int t = 0;
            int s0 = t + d;
            int s1 = s0 + 1;

            while (s1 < VN)
                mV[t++] = (mV[s0++] >> r) | (mV[s1++] << (BIT_SIZE - r));
            if (s0 < VN)
                mV[t++] = (mV[s0++] >> r);
            while (t < VN)
                mV[t++] = 0;
        }
        if (mEndMask + 1 != 0)
            mV[mN >> INDEX_SHIFT] &= mEndMask;

        return *this;
    }

    //-----------------------------------------------------

    int firstClearBit() const {
        for (int i = 0; i < (int)mV.size(); i++) {
            if (mV[i] != BIT_ALL) {
                int m = (int)~mV[i];
                return i * BIT_SIZE + BIT_SIZE - clz(unsigned(m & -m)) - 1;
            }
        }
        return size();
    }

    int first() const {
        for (int i = 0; i < (int)mV.size(); i++) {
            if (mV[i]) {
                int m = (int)mV[i];
                return i * BIT_SIZE + BIT_SIZE - clz(unsigned(m & -m)) - 1;
            }
        }
        return -1;
    }

    int last() const {
        for (int i = (int)mV.size() - 1; i >= 0; i--) {
            if (mV[i])
                return i * BIT_SIZE + BIT_SIZE - clz(mV[i]) - 1;
        }
        return -1;
    }

    int next(int pos) const {
        if (++pos >= mN)
            return -1;

        int index = pos >> INDEX_SHIFT;
        int offset = pos & INDEX_MASK;

        int m = (int)mV[index] & (BIT_ALL << offset);
        if (m)
            return (index << INDEX_SHIFT) + BIT_SIZE - clz(unsigned(m & -m)) - 1;

        for (int i = index + 1; i < (int)mV.size(); i++) {
            if (mV[i]) {
                m = (int)mV[i];
                return (i << INDEX_SHIFT) + BIT_SIZE - clz(unsigned(m & -m)) - 1;
            }
        }

        return -1;
    }

    int prev(int pos) const {
        if (--pos < 0)
            return -1;

        int index = pos >> INDEX_SHIFT;
        int offset = pos & INDEX_MASK;

        int m = (int)mV[index] & (BIT_ALL >> (BIT_SIZE - 1 - offset));
        if (m)
            return (index << INDEX_SHIFT) + BIT_SIZE - clz(m) - 1;

        for (int i = index - 1; i >= 0; i--) {
            if (mV[i])
                return (i << INDEX_SHIFT) + BIT_SIZE - clz(mV[i]) - 1;
        }

        return -1;
    }

    static int clz(unsigned x) {
#ifndef __GNUC__
        return (int)__lzcnt(x);
#else
        return __builtin_clz(x);
#endif
    }

    static int popCount(unsigned x) {
#ifndef __GNUC__
        return (int)__popcnt(x);
        /*
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x + (x >> 4)) & 0x0F0F0F0F;
        x = x + (x >> 8);
        x = x + (x >> 16);
        return x & 0x0000003F;
        */
#else
        return __builtin_popcount(x);
#endif
    }
};
