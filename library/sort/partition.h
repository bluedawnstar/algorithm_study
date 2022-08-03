#pragma once

// Hoare
template <typename T>
inline pair<int, int> partitionHoare(T data[], int lo, int hi) {
    T pivot = data[(lo + hi) / 2];

    int i = lo - 1;
    int j = hi + 1;
    while (true) {
        do i++; while (data[i] < pivot);
        do j--; while (data[j] > pivot);

        if (i < j)
            swap(data[i], data[j]);
        else
            break;
    }

    return make_pair(j, j + 1);
}

// modified Hoare
template <typename T>
inline pair<int, int> partitionHoare2(T data[], int lo, int hi) {
    T pivot = data[(lo + hi) / 2];

    int i = lo;
    int j = hi;
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

    return make_pair(j, j + 1);
}


template <typename T>
inline int choosePivot(const T data[], int lo, int hi) {
#define KEY(idx)    data[idx]
#define MED3(a,b,c) (KEY(a) < KEY(b) ? (KEY(b) < KEY(c) ? (b) : KEY(a) < KEY(c) ? (c) : (a)) \
                                     : (KEY(b) > KEY(c) ? (b) : KEY(a) > KEY(c) ? (c) : (a)))
    int n = hi - lo + 1;
    int mid = (lo + hi) >> 1;
    if (n > 7) {
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
template <typename T>
inline pair<int, int> partitionHoareWithMED3(T data[], int lo, int hi) {
    T pivot = data[choosePivot(data, lo, hi)];

    int i = lo - 1;
    int j = hi + 1;
    while (true) {
        do i++; while (data[i] < pivot);
        do j--; while (data[j] > pivot);

        if (i < j)
            swap(data[i], data[j]);
        else
            break;
    }

    return make_pair(j, j + 1);
}


//--- Lomuto

// Lomuto
template <typename T>
inline pair<int, int> partitionLomuto(T data[], int lo, int hi) {
    swap(data[(lo + hi) / 2], data[hi]);

    T pivot = data[hi];

    int i = lo;
    for (int j = lo; j < hi; j++) {
        if (data[j] <= pivot)
            swap(data[i++], data[j]);
    }
    swap(data[i], data[hi]);
    return make_pair(i - 1, i + 1);
}

template <typename T>
inline pair<int, int> partitionLomutoWithMED3(T data[], int lo, int hi) {
    swap(data[choosePivot(data, lo, hi)], data[hi]);

    T pivot = data[hi];

    int i = lo;
    for (int j = lo; j < hi; j++) {
        if (data[j] <= pivot)
            swap(data[i++], data[j]);
    }
    swap(data[i], data[hi]);
    return make_pair(i - 1, i + 1);
}
