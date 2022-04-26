#pragma once

/*
    Introsort, 1997
*/
template <typename T>
struct IntroSort {
    static void sort(T data[], int size) {
        if (size <= 1)
            return;

        int maxDepth = 0;
        for (int i = size; i > 1; i >>= 1)
            maxDepth++;

        sortSub(data, 0, size - 1, maxDepth * 2);
    }

    static void sort2(T data[], int size) {
        if (size <= 1)
            return;

        int maxDepth = 0;
        for (int i = size; i > 1; i >>= 1)
            maxDepth++;

        sortSub2(data, 0, size - 1, maxDepth * 2);
    }

    static void sort3(T data[], int size) {
        if (size <= 1)
            return;

        int maxDepth = 0;
        for (int i = size; i > 1; i >>= 1)
            maxDepth++;

        sortSub3(data, 0, size - 1, maxDepth * 2);
    }


    static void sort(vector<T>& data) {
        sort(data.data(), int(data.size()));
    }

    static void sort2(vector<T>& data) {
        sort2(data.data(), int(data.size()));
    }

    static void sort3(vector<T>& data) {
        sort3(data.data(), int(data.size()));
    }

private:
    //--- partition

    static void sortSub(T data[], int left, int right, int maxDepth) {
        if (right - left <= 16) {
            insertionSort(&data[left], right - left + 1);
            return;
        } else if (maxDepth <= 0) {
            heapSort(&data[left], right - left + 1);
            return;
        }

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
            sortSub(data, left, j, maxDepth - 1);
        if (j + 1 < right)
            sortSub(data, j + 1, right, maxDepth - 1);
    }

    static void sortSub2(T data[], int left, int right, int maxDepth) {
        if (right - left <= 16) {
            insertionSort(&data[left], right - left + 1);
            return;
        } else if (maxDepth <= 0) {
            heapSort(&data[left], right - left + 1);
            return;
        }

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
            sortSub2(data, left, j, maxDepth - 1);
        if (j + 1 < right)
            sortSub2(data, j + 1, right, maxDepth - 1);
    }

    //---

    static int choosePivot(const T data[], int left, int right) {
#define KEY(idx)    data[idx]
#define MED3(a,b,c) (KEY(a) < KEY(b) ? (KEY(b) < KEY(c) ? (b) : KEY(a) < KEY(c) ? (c) : (a)) \
                                     : (KEY(b) > KEY(c) ? (b) : KEY(a) > KEY(c) ? (c) : (a)))
        int n = right - left + 1;
        int mid = (left + right) >> 1;
        if (n > 7) {
            int lo = left;
            int hi = left + n - 1;
            if (n > 40) {
                int s = n >> 3;
                lo = MED3(lo, lo + s, lo + s + s);
                mid = MED3(mid - s, mid, mid + s);
                hi = MED3(hi - s - s, hi - s, hi);
            }
            mid = MED3(lo, mid, hi);
        }
        return mid;
#undef KEY
#undef MED3
    }

    static void sortSub3(T data[], int left, int right, int maxDepth) {
        if (right - left <= 16) {
            insertionSort(&data[left], right - left + 1);
            return;
        } else if (maxDepth <= 0) {
            heapSort(&data[left], right - left + 1);
            return;
        }

        T pivot = data[choosePivot(data, left, right)];

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
            sortSub3(data, left, j, maxDepth - 1);
        if (j + 1 < right)
            sortSub3(data, j + 1, right, maxDepth - 1);
    }

    //--- insertion sort

    static void insertionSort(T data[], int size) {
        for (int i = 1; i < size; i++) {
            T tmp = data[i];

            int j;
            for (j = i - 1; j >= 0 && data[j] > tmp; j--)
                data[j + 1] = data[j];

            data[j + 1] = tmp;
        }
    }

    //--- heap sort

    static void heapSort(T data[], int size) {
        // heapify
        for (int i = (size - 2) / 2; i >= 0; i--)
            siftDown(data, i, size);

        // sorting
        for (int i = size - 1; i > 0; i--) {
            swap(data[0], data[i]);
            siftDown(data, 0, i);
        }
    }

    static void siftDown(T data[], int start, int end) {
        int i = start;
        int j = start * 2 + 1;

        while (j < end) {
            if (j + 1 < end && data[j] < data[j + 1])
                j++;

            if (!(data[i] < data[j]))
                break;

            swap(data[i], data[j]);

            i = j;
            j = j * 2 + 1;
        }
    }
};
