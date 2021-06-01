#pragma once

//--------- RMQ (Range Minimum Query) - Min Sparse Table ----------------------
// handing indexes instread of real values
struct SparseTableMinIndex {
    int N;
    vector<vector<int>> value;
    vector<int> H;

    vector<int> in;

    SparseTableMinIndex() {
    }

    SparseTableMinIndex(const int a[], int n) {
        build(a, n);
    }

    explicit SparseTableMinIndex(const vector<int>& a) {
        build(a);
    }


    void build(const int a[], int n) {
        this->N = n;

        in.resize(n);
        for (int i = 0; i < n; i++)
            in[i] = a[i];

        H.resize(n + 1);
        H[1] = 0;
        for (int i = 2; i < int(H.size()); i++)
            H[i] = H[i >> 1] + 1;

        value.assign(H.back() + 1, vector<int>(n));
        for (int i = 0; i < n; i++)
            value[0][i] = i;

        for (int i = 1; i < int(value.size()); i++) {
            vector<int>& prev = value[i - 1];
            vector<int>& curr = value[i];
            for (int j = 0; j < n; j++) {
                int j2 = j + (1 << (i - 1));
                if (j2 < n)
                    curr[j] = (in[prev[j]] < in[prev[j2]]) ? prev[j] : prev[j2];
                else
                    curr[j] = prev[j];
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
            return left;    //invalid range

        int level = H[right - left];

        if (in[value[level][left]] < in[value[level][right - (1 << level)]])
            return value[level][left];
        else
            return value[level][right - (1 << level)];
    }
};
