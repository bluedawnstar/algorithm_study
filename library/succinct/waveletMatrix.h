#pragma once
#pragma once

#include "../set/bitVectorRank.h"

template <typename T>
struct WaveletMatrix {
    static const T NaN = numeric_limits<T>::min();

    int mN;
    int mH;
    T mMaxVal;
    vector<BitVectorRank> mV;   // MSB bit first
    vector<int> mMids;          

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

    void build(const T* first, int N) {
        build(first, N, (N == 0) ? 0 : *max_element(first, first + N));
    }

    void build(const T* first, int N, int maxVal) {
        mN = N;
        mMaxVal = maxVal;

        mH = 1;
        while (mMaxVal >= (T(1) << mH))
            ++mH;

        mV = vector<BitVectorRank>(mH, BitVectorRank(mN));
        mMids = vector<int>(mH);

        vector<T> cur(first, first + N);
        vector<T> next(mN);
        for (int i = 0; i < mH; i++) {
            T mask = T(1) << (mH - i - 1);

            int zeroN = 0;
            for (int j = 0; j < mN; j++)
                zeroN += ((cur[j] & mask) == 0);
            mMids[i] = zeroN;

            BitVectorRank &bv = mV[i];
            int zeroPos = 0, onePos = zeroN;
            for (int j = 0; j < mN; j++) {
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
        return mN;
    }

    // (0 <= pos < N)
    T get(int pos) const {
        T val = 0;
        for (int i = 0; i < mH; i++) {
            const BitVectorRank &bv = mV[i];

            if (bv.get(pos)) {
                val = (val << 1) | 1;
                pos = mMids[i] + bv.rank1(pos - 1);
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
        for (int i = 0; i < mH; i++) {
            const BitVectorRank &bv = mV[i];

            int count = bv.rank0(left, right);
            if (k >= count) {
                val = (val << 1) | 1;
                left = mMids[i] + bv.rank1(left - 1);
                right = mMids[i] + bv.rank1(right) - 1;
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
        if (val > mMaxVal) {
            return make_tuple(0, right - left + 1, 0);
        }

        int lt = 0, gt = 0;
        for (int i = 0; i < mH; i++) {
            const BitVectorRank &bv = mV[i];

            if ((val >> (mH - i - 1)) & 1) {
                int leftN = bv.rank1(left - 1);
                int rightN = bv.rank1(right);

                lt += (right - left + 1) - (rightN - leftN);
                left = mMids[i] + leftN;
                right = mMids[i] + rightN - 1;
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
