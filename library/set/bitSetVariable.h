#pragma once

// BitSet class without mBitCnt member variable
struct BitSetVariable {
    static const int BIT_SIZE = sizeof(unsigned) * 8;
    static const unsigned BIT_ALL = (unsigned)-1;
    static const unsigned BIT_ONE = 1u;

    static const int INDEX_MASK = 0x1F;
    static const int INDEX_SHIFT = 5;

    vector<unsigned> mV;

    int size() const {
        return (int)mV.size() * BIT_SIZE;
    }

    void resize(int size) {
        size = (size + BIT_SIZE - 1) >> INDEX_SHIFT;
        if ((int)mV.size() < size)
            mV.resize((size_t)size);
    }

    int count() const {
        int res = 0;
        for (int i = 0; i < (int)mV.size(); i++)
            res += popCount(mV[i]);
        return res;
    }

    bool test(int pos) const {
        if (pos >= size())
            return false;
        return (mV[pos >> INDEX_SHIFT] & (BIT_ONE << (pos & INDEX_MASK))) != 0;
    }

    BitSetVariable& set(int pos, bool value = true) {
        int idx = pos >> INDEX_SHIFT;
        int off = pos & INDEX_MASK;

        if (value) {
            resize(pos + 1);
            mV[idx] |= BIT_ONE << off;
        } else if (idx < (int)mV.size()) {
            mV[idx] &= ~(BIT_ONE << off);
        }
        return *this;
    }

    BitSetVariable& reset(int pos) {
        int idx = pos >> INDEX_SHIFT;
        int off = pos & INDEX_MASK;

        if (idx < (int)mV.size())
            mV[idx] &= ~(BIT_ONE << off);

        return *this;
    }

    BitSetVariable& flip(int pos) {
        int idx = pos >> INDEX_SHIFT;
        int off = pos & INDEX_MASK;

        resize(pos + 1);
        if ((mV[idx] & (BIT_ONE << off)) == 0)
            mV[idx] |= BIT_ONE << off;
        else
            mV[idx] &= ~(BIT_ONE << off);

        return *this;
    }

    bool operator ==(const BitSetVariable& rhs) const {
        int n = (int)min(mV.size(), rhs.mV.size());
        for (int i = 0; i < n; i++) {
            if (mV[i] != rhs.mV[i])
                return false;
        }

        const vector<unsigned int>& maxV = (mV.size() >= rhs.mV.size()) ? mV : rhs.mV;
        for (int i = n; i < (int)maxV.size(); i++)
            if (!maxV[i])
                return false;

        return true;
    }

    bool operator !=(const BitSetVariable& rhs) const {
        return !operator ==(rhs);
    }

    BitSetVariable& operator |=(const BitSetVariable& rhs) {
        if (mV.size() < rhs.mV.size())
            mV.resize(rhs.mV.size());

        for (int i = 0; i < (int)rhs.mV.size(); i++)
            mV[i] |= rhs.mV[i];
        return *this;
    }

    BitSetVariable& operator &=(const BitSetVariable& rhs) {
        int n = (int)min(mV.size(), rhs.mV.size());
        for (int i = 0; i < n; i++)
            mV[i] &= rhs.mV[i];
        for (int i = n; i < (int)mV.size(); i++)
            mV[i] = 0;
        return *this;
    }

    BitSetVariable& operator ^=(const BitSetVariable& rhs) {
        if (mV.size() < rhs.mV.size())
            mV.resize(rhs.mV.size());

        for (int i = 0; i < (int)rhs.mV.size(); i++)
            mV[i] ^= rhs.mV[i];
        return *this;
    }

    BitSetVariable operator |(const BitSetVariable& rhs) {
        BitSetVariable bs;
        bs.mV.resize(max(mV.size(), rhs.mV.size()));

        int n = (int)min(mV.size(), rhs.mV.size());
        for (int i = 0; i < n; i++)
            bs.mV[i] = mV[i] | rhs.mV[i];

        if (mV.size() < rhs.mV.size()) {
            for (int i = n; i < (int)rhs.mV.size(); i++)
                bs.mV[i] = rhs.mV[i];
        } else {
            for (int i = n; i < (int)mV.size(); i++)
                bs.mV[i] = mV[i];
        }

        return bs;
    }

