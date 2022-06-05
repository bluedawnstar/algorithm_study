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


    static void sort3(T data[], int size) {
        sortSub3(data, 0, size - 1);
    }

    static void sort3(vector<T>& data) {
        sortSub3(data.data(), 0, int(data.size()) - 1);
    }

private:
    // modified Hoare
    static void sortSub(T data[], int left, int right) {
        if (left >= right)
            return;

        T pivot = data[(left + right) / 2];

        int i = left;
        int j = right;
        while (i <= j) {
            while (data[i] < pivot)
                i++;
            while (data[j] > pivot)
                j--;

            if (i < j)
                swap(data[i++], data[j--]);
            else
                i++;
        }

        sortSub(data, left, j);
        sortSub(data, j + 1, right);
    }

    // Hoare
    static void sortSub2(T data[], int left, int right) {
        if (left >= right)
            return;

        T pivot = data[(left + right) / 2];

        int i = left - 1;
        int j = right + 1;
        while (true) {
            do i++; while (data[i] < pivot);
            do j--; while (data[j] > pivot);

            if (i < j)
                swap(data[i], data[j]);
            else
                break;
        }

        sortSub2(data, left, j);
        sortSub2(data, j + 1, right);
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

    // Hore with MED3
    static void sortSub3(T data[], int left, int right) {
        if (left >= right)
            return;

        T pivot = data[choosePivot(data, left, right)];

        int i = left - 1;
        int j = right + 1;
        while (true) {
            do i++; while (data[i] < pivot);
            do j--; while (data[j] > pivot);

            if (i < j)
                swap(data[i], data[j]);
            else
                break;
        }

        sortSub3(data, left, j);
        sortSub3(data, j + 1, right);
    }
};
