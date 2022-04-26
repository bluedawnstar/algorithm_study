#pragma once

template <typename T>
struct MergeSortEnhanced {
    static void sort(T data[], int size) {
        if (size <= 1)
            return;

        unique_ptr<T> aux(new T[size]);
        recSort(data, 0, size - 1, aux.get());
    }

    static void sort(vector<T>& data) {
        sort(data.data(), int(data.size()));
    }

private:
    static void merge(T data[], int lo, int mid, int hi, T aux[]) {
        if (data[mid] <= data[mid + 1])             // enhanced point #1
            return;

        for (int i = lo, j = 0; i <= mid; i++, j++)
            aux[j] = data[i];

        int i = mid + 1, j = 0, k = lo;
        while (i <= hi && j <= mid - lo) {
            if (aux[j] <= data[i])
                data[k++] = aux[j++];
            else
                data[k++] = data[i++];
        }

        while (j <= mid - lo)
            data[k++] = aux[j++];
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

    static void recSort(T data[], int lo, int hi, T aux[]) {
        if (lo >= hi)
            return;
        if (hi - lo < 128) {
            insertionSort(data + lo, hi - lo + 1);  // enhanced point #2
        } else {
            int mid = (lo + hi) / 2;
            recSort(data, lo, mid, aux);
            recSort(data, mid + 1, hi, aux);
            merge(data, lo, mid, hi, aux);
        }
    }
};
