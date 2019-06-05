#pragma once

template <typename T>
struct MaxHeap {
    // O(N)
    static void heapify(T data[], int size) {
        for (int i = (size - 2) / 2; i >= 0; i--)
            siftDown(data, i, size);
    }

    static void heapify(vector<T>& data) {
        heapy(data.data(), int(data.size()));
    }

    // O(NlogN)
    static void sort(T data[], int size) {
        // heapify
        for (int i = (size - 2) / 2; i >= 0; i--)
            siftDown(data, i, size);

        // sorting
        for (int i = size - 1; i > 0; i--) {
            swap(data[0], data[i]);
            siftDown(data, 0, i);
        }
    }

    static void sort(vector<T>& data) {
        return sort(data.data(), int(data.size()));
    }

private:
    // O(logN)
    static void siftDown(T data[], int start, int end) {
        int i = start;
        int j = start * 2 + 1;

        while (j < end) {
            if (j + 1 < end && data[j] < data[j + 1])
                j++;

            if (!(data[i] < data[j]))
                break;

            swap(data[i], data[j]);

            i = j;
            j = j * 2 + 1;
        }
    }
};
