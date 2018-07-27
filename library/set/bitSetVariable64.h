#pragma once

// BitSet class without mBitCnt member variable
struct BitSetVariable64 {
    static int clz(unsigned long long x) {
#if defined(_M_X64)
        return int(_lzcnt_u64(x));
#elif defined(__GNUC__)
        return __builtin_clzll(x);
#else
        if ((x >> 32) != 0)
            return int(_lzcnt_u32(unsigned(x >> 32)));
        else
            return 32 + int(_lzcnt_u32(unsigned(x)));
#endif
    }

    static int popCount(unsigned long long x) {
#if defined(_M_X64)
        return int(__popcnt64(x));
#elif defined(__GNUC__)
        return __builtin_popcountll(x);
#else
        return int(__popcnt(unsigned(x))) + int(__popcnt(unsigned(x >> 32)));
#endif
    }

    static const int BIT_SIZE = sizeof(unsigned long long) * 8;
    static const unsigned long long BIT_ALL = (unsigned long long)-1ll;
    static const unsigned long long BIT_ONE = 1ull;

    static const int INDEX_MASK = 0x3F;
    static const int INDEX_SHIFT = 6;

    vector<unsigned long long> values;

    int size() const {
        return int(values.size()) * BIT_SIZE;
    }

    void resize(int size) {
        size = (size + BIT_SIZE - 1) >> INDEX_SHIFT;
        if (int(values.size()) < size)
            values.resize((size_t)size);
    }

    int count() const {
        int res = 0;
        for (int i = 0; i < int(values.size()); i++)
            res += popCount(values[i]);
        return res;
    }

    bool test(int pos) const {
        if (pos >= size())
            return false;
        return (values[pos >> INDEX_SHIFT] & (BIT_ONE << (pos & INDEX_MASK))) != 0;
    }

    BitSetVariable64& set(int pos, bool value = true) {
        int idx = pos >> INDEX_SHIFT;
        int off = pos & INDEX_MASK;

        if (value) {
            resize(pos + 1);
            values[idx] |= BIT_ONE << off;
        } else if (idx < int(values.size())) {
            values[idx] &= ~(BIT_ONE << off);
        }
        return *this;
    }

    BitSetVariable64& reset(int pos) {
        int idx = pos >> INDEX_SHIFT;
        int off = pos & INDEX_MASK;

        if (idx < int(values.size()))
            values[idx] &= ~(BIT_ONE << off);

        return *this;
    }

    BitSetVariable64& flip(int pos) {
        int idx = pos >> INDEX_SHIFT;
        int off = pos & INDEX_MASK;

        resize(pos + 1);
        if ((values[idx] & (BIT_ONE << off)) == 0)
            values[idx] |= BIT_ONE << off;
        else
            values[idx] &= ~(BIT_ONE << off);

        return *this;
    }

    bool operator ==(const BitSetVariable64& rhs) const {
        int n = int(min(values.size(), rhs.values.size()));
        for (int i = 0; i < n; i++) {
            if (values[i] != rhs.values[i])
                return false;
        }

        const vector<unsigned long long>& maxV = (values.size() >= rhs.values.size()) ? values : rhs.values;
        for (int i = n; i < int(maxV.size()); i++)
            if (!maxV[i])
                return false;

        return true;
    }

    bool operator !=(const BitSetVariable64& rhs) const {
        return !operator ==(rhs);
    }

    BitSetVariable64& operator |=(const BitSetVariable64& rhs) {
        if (values.size() < rhs.values.size())
            values.resize(rhs.values.size());

        for (int i = 0; i < int(rhs.values.size()); i++)
            values[i] |= rhs.values[i];
        return *this;
    }

    BitSetVariable64& operator &=(const BitSetVariable64& rhs) {
        int n = int(min(values.size(), rhs.values.size()));
        for (int i = 0; i < n; i++)
            values[i] &= rhs.values[i];
        for (int i = n; i < int(values.size()); i++)
            values[i] = 0;
        return *this;
    }

    BitSetVariable64& operator ^=(const BitSetVariable64& rhs) {
        if (values.size() < rhs.values.size())
            values.resize(rhs.values.size());

        for (int i = 0; i < int(rhs.values.size()); i++)
            values[i] ^= rhs.values[i];
        return *this;
    }

    BitSetVariable64 operator |(const BitSetVariable64& rhs) {
        BitSetVariable64 bs;
        bs.values.resize(max(values.size(), rhs.values.size()));

        int n = int(min(values.size(), rhs.values.size()));
        for (int i = 0; i < n; i++)
            bs.values[i] = values[i] | rhs.values[i];

        if (values.size() < rhs.values.size()) {
            for (int i = n; i < int(rhs.values.size()); i++)
                bs.values[i] = rhs.values[i];
        } else {
            for (int i = n; i < int(values.size()); i++)
                bs.values[i] = values[i];
        }

        return bs;
    }

