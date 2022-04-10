#pragma once

template <typename T>
struct ShellSort {
    static void sort(T data[], int size) {
        int h = 1;
        while (h < size / 3)
            h = h * 3 + 1;      // 1, 4, 13, 40, 121, 364, 1093, ...

        while (h >= 1) {
            for (int i = h; i < size; i++) {
                int tmp = data[i];

                int j;
                for (j = i - h; j >= 0 && data[j] > tmp; j -= h)
                    data[j + h] = data[j];

                data[j + h] = tmp;
            }

            h = h / 3;
        }
    }

    static void sort(vector<T>& data) {
        sort(data.data(), int(data.size()));
    }
};
