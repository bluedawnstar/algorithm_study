#pragma once

template <typename T>
struct ArraySum {
    vector<T> sum;

    ArraySum() {
    }

    explicit ArraySum(const T* in, int N) {
        build(in, N);
    }

    explicit ArraySum(const vector<T>& in) {
        build(in);
    }

    void build(const vector<T>& in) {
        build(&in[0], (int)in.size());
    }

    void build(const T* in, int N) {
        sum.resize(N + 1);
        sum[0] = 0;
        for (int i = 1; i <= N; i++) {
            sum[i] = sum[i - 1] + *in++;
        }
    }

    // inclusive (0 <= left <= right < N)
    T query(int left, int right) {
        return sum[right + 1] - sum[left];
    }
};