    BitSetVariable64 operator &(const BitSetVariable64& rhs) {
        BitSetVariable64 bs;
        bs.values.resize(min(values.size(), rhs.values.size()));

        int n = int(min(values.size(), rhs.values.size()));
        for (int i = 0; i < n; i++)
            bs.values[i] = values[i] & rhs.values[i];

        return bs;
    }

    BitSetVariable64 operator ^(const BitSetVariable64& rhs) {
        BitSetVariable64 bs;
        bs.values.resize(max(values.size(), rhs.values.size()));

        int n = int(min(values.size(), rhs.values.size()));
        for (int i = 0; i < n; i++)
            bs.values[i] = values[i] ^ rhs.values[i];

        if (values.size() < rhs.values.size()) {
            for (int i = n; i < int(rhs.values.size()); i++)
                bs.values[i] = rhs.values[i];
        } else {
            for (int i = n; i < int(values.size()); i++)
                bs.values[i] = values[i];
        }

        return bs;
    }

    BitSetVariable64 operator ~() const {
        BitSetVariable64 bs;
        bs.values.resize(values.size());

        for (int i = 0; i < int(values.size()); i++)
            bs.values[i] = ~values[i];

        return bs;
    }

    BitSetVariable64& operator <<=(int n) {
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
                values[t--] = values[s--];
            while (t >= 0)
                values[t--] = 0;
        } else {
            int t = (size() - 1) >> INDEX_SHIFT;
            int s1 = t - d;
            int s0 = s1 - 1;

            while (s0 >= 0)
                values[t--] = (values[s1--] << r) | (values[s0--] >> (BIT_SIZE - r));
            if (s1 >= 0)
                values[t--] = (values[s1--] << r);
            while (t >= 0)
                values[t--] = 0;
        }

        return *this;
    }

    BitSetVariable64& operator >>=(int n) {
        if (n <= 0)
            return *this;

        if (n >= size()) {
            fill(values.begin(), values.end(), 0);
            return *this;
        }

        int d = n >> INDEX_SHIFT;
        int r = n & INDEX_MASK;

        int VN = int(values.size());

        if (r == 0) {
            int t = 0;
            int s = t + d;

            while (s < VN)
                values[t++] = values[s++];
            while (t < VN)
                values[t++] = 0;
        } else {
            int t = 0;
            int s0 = t + d;
            int s1 = s0 + 1;

            while (s1 < VN)
                values[t++] = (values[s0++] >> r) | (values[s1++] << (BIT_SIZE - r));
            if (s0 < VN)
                values[t++] = (values[s0++] >> r);
            while (t < VN)
                values[t++] = 0;
        }

        return *this;
    }

    //-----------------------------------------------------

    int firstClearBit() const {
        for (int i = 0; i < int(values.size()); i++) {
            if (values[i] != BIT_ALL) {
                long long m = (long long)~values[i];
                return i * BIT_SIZE + (BIT_SIZE - 1) - clz((unsigned long long)(m & -m));
            }
        }
        return size();
    }

    int first() const {
        for (int i = 0; i < int(values.size()); i++) {
            if (values[i]) {
                long long m = (long long)values[i];
                return i * BIT_SIZE + (BIT_SIZE - 1) - clz((unsigned long long)(m & -m));
            }
        }
        return -1;
    }

    int last() const {
        for (int i = int(values.size()) - 1; i >= 0; i--) {
            if (values[i])
                return i * BIT_SIZE + (BIT_SIZE - 1) - clz(values[i]);
        }
        return -1;
    }

    // pos < next(pos) < N (or -1)
    int next(int pos) const {
        if (++pos >= int(values.size()) * BIT_SIZE)
            return -1;

        int index = pos >> INDEX_SHIFT;
        int offset = pos & INDEX_MASK;

        long long m = (long long)values[index] & (BIT_ALL << offset);
        if (m)
            return (index << INDEX_SHIFT) + BIT_SIZE - clz((unsigned long long)(m & -m)) - 1;

        for (int i = index + 1; i < int(values.size()); i++) {
            if (values[i]) {
                m = (long long)values[i];
                return (i << INDEX_SHIFT) + BIT_SIZE - clz((unsigned long long)(m & -m)) - 1;
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

        long long m = (long long)values[index] & (BIT_ALL >> (BIT_SIZE - 1 - offset));
        if (m)
            return (index << INDEX_SHIFT) + BIT_SIZE - clz(m) - 1;

        for (int i = index - 1; i >= 0; i--) {
            if (values[i])
                return (i << INDEX_SHIFT) + BIT_SIZE - clz(values[i]) - 1;
        }

        return -1;
    }
};
