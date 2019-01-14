#pragma once

#include <vector>

//--------- RMQ (Range Minimum Query) - Min Segment Tree ----------------------

// RMQ (Range Minimum Query)
struct RMQ {
    int       N;
    vector<int> value;
    vector<int> valueId;

    RMQ() {
    }

    explicit RMQ(int size) {
        init(size);
    }

    RMQ(int v[], int size) {
        build(v, size);
    }

    RMQ(const vector<int>& v) {
        build(v);
    }


    void init(int size) {
        N = 1;
        while (N <= size)
            N <<= 1;

        value.resize(N * 2);
        valueId.resize(N * 2);
        for (int i = 0; i < N; i++)
            valueId[N + i] = i;

        for (int i = 0; i < size; i++)
            update(i, INT_MAX);
    }

    void build(const int v[], int size) {
        init(size);

        for (int i = 0; i < size; i++)
            update(i, v[i]);
    }

    void build(const vector<int>& v) {
        build(&v[0], int(v.size()));
    }


    void update(int x, int val) {
        x += N;
        value[x] = val;

        while (x > 1) {
            x >>= 1;
            int left = value[x << 1];               // left = value[x * 2]
            int right = value[(x << 1) | 1];        // right = value[x * 2 + 1]
            if (left <= right) {
                value[x] = left;
                valueId[x] = valueId[x << 1];       // valueId = valueId[x * 2]
            } else {
                value[x] = right;
                valueId[x] = valueId[(x << 1) | 1]; // valueId = valueId[x * 2 + 1]
            }
        }
    }

    // O(log(right - left)), inclusive
    int query(int left, int right) const {
        left += N;
        right += N;
        int ret = INT_MAX;
        while (left <= right) {
            if ((left & 1) == 1) {
                ret = min(ret, value[left]);
                left++;
            }
            if ((right & 1) == 0) {
                ret = min(ret, value[right]);
                right--;
            }
            left >>= 1;
            right >>= 1;
        }
        return ret;
    }
};
