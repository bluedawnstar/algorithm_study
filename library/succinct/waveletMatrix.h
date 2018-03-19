#pragma once
#pragma once

#include "bitVectorRank.h"

template <typename T>
struct WaveletMatrix {
    static const T NaN = numeric_limits<T>::min();

    int                     N;
    int                     H;
    T                       maxVal;
    vector<BitVectorRank>   values;     // MSB bit first
    vector<int>             mids;          

    WaveletMatrix() {
    }

    explicit WaveletMatrix(const vector<T>& in) {
        build(in);
    }

    void build(const vector<T>& in) {
        build(&in[0], (int)in.size(), (in.empty()) ? 0 : *max_element(in.begin(), in.end()));
    }

    void build(const vector<T>& in, int maxVal) {
        build(&in[0], (int)in.size(), maxVal);
    }

    void build(const T* first, int n) {
        build(first, n, (n == 0) ? 0 : *max_element(first, first + n));
    }

    void build(const T* first, int n, int maxVal) {
        this->N = n;
        this->maxVal = maxVal;

        H = 1;
        while (maxVal >= (T(1) << H))
            ++H;

        values = vector<BitVectorRank>(H, BitVectorRank(N));
        mids = vector<int>(H);

        vector<T> cur(first, first + N);
        vector<T> next(N);
        for (int i = 0; i < H; i++) {
            T mask = T(1) << (H - i - 1);

            int zeroN = 0;
            for (int j = 0; j < N; j++)
                zeroN += ((cur[j] & mask) == 0);
            mids[i] = zeroN;

            BitVectorRank &bv = values[i];
            int zeroPos = 0, onePos = zeroN;
            for (int j = 0; j < N; j++) {
                if (cur[j] & mask) {
                    next[onePos++] = cur[j];
                    bv.set(j);
                } else {
                    next[zeroPos++] = cur[j];
                }
            }
            bv.buildRank();
            next.swap(cur);
        }
    }


    int size() const {
        return N;
    }

    // (0 <= pos < N)
    T get(int pos) const {
        T val = 0;
        for (int i = 0; i < H; i++) {
            const BitVectorRank &bv = values[i];

            if (bv.get(pos)) {
                val = (val << 1) | 1;
                pos = mids[i] + bv.rank1(pos - 1);
            } else {
                val = val << 1;
                pos = bv.rank0(pos - 1);
            }
        }
        return val;
    }

    // inclusive (0 <= left <= right < N, 0 <= k)
    T kth(int left, int right, int k) const {
        if (k < 0 || right - left < k) {
            return NaN;
        }

        T val = 0;
        for (int i = 0; i < H; i++) {
            const BitVectorRank &bv = values[i];

            int count = bv.rank0(left, right);
            if (k >= count) {
                val = (val << 1) | 1;
                left = mids[i] + bv.rank1(left - 1);
                right = mids[i] + bv.rank1(right) - 1;
                k -= count;
            } else {
                val = val << 1;
                left = bv.rank0(left - 1);
                right = bv.rank0(right) - 1;
            }
        }
        return val;
    }

    // inclusive (0 <= left <= right < N)
    int countLessThanOrEqual(int left, int right, T val) const {
        auto t = countEx(left, right, val);
        return ::get<0>(t) + ::get<1>(t);
    }

    // inclusive (0 <= left <= right < N)
    int count(int left, int right, T val) const {
        return ::get<0>(countEx(left, right, val));
    }

    // inclusive (0 <= left <= right < N)
    int count(int left, int right, T valLow, T valHigh) const {
        auto tL = countEx(left, right, valLow);
        auto tH = countEx(left, right, valHigh);
        return ::get<0>(tH) + ::get<1>(tH) - ::get<1>(tL);
    }

    // return (the number of val, numbers less than val, numbers greater than val)
    // inclusive (0 <= left <= right < N)
    tuple<int, int, int> countEx(int left, int right, T val) const {
        if (val > maxVal) {
            return make_tuple(0, right - left + 1, 0);
        }

        int lt = 0, gt = 0;
        for (int i = 0; i < H; i++) {
            const BitVectorRank &bv = values[i];

            if ((val >> (H - i - 1)) & 1) {
                int leftN = bv.rank1(left - 1);
                int rightN = bv.rank1(right);

                lt += (right - left + 1) - (rightN - leftN);
                left = mids[i] + leftN;
                right = mids[i] + rightN - 1;
            } else {
                int leftN = bv.rank0(left - 1);
                int rightN = bv.rank0(right);

                gt += (right - left + 1) - (rightN - leftN);
                left = leftN;
                right = rightN - 1;
            }
        }
        return make_tuple(right - left + 1, lt, gt);
    }
};
