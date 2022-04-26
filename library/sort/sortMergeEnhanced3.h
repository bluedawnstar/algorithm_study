#pragma once

template <typename T>
struct MergeSortEnhanced3 {
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
    static void mergeL2R(T to[], const T from[], int lo, int mid, int hi) {
        int i = mid + 1, j = lo, k = lo;
        while (i <= hi && j <= mid) {
            if (from[j] <= to[i])
                to[k++] = from[j++];
            else
                to[k++] = to[i++];
        }

        while (j <= mid)
            to[k++] = from[j++];
    }

    static void mergeR2L(T to[], const T from[], int lo, int mid, int hi) {
        int i = mid, j = hi, k = hi;
        while (i >= lo && j > mid) {
            if (from[j] >= to[i])
                to[k--] = from[j--];
            else
                to[k--] = to[i--];
        }

        while (j > mid)
            to[k--] = from[j--];
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
            recSort(data, lo, mid, aux, true);
            recSort(data, mid + 1, hi, aux, false);
            if (toAux)
                mergeR2L(aux, data, lo, mid, hi);
            else
                mergeL2R(data, aux, lo, mid, hi);
        }
    }
};