    BitSetVariable operator &(const BitSetVariable& rhs) {
        BitSetVariable bs;
        bs.mV.resize(min(mV.size(), rhs.mV.size()));

        int n = (int)min(mV.size(), rhs.mV.size());
        for (int i = 0; i < n; i++)
            bs.mV[i] = mV[i] & rhs.mV[i];

        return bs;
    }

    BitSetVariable operator ^(const BitSetVariable& rhs) {
        BitSetVariable bs;
        bs.mV.resize(max(mV.size(), rhs.mV.size()));

        int n = (int)min(mV.size(), rhs.mV.size());
        for (int i = 0; i < n; i++)
            bs.mV[i] = mV[i] ^ rhs.mV[i];

        if (mV.size() < rhs.mV.size()) {
            for (int i = n; i < (int)rhs.mV.size(); i++)
                bs.mV[i] = rhs.mV[i];
        } else {
            for (int i = n; i < (int)mV.size(); i++)
                bs.mV[i] = mV[i];
        }

        return bs;
    }

    BitSetVariable operator ~() const {
        BitSetVariable bs;
        bs.mV.resize(mV.size());

        for (int i = 0; i < (int)mV.size(); i++)
            bs.mV[i] = ~mV[i];

        return bs;
    }

    BitSetVariable& operator <<=(int n) {
        if (n <= 0)
            return *this;

        int lastPos = last();
        if (lastPos + n >= size())
            resize(lastPos + n + 1);

        int d = n >> INDEX_SHIFT;
        int r = n & INDEX_MASK;

        if (r == 0) {
            int t = (size() - 1) >> INDEX_SHIFT;
            int s = t - d;

            while (s >= 0)
                mV[t--] = mV[s--];
            while (t >= 0)
                mV[t--] = 0;
        } else {
            int t = (size() - 1) >> INDEX_SHIFT;
            int s1 = t - d;
            int s0 = s1 - 1;

            while (s0 >= 0)
                mV[t--] = (mV[s1--] << r) | (mV[s0--] >> (BIT_SIZE - r));
            if (s1 >= 0)
                mV[t--] = (mV[s1--] << r);
            while (t >= 0)
                mV[t--] = 0;
        }

        return *this;
    }

    BitSetVariable& operator >>=(int n) {
        if (n <= 0)
            return *this;

        if (n >= size()) {
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

        return *this;
    }

    //-----------------------------------------------------

    int firstClearBit() const {
        for (int i = 0; i < (int)mV.size(); i++) {
            if (mV[i] != BIT_ALL) {
                int m = (int)~mV[i];
                return i * BIT_SIZE + (BIT_SIZE - 1) - clz(unsigned(m & -m));
            }
        }
        return size();
    }

    int first() const {
        for (int i = 0; i < (int)mV.size(); i++) {
            if (mV[i]) {
                int m = (int)mV[i];
                return i * BIT_SIZE + (BIT_SIZE - 1) - clz(unsigned(m & -m));
            }
        }
        return -1;
    }

    int last() const {
        for (int i = (int)mV.size() - 1; i >= 0; i--) {
            if (mV[i])
                return i * BIT_SIZE + (BIT_SIZE - 1) - clz(mV[i]);
        }
        return -1;
    }

    // pos < next(pos) < N (or -1)
    int next(int pos) const {
        if (++pos >= (int)mV.size() * BIT_SIZE)
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

    // -1 <= prev(pos) < pos
    int prev(int pos) const {
        if (--pos < 0)
            return -1;

        int index = pos >> INDEX_SHIFT;
        int offset = pos & INDEX_MASK;

        int m = (int)mV[index] & (BIT_ALL >> (BIT_SIZE - 1 - offset));
        if (m)
            return (index << INDEX_SHIFT) + BIT_SIZE - clz(m) - 1;

        for (int i = index - 1; i < (int)mV.size(); i--) {
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
