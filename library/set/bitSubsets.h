#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

struct BitSubsets {
    static int clz(unsigned x) {
        if (!x)
            return 32;
#ifndef __GNUC__
        return int(__lzcnt(x));
#else
        return __builtin_clz(x);
#endif
    }

    static int popCount(unsigned x) {
#ifndef __GNUC__
        return int(__popcnt(x));
#else
        return __builtin_popcount(x);
#endif
    }

    static int clz(unsigned long long x) {
        if (!x)
            return 64;
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

#if 0
    typedef unsigned int T;
    typedef int SignedT;

    static const int BIT_SIZE = sizeof(unsigned) * 8;
    static const unsigned BIT_ALL = ~0u;
    static const unsigned BIT_ONE = 1u;

    static const int INDEX_MASK = 0x1F;
    static const int INDEX_SHIFT = 5;
#else
    typedef unsigned long long T;
    typedef long long SignedT;

    static const int BIT_SIZE = sizeof(unsigned long long) * 8;
    static const unsigned long long BIT_ALL = ~0ull;
    static const unsigned long long BIT_ONE = 1ull;

    static const int INDEX_MASK = 0x3F;
    static const int INDEX_SHIFT = 6;
#endif

    static const unsigned int ZERO_BLOCK = 0u;

    struct BlockManager {
        int blockSize;
        int blockCountInChunk;
        int blockBitCount;

        int unusedCount;
        unsigned int freeBlock;
        static const unsigned int NULL_BLOCK = (unsigned int)-1;

        std::vector<std::unique_ptr<int[]>> refCnt;
        std::vector<std::unique_ptr<T[]>> blocks;

        BlockManager() {
            freeBlock = NULL_BLOCK;
            unusedCount = 0;
        }

        BlockManager(int blockSize, int blockCountInChunk) {
            init(blockSize, blockCountInChunk);
        }

        void init(int blockSize, int blockCountInChunk) {
            this->blockSize = blockSize;
            this->blockCountInChunk = min(blockCountInChunk, 1 << 16);
            blockBitCount = blockSize * sizeof(T) * 8;
            freeBlock = NULL_BLOCK;
            unusedCount = 0;
        }

        int getRefCnt(unsigned int id) const {
            return refCnt[id >> 16][(unsigned short)(id)];
        }

        int incRef(unsigned int id) {
            return ++refCnt[id >> 16][(unsigned short)(id)];
        }

        void decRef(unsigned int id) {
            if (id > 0 && --refCnt[id >> 16][(unsigned short)(id)] <= 0)
                deallocate(id);
        }

        int isShared(unsigned int id) const {
            return id == ZERO_BLOCK || refCnt[id >> 16][(unsigned short)(id)] > 1;
        }

        T* get(unsigned int id) {
            return &blocks[id >> 16][blockSize * (unsigned short)(id)];
        }

        const T* get(unsigned int id) const {
            return &blocks[id >> 16][blockSize * (unsigned short)(id)];
        }

        unsigned int allocate() {
            if (blocks.empty() || (freeBlock == NULL_BLOCK && unusedCount <= 0)) {
                blocks.emplace_back(new T[blockSize * blockCountInChunk]);
                refCnt.emplace_back(new int[blockCountInChunk]);
                unusedCount = blockCountInChunk;
                memset(refCnt.back().get(), 0, sizeof(refCnt[0][0]) * blockCountInChunk);
            }

            unsigned int res = 0;
            if (unusedCount > 0) {
                res = ((unsigned int)(blocks.size() - 1) << 16) | (unsigned int)(blockCountInChunk - unusedCount--);
            } else {
                res = freeBlock;
                freeBlock = *reinterpret_cast<unsigned int*>(get(res));
            }
            incRef(res);
            return res;
        }

        unsigned int clone(unsigned int id) {
            auto res = allocate();
            memcpy(get(res), get(id), sizeof(T) * blockSize);
            decRef(id);
            return res;
        }

        unsigned int clone(T* p) {
            auto res = allocate();
            memcpy(get(res), p, sizeof(T) * blockSize);
            return res;
        }

        void fillZero(unsigned int id) {
            T* p = get(id);
            memset(p, 0, sizeof(T) * blockSize);
        }

        bool test(unsigned int id, int index) const {
            const T* p = get(id);
            return (p[index >> INDEX_SHIFT] & (BIT_ONE << (index & INDEX_MASK))) != 0;
        }

        void set(unsigned int id, int index) {
            T* p = get(id);
            p[index >> INDEX_SHIFT] |= BIT_ONE << (index & INDEX_MASK);
        }

        void reset(unsigned int id, int index) {
            T* p = get(id);
            p[index >> INDEX_SHIFT] &= ~(BIT_ONE << (index & INDEX_MASK));
        }

    private:
        void deallocate(unsigned int id) {
            *reinterpret_cast<unsigned int*>(get(id)) = freeBlock;
            freeBlock = id;
        }
    };

    int N;
    int sqrtBlockBitN;
    int sqrtBlockSize;
    int sqrtBlockN;

    BlockManager blocks;

    typedef unique_ptr<unsigned int[]> SubsetT;
    typedef vector<SubsetT> SubsetVectorT;

    BitSubsets() {
    }

    explicit BitSubsets(int _valueN) {
        init(_valueN);
    }

    void init(int _valueN) {
        N = _valueN;
        sqrtBlockSize = (int(sqrt(N)) + BIT_SIZE - 1) / BIT_SIZE;
        sqrtBlockBitN = sqrtBlockSize * BIT_SIZE;
        sqrtBlockN = (N + sqrtBlockBitN - 1) / sqrtBlockBitN;

        blocks.init(sqrtBlockSize, 1 << 16);
        auto id = blocks.allocate();    // allocate zero block
        blocks.fillZero(id);
    }

    void set(SubsetT& subset, int index) {
        int blkIndex = index / sqrtBlockBitN;
        int blkOffset = index % sqrtBlockBitN;

        unsigned int id = subset[blkIndex];
        //if (blocks.test(id, index))
        //    return;
        if (blocks.isShared(id))
            subset[blkIndex] = id = blocks.clone(id);
        blocks.set(id, blkOffset);
    }

    void reset(SubsetT& subset, int index) {
        int blkIndex = index / sqrtBlockBitN;
        int blkOffset = index % sqrtBlockBitN;

        unsigned int id = subset[blkIndex];
        //if (!blocks.test(id, index))
        //    return;
        if (blocks.isShared(id))
            subset[blkIndex] = id = blocks.clone(id);
        blocks.reset(id, blkOffset);
    }

    bool test(SubsetT& subset, int index) {
        int blkIndex = index / sqrtBlockBitN;
        int blkOffset = index % sqrtBlockBitN;

        unsigned int id = subset[blkIndex];
        if (id == ZERO_BLOCK)
            return false;
        return blocks.test(id, blkOffset);
    }

    int count(SubsetT& subset) {
        int res = 0;

        auto src = subset.get();
        for (int i = 0; i < sqrtBlockN; i++) {
            if (src[i] != ZERO_BLOCK) {
                T* psrc = blocks.get(src[i]);
                for (int n = sqrtBlockSize; n > 0; n--)
                    res += popCount(*psrc++);
            }
        }

        return res;
    }

    // return index of kth bit (1 <= k)
    int kthBit(SubsetT& subset, int k) {
        auto src = subset.get();
        for (int i = 0; i < sqrtBlockN; i++) {
            if (src[i] != ZERO_BLOCK) {
                T* psrc = blocks.get(src[i]);
                for (int j = 0; j < sqrtBlockSize; j++) {
                    auto t = *psrc++;
                    int n = popCount(t);

                    if (n >= k) {
                        while (--k > 0)
                            t &= t - 1;

                        SignedT t2 = (SignedT)t & -(SignedT)t;
                        return i * sqrtBlockBitN + j * BIT_SIZE + popCount((T)t2 - 1);
                    }
                    k -= n;
                }
            }
        }

        return -1;
    }

    void copy(SubsetT& subsetDst, SubsetT& subsetSrc) {
        if (&subsetDst == &subsetSrc)
            return;

        auto dst = subsetDst.get();
        auto src = subsetSrc.get();
        for (int i = 0; i < sqrtBlockN; i++) {
            dst[i] = src[i];
            blocks.incRef(src[i]);
        }
    }

    void doAnd(SubsetT& subsetDst, SubsetT& subsetSrc) {
        if (&subsetDst == &subsetSrc)
            return;

        auto dst = subsetDst.get();
        auto src = subsetSrc.get();
        for (int i = 0; i < sqrtBlockN; i++) {
            if (dst[i] == ZERO_BLOCK)
                ;
            else if (src[i] == ZERO_BLOCK) {
                blocks.decRef(dst[i]);
                dst[i] = ZERO_BLOCK;
            } else {
                if (blocks.isShared(dst[i]))
                    dst[i] = blocks.clone(dst[i]);

                T* pdst = blocks.get(dst[i]);
                T* psrc = blocks.get(src[i]);
                for (int n = sqrtBlockSize; n > 0; n--)
                    *pdst++ &= *psrc++;
            }
        }
    }

    void doOr(SubsetT& subsetDst, SubsetT& subsetSrc) {
        if (&subsetDst == &subsetSrc)
            return;

        auto dst = subsetDst.get();
        auto src = subsetSrc.get();
        for (int i = 0; i < sqrtBlockN; i++) {
            if (src[i] == ZERO_BLOCK)
                continue;

            if (dst[i] == ZERO_BLOCK) {
                blocks.incRef(src[i]);
                dst[i] = src[i];
            } else {
                if (blocks.isShared(dst[i]))
                    dst[i] = blocks.clone(dst[i]);

                T* pdst = blocks.get(dst[i]);
                T* psrc = blocks.get(src[i]);
                for (int n = sqrtBlockSize; n > 0; n--)
                    *pdst++ |= *psrc++;
            }
        }
    }

    void doXor(SubsetT& subsetDst, SubsetT& subsetSrc) {
        auto dst = subsetDst.get();
        auto src = subsetSrc.get();
        if (subsetDst == subsetSrc) {
            for (int i = 0; i < sqrtBlockN; i++) {
                if (dst[i] != ZERO_BLOCK) {
                    blocks.decRef(dst[i]);
                    dst[i] = ZERO_BLOCK;
                }
            }
        } else {
            for (int i = 0; i < sqrtBlockN; i++) {
                if (src[i] == ZERO_BLOCK)
                    continue;

                if (dst[i] == ZERO_BLOCK) {
                    blocks.incRef(src[i]);
                    dst[i] = src[i];
                } else {
                    if (blocks.isShared(dst[i]))
                        dst[i] = blocks.clone(dst[i]);

                    T* pdst = blocks.get(dst[i]);
                    T* psrc = blocks.get(src[i]);
                    for (int n = sqrtBlockSize; n > 0; n--)
                        *pdst++ ^= *psrc++;
                }
            }
        }
    }

    void doNot(SubsetT& subset) {
        auto dst = subset.get();
        for (int i = 0; i < sqrtBlockN; i++) {
            if (blocks.isShared(dst[i]))
                dst[i] = blocks.clone(dst[i]);

            T* pdst = blocks.get(dst[i]);
            for (int n = sqrtBlockSize; n > 0; n--, pdst++)
                *pdst = ~*pdst;
        }
    }

    //-------------------------------------------------

    SubsetVectorT createSubsets(int cnt) {
        SubsetVectorT res(cnt);
        for (int i = 0; i < cnt; i++) {
            res[i].reset(new unsigned int[sqrtBlockN]);
            memset(res[i].get(), 0, sizeof(ZERO_BLOCK) * sqrtBlockN);
        }
        return res;
    }

    void destroySubset(SubsetT& subset) {
        unsigned int* p = subset.get();
        for (int i = 0; i < sqrtBlockN; i++) {
            if (*p != ZERO_BLOCK)
                blocks.decRef(*p);
            p++;
        }
        subset.reset();
    }

    void destroySubsets(SubsetVectorT& subsets) {
        for (int i = 0; i < int(subsets.size()); i++)
            destroySubset(subsets[i]);
        subsets.clear();
    }
};
