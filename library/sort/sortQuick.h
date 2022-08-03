#pragma once

#include <functional>

template <typename T>
struct QuickSort {
    function<pair<int, int>(T[], int, int)> partition;

    QuickSort(const function<pair<int, int>(T[], int, int)>& partition) : partition(partition) {
    }

    void sort(T data[], int n) {
        sortRec(data, 0, n - 1);
    }

    void sort(vector<T>& data) {
        sortRec(data.data(), 0, int(data.size()) - 1);
    }

    //---

    void sort2(T data[], int n) {
        sortRec2(data, 0, n - 1);
    }

    void sort2(vector<T>& data) {
        sortRec2(data.data(), 0, int(data.size()) - 1);
    }

    //---

    // quick select
    void select(T data[], int n, int nth) {
        if (nth < 0 || nth >= n)
            return;

        int lo = 0, hi = n - 1;
        while (lo < hi) {
            int leftLast, rightFirst;
            tie(leftLast, rightFirst) = partition(data, lo, hi);

            if (nth <= leftLast)
                hi = leftLast;
            else if (nth >= rightFirst)
                lo = rightFirst;
            else
                break;
        }
    }

    void select(vector<T>& data, int nth) {
        select(data.data(), int(data.size()), nth);
    }

private:
    void sortRec(T data[], int lo, int hi) {
        if (lo >= hi)
            return;

        int leftLast, rightFirst;
        tie(leftLast, rightFirst) = partition(data, lo, hi);

        sortRec(data, lo, leftLast);
        sortRec(data, rightFirst, hi);
    }

    //---

    void sortRec2(T data[], int lo, int hi) {
        if (hi - lo < 16) {
            insertionSort(data + lo, hi - lo + 1);
            return;
        }

        int leftLast, rightFirst;
        tie(leftLast, rightFirst) = partition(data, lo, hi);

        sortRec2(data, lo, leftLast);
        sortRec2(data, rightFirst, hi);
    }

    static void insertionSort(T data[], int size) {
        for (int i = 1; i < size; i++) {
            int tmp = data[i];

            int j;
            for (j = i - 1; j >= 0 && data[j] > tmp; j--)
                data[j + 1] = data[j];

            data[j + 1] = tmp;
        }
    }
};

template <typename T, typename Fn>
inline void quickSort(T data[], int n, Fn& partition) {
    QuickSort<T> qs(partition);
    qs.sort(data, n);
}

template <typename T, typename Fn>
inline void quickSort(vector<T>& data, Fn& partition) {
    QuickSort<T> qs(partition);
    qs.sort(data);
}


template <typename T, typename Fn>
inline void quickSort2(T data[], int n, Fn& partition) {
    QuickSort<T> qs(partition);
    qs.sort2(data, n);
}

template <typename T, typename Fn>
inline void quickSort2(vector<T>& data, Fn& partition) {
    QuickSort<T> qs(partition);
    qs.sort2(data);
}


template <typename T, typename Fn>
inline void quickSelect(T data[], int n, int nth, Fn& partition) {
    QuickSort<T> qs(partition);
    qs.select(data, n, nth);
}

template <typename T, typename Fn>
inline void quickSelect(vector<T>& data, int nth, Fn& partition) {
    QuickSort<T> qs(partition);
    qs.select(data, nth);
}
