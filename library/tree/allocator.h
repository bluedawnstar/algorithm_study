#pragma once

template <typename T, size_t ChunkSizeN>
struct Allocator {
    struct ChunkT {
        int n;
        T values[ChunkSizeN];

        ChunkT() {
            n = 0;
        }
    };
    stack<shared_ptr<ChunkT>> chunks;

    Allocator() {
        chunks.push(shared_ptr<ChunkT>(new ChunkT()));
    }

    ~Allocator() {
        // no action
    }

    T* alloc() {
        if (chunks.top()->n >= ChunkSizeN)
            chunks.push(shared_ptr<ChunkT>(new ChunkT()));
        return &chunks.top()->values[chunks.top()->n++];
    }

    void clear() {
        stack<shared_ptr<ChunkT>>().swap(chunks);
    }
};
