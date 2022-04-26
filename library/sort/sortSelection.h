#pragma once

template <typename T>
struct SelectionSort {
    static void sort(T data[], int size) {
        for (int i = 0; i + 1 < size; i++) {
            int minIndex = i;
            for (int j = i + 1; j < size; j++) {
                if (data[j] < data[minIndex])
                    minIndex = j;
            }
            if (minIndex != i)
                swap(data[i], data[minIndex]);
        }
    }

    static void sort(vector<T>& data) {
        sort(data.data(), int(data.size()));
    }
};
