#pragma once
#pragma once

#include <functional>

template <typename T>
struct QuickSort3way {
    function<pair<int, int>(T[], int, int)> partition;

    QuickSort3way(const function<pair<int, int>(T[], int, int)>& partition) : partition(partition) {
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

private:
    void sortRec(T data[], int lo, int hi) {
        if (lo >= hi)
            return;

        int lt, gt;
        tie(lt, gt) = partition(data, lo, hi);

        sortRec(data, lo, lt - 1);
        if (data[lt] < data[gt])
            sortRec(data, lt + 1, gt - 1);
        sortRec(data, gt + 1, hi);
    }

    //---

    void sortRec2(T data[], int lo, int hi) {
        if (lo >= hi)
            return;

        if (hi - lo < 16) {
            insertionSort(data + lo, hi - lo + 1);
            return;
        }

        int lt, gt;
        tie(lt, gt) = partition(data, lo, hi);

        sortRec(data, lo, lt - 1);
        if (data[lt] < data[gt])
            sortRec(data, lt + 1, gt - 1);
        sortRec(data, gt + 1, hi);
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
inline void quickSort3way(T data[], int n, Fn& partition) {
    QuickSort3way<T> qs(partition);
    qs.sort(data, n);
}

template <typename T, typename Fn>
inline void quickSort3way(vector<T>& data, Fn& partition) {
    QuickSort3way<T> qs(partition);
    qs.sort(data);
}


template <typename T, typename Fn>
inline void quickSort3way2(T data[], int n, Fn& partition) {
    QuickSort3way<T> qs(partition);
    qs.sort2(data, n);
}

template <typename T, typename Fn>
inline void quickSort3way2(vector<T>& data, Fn& partition) {
    QuickSort3way<T> qs(partition);
    qs.sort2(data);
}
