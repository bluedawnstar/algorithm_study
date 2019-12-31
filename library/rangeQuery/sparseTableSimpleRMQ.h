#pragma once

#ifndef __GNUC__
#include <intrin.h>
#endif
#include <immintrin.h>

template<class T>
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
            auto& prev = values[i - 1];
            auto& curr = values[i];
            for (int j = 0; j < N - d; j++)
                //curr[j] = min(prev[j], prev[min(N - 1, j + d)]);
                curr[j] = min(prev[j], prev[j + d]);
        }
    }

    // O(1), inclusive
    // PRECONDITION: L <= R
    T query(int L, int R) const {
        //assert(L <= R);
        R++;
#ifndef __GNUC__
        int depth = 31 - _lzcnt_u32((unsigned int)(R - L));
#else
        int depth = 31 - __builtin_clz((unsigned int)(R - L));
#endif
        return min(values[depth][L], values[depth][R - (1 << depth)]);
    }
};
