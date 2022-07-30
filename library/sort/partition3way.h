#pragma once


//--- Dijkstra 3-way partitioning
template <typename T>
inline pair<int, int> partitionDijkstra3way(T data[], int lo, int hi) {
    T pivot = data[(lo + hi) / 2];

    int lt = lo;
    int gt = hi;
    for (int i = lo; i <= gt; ) {
        if (data[i] < pivot)
            swap(data[lt++], data[i++]);
        else if (data[i] > pivot)
            swap(data[i], data[gt--]);
        else
            i++;
    }

    return make_pair(lt - 1, gt + 1);
}


template <typename T>
inline int choosePivot3way(const T data[], int lo, int hi) {
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

template <typename T>
inline pair<int, int> partitionDijkstra3wayWithMED3(T data[], int lo, int hi) {
    T pivot = data[choosePivot3way(data, lo, hi)];

    int lt = lo;
    int gt = hi;
    for (int i = lo; i <= gt; ) {
        if (data[i] < pivot)
            swap(data[lt++], data[i++]);
        else if (data[i] > pivot)
            swap(data[i], data[gt--]);
        else
            i++;
    }

    return make_pair(lt - 1, gt + 1);
}


//--- Bentley-McIlroy 3-way partitioning

template <typename T>
inline pair<int, int> partitionBentleyMcIlroy3way(T data[], int lo, int hi) {
    T pivot = data[(lo + hi) / 2];

    int i = lo - 1, j = hi + 1;
    int p = lo - 1, q = hi + 1;
    while (true) {
        while (data[++i] < pivot)
            ;
        while (data[--j] > pivot)
            ;

        if (i < j) {
            swap(data[i], data[j]);
            if (data[i] == pivot)
                swap(data[++p], data[i]);
            if (data[j] == pivot)
                swap(data[--q], data[j]);
        } else {
            if (i == j)
                swap(data[++p], data[i]);
            break;
        }
    }

    i = j + 1;
    for (int k = lo; k <= p; k++)
        swap(data[k], data[j--]);
    for (int k = hi; k >= q; k--)
        swap(data[k], data[i++]);

    return make_pair(j, i);
}



//--- Dual-pivot paritioning

template <typename T>
inline pair<int, int> partitionDualPivot3way(T data[], int lo, int hi) {
    swap(data[lo], data[lo + ((hi - lo) >> 2)]);
    swap(data[hi], data[hi - ((hi - lo) >> 2)]);

    if (data[lo] > data[hi])
        swap(data[lo], data[hi]);

    int lt = lo + 1;
    int gt = hi - 1;
    for (int i = lo + 1; i <= gt; ) {
        if (data[i] < data[lo])
            swap(data[lt++], data[i++]);
        else if (data[i] > data[hi])
            swap(data[i], data[gt--]);
        else
            i++;
    }
    swap(data[lo], data[--lt]);
    swap(data[hi], data[++gt]);

    // [lo, lt - 1], [lt + 1, gt - 1], [gt + 1, hi]
    return make_pair(lt, gt);
}
