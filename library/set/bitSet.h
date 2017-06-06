#pragma once

struct BitSet {
    static const int BIT_SIZE = sizeof(unsigned long long) * 8;
    static const int BIT_MASK = 0x3F;
    static const int BIT_SHIFT = 6;
    static const unsigned long long BIT_ALL = (unsigned long long)-1ll;

    int mN;
    int mBitCnt;

    unsigned long long mEndMask;
    vector<unsigned long long> mV;

    void init(int N) {
        mBitCnt = 0;

        int r = N % BIT_SIZE;
        mEndMask = r ? ((1ull << r) - 1ull) : 0ull;

        mV.resize((N + BIT_SIZE - 1) / BIT_SIZE);
    }

    int size() const {
        return mN;
    }

    int count() const {
        return mBitCnt;
    }

    bool all() const {
        return mBitCnt >= mN;
    }

    bool any() const {
        return mBitCnt > 0;
    }

    bool none() const {
        return mBitCnt <= 0;
    }

    bool test(int pos) const {
        return (mV[pos >> BIT_SHIFT] & (1ull << (pos & BIT_MASK))) != 0;
    }

    BitSet& set() {
        if (mBitCnt < mN) {
            int n = mN >> BIT_SHIFT;
            for (int i = 0; i < n; i++)
                mV[i] = BIT_ALL;
            if (mEndMask)
                mV[n] = mEndMask;

            mBitCnt = mN;
        }
        return *this;
    }

    BitSet& set(int pos, bool value = true) {
        int idx = (pos >> BIT_SHIFT);
        int off = (pos & BIT_MASK);
        if (value) {
            if ((mV[idx] & (1ull << off)) == 0) {
                mV[idx] |= 1ull << off;
                mBitCnt++;
            }
        } else {
            if (mV[idx] & (1ull << off)) {
                mV[idx] &= ~(1ull << off);
                mBitCnt--;
            }
        }
        return *this;
    }

    BitSet& reset() {
        if (mBitCnt > 0) {
            int n = (mN + BIT_SIZE - 1) >> BIT_SHIFT;
            for (int i = 0; i < n; i++)
                mV[i] = 0;
            mBitCnt = 0;
        }
        return *this;
    }

    BitSet& reset(int pos) {
        int idx = (pos >> BIT_SHIFT);
        int off = (pos & BIT_MASK);
        if (mV[idx] & (1ull << off)) {
            mV[idx] &= ~(1ull << off);
            mBitCnt--;
        }

        return *this;
    }

    BitSet& flip() {
        int n = mN >> BIT_SHIFT;
        for (int i = 0; i < n; i++)
            mV[i] ^= BIT_ALL;
        if (mEndMask)
            mV[n] ^= mEndMask;

        mBitCnt = mN - mBitCnt;

        return *this;
    }

    BitSet& flip(int pos) {
        int idx = (pos >> BIT_SHIFT);
        int off = (pos & BIT_MASK);
        if ((mV[idx] & (1ull << off)) == 0) {
            mV[idx] |= 1ull << off;
            mBitCnt++;
        } else {
            mV[idx] &= ~(1ull << off);
            mBitCnt--;
        }
        return *this;
    }

    bool operator ==(const BitSet& rhs) const {
        if (mBitCnt != rhs.mBitCnt)
            return false;

        int n1 = (mN + BIT_SIZE - 1) >> BIT_SHIFT;
        int n2 = (rhs.mN + BIT_SIZE - 1) >> BIT_SHIFT;
        if (n1 != n2)
            return false;

        for (int i = 0; i < n1; i++) {
            if (mV[i] != rhs.mV[i])
                return false;
        }
        return true;
    }

    bool operator !=(const BitSet& rhs) const {
        return !operator ==(rhs);
    }

    BitSet& operator |=(const BitSet& rhs) {
        int n1 = (mN + BIT_SIZE - 1) >> BIT_SHIFT;
        int n2 = (rhs.mN + BIT_SIZE - 1) >> BIT_SHIFT;

        int n = min(n1, n2);
        for (int i = 0; i < n; i++) {
            unsigned long long t = (mV[i] ^ rhs.mV[i]) & rhs.mV[i];
            mV[i] |= rhs.mV[i];
            while (t) {
                t &= t - 1;
                mBitCnt++;
            }
        }
        return *this;
    }

    BitSet& operator &=(const BitSet& rhs) {
        int n1 = (mN + BIT_SIZE - 1) >> BIT_SHIFT;
        int n2 = (rhs.mN + BIT_SIZE - 1) >> BIT_SHIFT;

        int n = min(n1, n2);
        for (int i = 0; i < n; i++) {
            unsigned long long t = (mV[i] ^ rhs.mV[i]) & mV[i];
            mV[i] &= rhs.mV[i];
            while (t) {
                t &= t - 1;
                mBitCnt--;
            }
        }
        return *this;
    }

    BitSet& operator ^=(const BitSet& rhs) {
        int n1 = (mN + BIT_SIZE - 1) >> BIT_SHIFT;
        int n2 = (rhs.mN + BIT_SIZE - 1) >> BIT_SHIFT;

        int n = min(n1, n2);
        for (int i = 0; i < n; i++) {
            unsigned long long t1 = (mV[i] & rhs.mV[i]);
            unsigned long long t2 = rhs.mV[i] & ~t1;

            mV[i] ^= rhs.mV[i];

            while (t2) {
                t2 &= t2 - 1;
                mBitCnt++;
            }
            while (t1) {
                t1 &= t1 - 1;
                mBitCnt--;
            }
        }
        return *this;
    }

    BitSet operator ~() const {
        BitSet bs = *this;
        bs.flip();
        return std::move(bs);
    }

    BitSet& operator <<=(int n) {
        //TODO: implement this function
        return *this;
    }

    BitSet& operator >>=(int n) {
        //TODO: implement this function
        return *this;
    }

    int first() const {
        if (mBitCnt <= 0)
            return -1;

        for (int i = 0; i < (int)mV.size(); i++) {
            if (mV[i]) {
                int j = 0;
                for (unsigned long long x = mV[i]; !(x & 1); x >>= 1, j++)
                    ;
                return i * BIT_SIZE + j;
            }
        }

        return -1;
    }
};
