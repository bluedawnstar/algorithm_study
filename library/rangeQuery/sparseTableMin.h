#pragma once

//--------- RMQ (Range Minimum Query) - Min Sparse Table ----------------------

struct SparseTableMin {
    int N;
    vector<vector<int>> value;
    vector<int> H;

    SparseTableMin() {
    }

    SparseTableMin(const int a[], int n) {
        build(a, n);
    }

    explicit SparseTableMin(const vector<int>& a) {
        build(a);
    }


    void build(const int a[], int n) {
        this->N = n;

        H.resize(n + 1);
        H[1] = 0;
        for (int i = 2; i < int(H.size()); i++)
            H[i] = H[i >> 1] + 1;

        value.assign(H.back() + 1, vector<int>(n));
        for (int i = 0; i < n; i++)
            value[0][i] = a[i];

        for (int i = 1, step = 1; i < int(value.size()); i++, step <<= 1) {
            for (int j = 0; j < n; j++) {
                if (j + step < n)
                    value[i][j] = min(value[i - 1][j], value[i - 1][j + step]);
                else
                    value[i][j] = value[i - 1][j];
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
            return INT_MAX;

        int level = H[right - left];
        return min(value[level][left], value[level][right - (1 << level)]);
    }
};
