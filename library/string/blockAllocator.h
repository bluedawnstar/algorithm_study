#pragma once

template <typename T, int ChunkSizeBit>
struct BlockAllocator {
    enum { ChunkSize = 1 << ChunkSizeBit };

    struct Chunk {
        int n;
        T values[ChunkSize];

        Chunk() {
            n = 0;
        }
    };
    vector<shared_ptr<Chunk>> chunks;

    BlockAllocator() {
        chunks.push_back(shared_ptr<Chunk>(new Chunk()));
    }

    ~BlockAllocator() {
        // no action
    }

    T* get(int index) {
        return &chunks[index >> ChunkSizeBit].values[index & (ChunkSize - 1)];
    }

    T& operator [](int index) {
        return chunks[index >> ChunkSizeBit].values[index & (ChunkSize - 1)];
    }

    T* alloc() {
        if (chunks.back()->n >= ChunkSize)
            chunks.push_back(shared_ptr<Chunk>(new Chunk()));
        return &chunks.back()->values[chunks.back()->n++];
    }

    int allocAsIndex() {
        if (chunks.back()->n >= ChunkSize)
            chunks.push_back(shared_ptr<Chunk>(new Chunk()));
        return ((int(chunks.size()) - 1) << ChunkSizeBit) | chunks.back()->n++;
    }

    void clear() {
        vector<shared_ptr<Chunk>>().swap(chunks);
    }
};
