#pragma once

template <typename T = unsigned int>
struct PackedArray {
    static const int BIT_SIZE = sizeof(T) * 8;
    static const int INDEX_MASK = (sizeof(T) == 4) ? 0x1F : 0x3F;
    static const int INDEX_SHIFT = (sizeof(T) == 4) ? 5 : 6;

    vector<T> data;

    PackedArray() {
    }

    template <typename SizeT>
    explicit PackedArray(SizeT bits) {
        resize(bits);
    }

    template <typename SizeT>
    void resize(SizeT bits) {
        data.resize((bits + BIT_SIZE - 1) / BIT_SIZE + 1);
    }

    template <typename OffsetT>
    T get(OffsetT bitOffset, int size) const {
        const T* block = &data[bitOffset >> INDEX_SHIFT];
        int bitPos = bitOffset & INDEX_MASK;

        if (bitPos + size <= BIT_SIZE)
            return (block[0] >> bitPos) & getMask(size);
        else
            return ((block[0] >> bitPos) | (block[1] << (BIT_SIZE - bitPos))) & getMask(size);
    }

    template <typename OffsetT>
    void set(OffsetT bitOffset, int size, T x) {
        T* block = &data[bitOffset >> INDEX_SHIFT];
        int bitPos = bitOffset & INDEX_MASK;

        if (bitPos + size <= BIT_SIZE) {
            block[0] = (block[0] & ~(getMask(size) << bitPos)) | (x << bitPos);
        } else {
            block[0] = (block[0] & getMask(bitPos)) | (x << bitPos);
            block[1] = (block[1] & ~getMask(bitPos + size - BIT_SIZE)) | (x >> (BIT_SIZE - bitPos));
        }
    }

    static T getMask(int size) {
        return (T(2) << (size - 1)) - 1;
    }
};
