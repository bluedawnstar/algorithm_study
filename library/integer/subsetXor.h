#pragma once

// https://codeforces.com/blog/entry/68953
// https://drschwad.github.io/2019-08-06-z2-space-xor-trick/

// in vector space
template <int mod = 1000000007>
struct SubsetXor {
    int N;
    int bitSize;
    vector<int> pow2;

    vector<int> basis;
    int basisCount;
    int valueCount;

    void init(int maxN, int bitSize) {
        this->N = maxN;
        this->bitSize = bitSize;

        pow2.resize(maxN + 1);

        pow2[0] = 1;
        for (int i = 1; i <= maxN; i++)
            pow2[i] = int(2ll * pow2[i - 1] % mod);

        basis = vector<int>(bitSize);
        basisCount = 0;
        valueCount = 0;
    }

    //---

    void clear() {
        memset(basis.data(), 0, sizeof(basis[0]) * bitSize);
        basisCount = 0;
        valueCount = 0;
    }

    // 0 <= x < 2^bitSize
    void insert(int x) {
        valueCount++;
        if (basisCount >= bitSize)
            return;

        for (int i = 0, mask = 1; i < bitSize; i++, mask <<= 1) {
            if ((x & mask) == 0)
                continue;

            if (!basis[i]) {
                // If there is no basis vector with the i'th bit set,
                //  then insert this vector into the basis
                basis[i] = x;
                ++basisCount;
                return;
            }
            x ^= basis[i];  // otherwise subtract the basis vector from this vector
        }
    }

    // check if there is any subset whose XOR value is the target.
    bool checkXor(int target) const {
        for (int i = 0, mask = 1; i < bitSize; i++, mask <<= 1) {
            if ((target & mask) == 0)
                continue;

            if (!basis[i])
                return false;

            target ^= basis[i];
        }
        return true;
    }

    //---

    // the number of subsets whose XOR value is the target.
    // [CAUTION] if target is 0, then result includes empty set
    int countSubsetXor(int target) const {
        int res = 0;
        if (checkXor(target))
            res = pow2[valueCount - basisCount];
        return res;
    }

    // the number of distinct integers that can be represented using XOR over the subset of the given elements.
    int countDistinctXorIntegers() const {
        return pow2[basisCount];    // 2^basisCount
    }

    // the maximum possible XOR of the elements of some subset
    int getMaxSubsetXor() const {
        int res = 0;
        for (int i = bitSize - 1; i >= 0; i--) {
            if (!basis[i])
                continue;

            if (res & (1 << i))
                continue;

            res ^= basis[i];
        }

        return res;
    }

    // the Kth number from the set of all possible XOR of elements from a subset
    // 1 <= kth <= |set|
    int kthSubsetXor(int kth) const {
        int res = 0;

        int total = 1 << basisCount;
        for (int i = basisCount - 1; i >= 0; i--) {
            if (basis[i]) {
                int low = total >> 1;

                if ((low < kth && (res & (1 << i)) == 0) ||
                    (low >= kth && (res & (1 << i)) > 0))
                    res ^= basis[i];

                if (low < kth)
                    kth -= low;

                total >>= 1;
            }
        }

        return res;
    }
};
