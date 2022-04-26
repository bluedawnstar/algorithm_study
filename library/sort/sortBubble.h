#pragma once

template <typename T>
struct BubbleSort {
    static void sort(T data[], int size) {
        for (int i = size - 1; i > 0; i--) {
            for (int j = 0; j < i; j++) {
                if (data[j] > data[j + 1])
                    swap(data[j], data[j + 1]);
            }
        }
    }

    static void sort2(T data[], int size) {
        for (int i = size - 1; i > 0; i--) {
            bool swapped = false;
            for (int j = 0; j < i; j++) {
                if (data[j] > data[j + 1]) {
                    swap(data[j], data[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped)
                break;
        }
    }

    static void sort(vector<T>& data) {
        sort(data.data(), int(data.size()));
    }

    static void sort2(vector<T>& data) {
        sort2(data.data(), int(data.size()));
    }
};
