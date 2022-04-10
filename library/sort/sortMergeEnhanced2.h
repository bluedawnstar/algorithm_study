#pragma once

template <typename T>
struct MergeSortEnhanced2 {
    static void sort(T data[], int size) {
        if (size <= 1)
            return;

        unique_ptr<T> aux(new T[size]);
        recSort(data, 0, size - 1, aux.get(), false);
    }

    static void sort(vector<T>& data) {
        sort(data.data(), int(data.size()));
    }

private:
    static void merge(T to[], const T from[], int lo, int mid, int hi) {
        int i = lo, j = mid + 1, k = lo;
        while (i <= mid && j <= hi) {
            if (from[i] <= from[j])
                to[k++] = from[i++];
            else
                to[k++] = from[j++];
        }

        while (i <= mid)
            to[k++] = from[i++];
        while (j <= hi)
            to[k++] = from[j++];
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

    static void insertionSort(T out[], const T in[], int size) {
        out[0] = in[0];
        for (int i = 1; i < size; i++) {
            int tmp = in[i];

            int j;
            for (j = i - 1; j >= 0 && out[j] > tmp; j--)
                out[j + 1] = out[j];
            out[j + 1] = tmp;
        }
    }

    static void recSort(T data[], int lo, int hi, T aux[], bool toAux) {
        if (lo >= hi)
            return;
        if (hi - lo < 128) {
            if (toAux)
                insertionSort(aux + lo, data + lo, hi - lo + 1);
            else
                insertionSort(data + lo, hi - lo + 1);
        } else {
            int mid = (lo + hi) / 2;
            recSort(data, lo, mid, aux, !toAux);
            recSort(data, mid + 1, hi, aux, !toAux);
            if (toAux)
                merge(aux, data, lo, mid, hi);
            else
                merge(data, aux, lo, mid, hi);
        }
    }
};
