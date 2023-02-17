#pragma once

// max heap
template <typename T>
struct MaxHeap {
    vector<T> heap;

    MaxHeap() : heap(1) {
    }

    MaxHeap(const T a[], int size) {
        heapify(a, size);
    }

    size_t size() const {
        return heap.size() - 1;
    }

    bool empty() const {
        return bool(heap.size() <= 1);
    }

    void clear() {
        heap.resize(1);
    }

    //--- priority queue

    void push(T x) {
        int index = int(heap.size());
        heap.emplace_back(x);
        siftUp(index);
    }

    const T& top() const {
        return heap[1];
    }

    bool pop() {
        if (empty())
            return false;
        swap(heap[1], heap.back());
        heap.pop_back();
        siftDown(1);
        return true;
    }


    void push(int maxSize, T x) {
        if (size() < maxSize) {
            push(x);
        } else if (x < heap[1]) {
            heap[1] = x;
            siftDown(1);
        }
    }

    //--- heap

    // O(logN)
    void siftUp(int index) {
        for (int i = index; (i > 1) && (heap[i / 2] < heap[i]); i /= 2)
            swap(heap[i / 2], heap[i]);
    }

    // O(logN)
    void siftDown(int index) {
        int N = int(heap.size());
        for (int i = index, j = index * 2; j < N; i = j, j *= 2) {
            if (j + 1 < N && heap[j] < heap[j + 1])
                j++;

            if (heap[i] >= heap[j])
                break;

            swap(heap[i], heap[j]);
        }
    }

    // O(N)
    void heapify(const T a[], int size) {
        heap.resize(1);
        heap.insert(heap.end(), a, a + size);

        // heapify
        for (int i = size / 2; i >= 1; i--)
            siftDown(i);
    }
};
