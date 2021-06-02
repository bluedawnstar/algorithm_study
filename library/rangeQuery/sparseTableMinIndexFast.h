#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

//--------- RMQ (Range Minimum Query) - Min Sparse Table ----------------------
// handing indexes instread of real values
struct FastSparseTableMinIndex {
    int N;
    int H;
    vector<vector<int>> value;

    vector<int> in;

    FastSparseTableMinIndex() {
    }

    FastSparseTableMinIndex(const int a[], int n) {
        build(a, n);
    }

    explicit FastSparseTableMinIndex(const vector<int>& a) {
        build(a);
    }


    void build(const int a[], int n) {
        this->N = n;

#ifndef __GNUC__
        H = 32 - _lzcnt_u32((N << 1) - 1);
#else
        H = 32 - __builtin_clz((N << 1) - 1);
#endif

        in.resize(n);
        for (int i = 0; i < n; i++)
            in[i] = a[i];

        value.assign(H, vector<int>(n));
        for (int i = 0; i < n; i++)
            value[0][i] = i;

        for (int i = 1; i < H; i++) {
            vector<int>& prev = value[i - 1];
            vector<int>& curr = value[i];
            for (int j = 0; j < n; j++) {
                int j2 = j + (1 << (i - 1));
                if (j2 < n)
                    curr[j] = (in[prev[j]] < in[prev[j2]]) ? prev[j] : prev[j2];
                else
                    curr[j] = prev[j];
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
            return left;    //invalid range

#ifndef __GNUC__
        int level = 31 - _lzcnt_u32(static_cast<unsigned int>(right - left));
#else
        int level = 31 - __builtin_clz(static_cast<unsigned int>(right - left));
#endif
        if (in[value[level][left]] < in[value[level][right - (1 << level)]])
            return value[level][left];
        else
            return value[level][right - (1 << level)];
    }
};
