#pragma once

// 3-way Quicksort
//   "Quicksort with 3-way partitioning is optimal for random keys with duplicates." - Sedgewick-Bentley

template <typename T>
struct QuickSort3way {
    static void sort(T data[], int size) {
        sortSub(data, 0, size - 1);
    }

    static void sort(vector<T>& data) {
        sortSub(data.data(), 0, int(data.size()) - 1);
    }

private:
    static void sortSub(T data[], int left, int right) {
        if (right <= left)
            return;
        int i = left - 1, j = right;
        int p = left - 1, q = right;
        while(true) {
            while (data[++i] < data[right])
                ;
            while (data[right] < data[--j]) {
                if (j == left)
                    break;
            }
            if (i >= j)
                break;
            swap(data[i], data[j]);
            if (data[i] == data[right])
                swap(data[++p], data[i]);
            if (data[j] == data[right])
                swap(data[--q], data[j]);
        }
        swap(data[i], data[right]);
        j = i - 1;
        i = i + 1;
        for (int k = left ; k <= p; k++)
            swap(data[k], data[j--]);
        for (int k = right - 1; k >= q; k--)
            swap(data[k], data[i++]);
        sortSub(data, left, j);
        sortSub(data, i, right);
    }
};
