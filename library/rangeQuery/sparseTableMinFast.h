#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

//--------- RMQ (Range Minimum Query) - Min Sparse Table ----------------------

struct FastSparseTableMin {
    int N;
    int H;
    vector<vector<int>> value;

    FastSparseTableMin() {
    }

    FastSparseTableMin(const int a[], int n) {
        build(a, n);
    }

    explicit FastSparseTableMin(const vector<int>& a) {
        build(a);
    }


    void build(const int a[], int n) {
        this->N = n;

#ifndef __GNUC__
        H = 32 - _lzcnt_u32((N << 1) - 1);
#else
        H = 32 - __builtin_clz((N << 1) - 1);
#endif

        value.assign(H, vector<int>(n));
        for (int i = 0; i < n; i++)
            value[0][i] = a[i];

        for (int i = 1, step = 1; i < H; i++, step <<= 1) {
            for (int j = 0; j < n; j++) {
                if (j + step < n)
                    value[i][j] = min(value[i - 1][j], value[i - 1][j + step]);
                else
                    value[i][j] = value[i - 1][j];
            }
        }
    }

    void build(const vector<int>& a) {
        build(&a[0], int(a.size()));
    }


    // inclusive
    int query(int left, int right) const {
        right++;
        if (right <= left)
            return INT_MAX;

#ifndef __GNUC__
        int level = 31 - _lzcnt_u32(static_cast<unsigned int>(right - left));
#else
        int level = 31 - __builtin_clz(static_cast<unsigned int>(right - left));
#endif
        return min(value[level][left], value[level][right - (1 << level)]);
    }
};
