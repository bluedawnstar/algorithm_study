#pragma once

#include <limits>
#include <vector>

//--------- RMQ (Range Minimum Query) - Min Segment Tree ----------------------

// RMQ (Range Minimum Query)
template <typename T>
struct RMQ {
    int       N;
    vector<T> value;

    RMQ() {
    }

    explicit RMQ(int size) {
        init(size);
    }

    RMQ(const T v[], int size) {
        build(v, size);
    }

    RMQ(const vector<T>& v) {
        build(v);
    }


    // O(N)
    void init(int size) {
        N = 1;
        while (N < size)
            N <<= 1;
        value.assign(N * 2, numeric_limits<T>::max());
    }

    // O(N)
    void build(const T v[], int size) {
        init(size);
        
        for (int i = 0; i < size; i++)
            value[N + i] = v[i];

        for (int i = N - 1; i > 0; i--)
            value[i] = min(value[i << 1], value[(i << 1) | 1]);
    }

    void build(const vector<T>& v) {
        build(&v[0], int(v.size()));
    }


    // O(logN)
    void update(int index, T val) {
        index += N;
        value[index] = val;

        while (index > 1) {
            index >>= 1;
            value[index] = min(value[index << 1], value[(index << 1) | 1]);
        }
    }


    T get(int index) const {
        return value[N + index];
    }

    // O(|right - left|), inclusive
    T query(int left, int right) const {
        T res = numeric_limits<T>::max();

        left += N;
        right += N;
        while (left <= right) {
            if ((left & 1) == 1) {
                res = min(res, value[left]);
                left++;
            }
            if ((right & 1) == 0) {
                res = min(res, value[right]);
                right--;
            }
            left >>= 1;
            right >>= 1;
        }

        return res;
    }
};
