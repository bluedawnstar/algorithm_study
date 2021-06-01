#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

template<typename T>
struct SimpleSparseTableRMQ {
    int N;
    int H;
    vector<vector<T>> values;

    // O(NlogN)
    SimpleSparseTableRMQ(const vector<T>& v) {
        N = int(v.size());

#ifndef __GNUC__
        H = 32 - _lzcnt_u32((N << 1) - 1);
#else
        H = 32 - __builtin_clz((N << 1) - 1);
#endif

        values.assign(H, v);
        for (int i = 1; i < H; i++) {
            int d = 1 << (i - 1);
            for (int j = 0; j < N - d; j++)
                //values[i][j] = min(values[i - 1][j], values[i - 1][min(N - 1, j + d)]);
                values[i][j] = min(values[i - 1][j], values[i - 1][j + d]);
        }
    }

    // O(1), inclusive
    // PRECONDITION: L <= R
    T query(int L, int R) const {
        //assert(L <= R);
        R++;
#ifndef __GNUC__
        int level = 31 - _lzcnt_u32(static_cast<unsigned int>(R - L));
#else
        int level = 31 - __builtin_clz(static_cast<unsigned int>(R - L));
#endif
        return min(values[level][L], values[level][R - (1 << level)]);
    }
};
