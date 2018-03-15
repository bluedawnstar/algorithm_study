#pragma once
#pragma once

#include "bitVectorRank.h"

// <WaveletMatrix vs WaveletMatrixArray>
// Speed:  WaveletMatrixArray is faster 4 ~ 5 times
// Memory: WaveletMatrix is more efficient 24 ~ 30%

template <typename T>
struct WaveletMatrixArray {
    static const T NaN = numeric_limits<T>::min();

    int mN;
    int mH;
    T mMaxVal;
    vector<vector<int>> mV; // MSB bit first

    WaveletMatrixArray() {
    }

    explicit WaveletMatrixArray(const vector<T>& in) {
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

        mV = vector<vector<int>>(mH, vector<int>(mN + 1));

        vector<T> cur(first, first + N);
        vector<T> next(mN);
        for (int i = 0; i < mH; i++) {
            T mask = T(1) << (mH - i - 1);

            vector<int>& v = mV[i];

            int zeroN = 0;
            for (int j = 0; j < mN; j++) {
                v[j] = zeroN;
                zeroN += ((cur[j] & mask) == 0);
            }
            v[mN] = zeroN;

            int zeroPos = 0, onePos = zeroN;
            for (int j = 0; j < mN; j++) {
                if (cur[j] & mask) {
                    next[onePos++] = cur[j];
                } else {
                    next[zeroPos++] = cur[j];
                }
            }
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
            const vector<int>& v = mV[i];

            if (v[pos] == v[pos + 1]) {
                val = (val << 1) | 1;
                pos = v.back() + (pos - v[pos]);
            } else {
                val = val << 1;
                pos = v[pos];
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
            const vector<int>& v = mV[i];

            int count = v[right + 1] - v[left];
            if (k >= count) {
                val = (val << 1) | 1;
                left = v.back() + (left - v[left]);
                right = v.back() + (right - v[right + 1]);
                k -= count;
            } else {
                val = val << 1;
                left = v[left];
                right = v[right + 1] - 1;
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
            const vector<int>& v = mV[i];

            if ((val >> (mH - i - 1)) & 1) {
                int leftN = left - v[left];
                int rightN = right + 1 - v[right + 1];

                lt += (right - left + 1) - (rightN - leftN);
                left = v.back() + leftN;
                right = v.back() + rightN - 1;
            } else {
                int leftN = v[left];
                int rightN = v[right + 1];

                gt += (right - left + 1) - (rightN - leftN);
                left = leftN;
                right = rightN - 1;
            }
        }
        return make_tuple(right - left + 1, lt, gt);
    }
};
