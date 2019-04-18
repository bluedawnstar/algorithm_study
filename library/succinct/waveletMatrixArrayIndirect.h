#pragma once

#include "bitVectorRank.h"

/* <How To Use>
    1. build wavelet matrix with input values
        WaveletMatrixArrayIndirect<T> wm;

        auto idx = wm.build(values, N);

    2. build another data structures with return values of WaveletMatrixArrayIndirect<T>::build()

        vector<T> reordered(N);
        for (int i = 0; i < N; i++)
            reordered[i] = values2[idx[i]]; // reorder with return values of WaveletMatrixArrayIndirect<T>::build()
        rmq.build(reordered);

    3. query in a wavelet matrix and another data structure
        auto r = wm.findMaxXor(L, R, k);
        cout << "max xor value = " << r.first << endl;
        cout << "second query value in same max xor values = " << rmq.query(r.second.first, r.second.second) << endl;
*/
template <typename T>
struct WaveletMatrixArrayIndirect {
    static const T NaN = numeric_limits<T>::min();

    int                 N;
    int                 H;
    T                   maxVal;
    vector<vector<int>> indexes;    // MSB bit first
    vector<T>           values;

    WaveletMatrixArrayIndirect() {
    }

    vector<int> build(const vector<T>& in) {
        return build(&in[0], int(in.size()), in.empty() ? 0 : *max_element(in.begin(), in.end()));
    }

    vector<int> build(const vector<T>& in, int maxVal) {
        return build(&in[0], int(in.size()), maxVal);
    }

    vector<int> build(const T* first, int n) {
        return build(first, n, (n == 0) ? 0 : *max_element(first, first + n));
    }

    // return reordered indexes of values
    // reordered_values[i] = values[return_value[i]]
    vector<int> build(const T* first, int n, int maxVal) {
        this->N = n;
        this->maxVal = maxVal;

        H = 1;
        while (maxVal >= (T(1) << H))
            ++H;

        indexes = vector<vector<int>>(H, vector<int>(N + 1));

        vector<int> cur(N);
        vector<T> next(N);

        iota(cur.begin(), cur.end(), 0);
        for (int i = 0; i < H; i++) {
            T mask = T(1) << (H - i - 1);

            vector<int>& v = indexes[i];

            int zeroN = 0;
            for (int j = 0; j < N; j++) {
                v[j] = zeroN;
                zeroN += ((first[cur[j]] & mask) == 0);
            }
            v[N] = zeroN;

            int zeroPos = 0, onePos = zeroN;
            for (int j = 0; j < N; j++) {
                if (first[cur[j]] & mask) {
                    next[onePos++] = cur[j];
                } else {
                    next[zeroPos++] = cur[j];
                }
            }
            next.swap(cur);
        }

        values.resize(N);
        for (int i = 0; i < n; i++)
            values[i] = first[cur[i]];

        return cur;
    }


    int size() const {
        return N;
    }

    // (0 <= pos < N)
    // return (value, index)
    pair<T,int> get(int pos) const {
        T val = 0;
        for (int i = 0; i < H; i++) {
            const vector<int>& v = indexes[i];

            if (v[pos] == v[pos + 1]) {
                val = (val << 1) | 1;
                pos = v.back() + (pos - v[pos]);
            } else {
                val = val << 1;
                pos = v[pos];
            }
        }
        return make_pair(val, pos);
    }

    // inclusive (0 <= left <= right < N, 0 <= k)
    pair<T,pair<int,int>> kth(int left, int right, int k) const {
        if (k < 0 || right - left < k) {
            return make_pair(NaN, make_pair(-1, -1));
        }

        T val = 0;
        for (int i = 0; i < H; i++) {
            const vector<int>& v = indexes[i];

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
        return make_pair(val, make_pair(left, right));
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
            const vector<int>& v = indexes[i];

            if ((val >> (H - i - 1)) & 1) {
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

    //--- extended operations

    pair<T,pair<int,int>> findMaxXor(int left, int right, T k) const {
        T val = 0;

        T bit = T(1) << (H - 1);
        for (int i = 0; i < H; i++, bit >>= 1) {
            const vector<int>& v = indexes[i];

            int rangeN = right - left + 1;
            int count = v[right + 1] - v[left]; // zero count
            if (count == 0 || (count < rangeN && !(k & bit))) {
                val |= bit;
                left = v.back() + (left - v[left]);         // total 0 count + 1 count
                right = v.back() + (right - v[right + 1]);  // total 0 count + 1 count
            } else {
                left = v[left];                             
                right = v[right + 1] - 1;
            }
        }

        return make_pair(val, make_pair(left, right));
    }
};
