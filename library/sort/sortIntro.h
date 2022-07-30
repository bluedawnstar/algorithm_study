#pragma once

/*
    Introsort, 1997
*/
template <typename T>
struct IntroSort {
    function<pair<int, int>(T[], int, int)> partition;

    IntroSort(const function<pair<int, int>(T[], int, int)>& partition) : partition(partition) {
    }

    void sort(T data[], int size) {
        if (size <= 1)
            return;

        int maxDepth = 0;
        for (int i = size; i > 1; i >>= 1)
            maxDepth++;

        sortSub(data, 0, size - 1, maxDepth * 2);
    }

    void sort(vector<T>& data) {
        sort(data.data(), int(data.size()));
    }

private:
    void sortSub(T data[], int lo, int hi, int maxDepth) {
        if (hi - lo <= 16) {
            insertionSort(&data[lo], hi - lo + 1);
            return;
        } else if (maxDepth <= 0) {
            heapSort(&data[lo], hi - lo + 1);
            return;
        }

        int leftLast, rightFirst;
        tie(leftLast, rightFirst) = partition(data, lo, hi);

        sortSub(data, lo, leftLast, maxDepth - 1);
        sortSub(data, rightFirst, hi, maxDepth - 1);
    }

    //--- insertion sort
    static void insertionSort(T data[], int size) {
        for (int i = 1; i < size; i++) {
            T tmp = data[i];

            int j;
            for (j = i - 1; j >= 0 && data[j] > tmp; j--)
                data[j + 1] = data[j];

            data[j + 1] = tmp;
        }
    }

    //--- heap sort
    static void heapSort(T data[], int size) {
        // heapify
        for (int i = (size - 2) / 2; i >= 0; i--)
            siftDown(data, i, size);

        // sorting
        for (int i = size - 1; i > 0; i--) {
            swap(data[0], data[i]);
            siftDown(data, 0, i);
        }
    }

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

template <typename T, typename Fn>
inline void introSort(T data[], int n, Fn& partition) {
    IntroSort<T> is(partition);
    is.sort(data, n);
}

template <typename T, typename Fn>
inline void introSort(vector<T>& data, Fn& partition) {
    IntroSort<T> is(partition);
    is.sort(data);
}
