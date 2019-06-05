#pragma once

template <typename T>
struct QuickSort {
    static void sort(T data[], int size) {
        sortSub(data, 0, size - 1);
    }

    static void sort(vector<T>& data) {
        sortSub(data.data(), 0, int(data.size()) - 1);
    }


    static void sort2(T data[], int size) {
        sortSub2(data, 0, size - 1);
    }

    static void sort2(vector<T>& data) {
        sortSub2(data.data(), 0, int(data.size()) - 1);
    }

private:
    static void sortSub(T data[], int left, int right) {
        T pivot = data[(left + right) / 2];

        int i = left;
        int j = right;
        while (i <= j) {
            while (data[i] < pivot)
                i++;
            while (pivot < data[j])
                j--;

            if (i < j)
                swap(data[i++], data[j--]);
            else
                i++;
        }

        if (left < j)
            sortSub(data, left, j);
        if (j + 1 < right)
            sortSub(data, j + 1, right);
    }

    static void sortSub2(int* data, int left, int right) {
        T pivot = data[(left + right) / 2];

        int i = left - 1;
        int j = right + 1;
        while (true) {
            do i++; while (data[i] < pivot);
            do j--; while (pivot < data[j]);

            if (i < j)
                swap(data[i], data[j]);
            else
                break;
        }

        if (left < j)
            sortSub2(data, left, j);
        if (j + 1 < right)
            sortSub2(data, j + 1, right);
    }
};