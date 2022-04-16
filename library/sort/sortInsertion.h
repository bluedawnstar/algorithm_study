#pragma once

template <typename T>
struct InsertionSort {
    static void sort(T data[], int size) {
        for (int i = 1; i < size; i++) {
            T tmp = data[i];

            int j;
            for (j = i - 1; j >= 0 && data[j] > tmp; j--)
                data[j + 1] = data[j];

            data[j + 1] = tmp;
        }
    }

    static void sort(vector<T>& data) {
        sort(data.data(), int(data.size()));
    }
};
