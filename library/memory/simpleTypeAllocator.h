#pragma once

#include <stack>
#include <memory>

// no free
template <typename T>
struct SimpleTypeAllocator {
    struct ChunkT {
        int n;
        std::unique_ptr<unsigned char> values;

        explicit ChunkT(size_t size) : n(0), values(new unsigned char[sizeof(T) * size]) {
        }
    };

    int chunkSize;
    std::stack<std::unique_ptr<ChunkT>> chunks;

    explicit SimpleTypeAllocator(int chunkSize) : chunkSize(chunkSize) {
    }

    ~SimpleTypeAllocator() {
    }

    T* allocate() {
        if (chunks.empty() || chunks.top()->n >= chunkSize)
            chunks.push(std::make_unique<ChunkT>(chunkSize));

        return reinterpret_cast<T*>(chunks.top()->values.get() + sizeof(T) * chunks.top()->n++);
    }

    template <typename... Args>
    T* construct(Args&&... args) {
        return ::new (allocate()) T(std::forward<Args>(args)...);
    }

    void destroy(T* obj) {
        obj->~T();
    }
};
