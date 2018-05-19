#pragma once

#include <memory>
#include <vector>

//Minumum block size : 8 bytes (= pointer size)
struct FixedSizeAllocator {
    struct FreeBlockT {
        FreeBlockT* next;
    };

    size_t blockSize;
    unsigned int blockCountInChunk;

    unsigned int unusedCount;
    FreeBlockT* firstFreeBlock;

    std::vector<std::unique_ptr<unsigned char[]>> groups;

    FixedSizeAllocator(size_t blockSize, unsigned int blockCountInChunk)
        : blockSize(max(sizeof(FreeBlockT), blockSize)), blockCountInChunk(blockCountInChunk),
          unusedCount(0), firstFreeBlock(nullptr) {
    }

    ~FixedSizeAllocator() {
    }

    void* allocate() {
        if (groups.empty() || (!firstFreeBlock && unusedCount == 0)) {
            groups.emplace_back(new unsigned char[blockSize * blockCountInChunk]);
            unusedCount = blockCountInChunk;
        }

        if (unusedCount > 0) {
            return &groups.back()[0] + blockSize * (blockCountInChunk - unusedCount--);
        } else {
            auto* p = firstFreeBlock;
            firstFreeBlock = firstFreeBlock->next;
            return p;
        }
    }

    void deallocate(void* p) {
        auto* blk = reinterpret_cast<FreeBlockT*>(p);
        blk->next = firstFreeBlock;
        firstFreeBlock = blk;
    }
};

template <typename T>
struct TypeAllocator : public FixedSizeAllocator {
    explicit TypeAllocator(unsigned int chunkSize)
        : FixedSizeAllocator(sizeof(T), chunkSize) {
    }

    ~TypeAllocator() {
    }

    T* allocate() {
        return reinterpret_cast<T*>(FixedSizeAllocator::allocate());
    }

    template <typename... Args>
    T* construct(Args&&... args) {
        return ::new (FixedSizeAllocator::allocate()) T(std::forward<Args>(args)...);
    }

    void destroy(T* obj) {
        obj->~T();
        FixedSizeAllocator::deallocate(obj);
    }
};
