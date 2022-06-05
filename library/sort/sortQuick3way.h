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


    static void sort2(T data[], int size) {
        sortSub2(data, 0, size - 1);
    }

    static void sort2(vector<T>& data) {
        sortSub2(data.data(), 0, int(data.size()) - 1);
    }

private:
    static void sortSub(T data[], int left, int right) {
        if (left >= right)
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
        for (int k = left; k < p; k++)
            swap(data[k], data[j--]);
        for (int k = right - 1; k > q; k--)
            swap(data[k], data[i++]);
        sortSub(data, left, j);
        sortSub(data, i, right);
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

    static void sortSub2(T data[], int left, int right) {
        if (left >= right)
            return;

        int pivot = choosePivot(data, left, right);
        if (pivot != right)
            swap(data[pivot], data[right]);

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
        for (int k = left; k < p; k++)
            swap(data[k], data[j--]);
        for (int k = right - 1; k > q; k--)
            swap(data[k], data[i++]);
        sortSub2(data, left, j);
        sortSub2(data, i, right);
    }
};
