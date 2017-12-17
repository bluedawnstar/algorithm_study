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

    template <typename U>
    SparseTableMinIndex(const U& a, int n) {
        build(a, n);
    }

    template <typename U>
    void build(const U& a, int n) {
        this->N = n;

        in.resize(n);
        for (int i = 0; i < n; i++)
            in[i] = a[i];

        H.resize(n + 1);
        H[1] = 0;
        for (int i = 2; i < (int)H.size(); i++)
            H[i] = H[i >> 1] + 1;

        value.resize(H.back() + 1, vector<int>(n));
        for (int i = 0; i < n; i++)
            value[0][i] = i;

        for (int i = 1; i < (int)value.size(); i++) {
            vector<int>& prev = value[i - 1];
            vector<int>& curr = value[i];
            for (int v = 0; v < n; v++) {
                int v2 = v + (1 << (i - 1));
                if (v2 < n) {
                    if (in[prev[v]] < in[prev[v2]])
                        curr[v] = prev[v];
                    else
                        curr[v] = prev[v2];
                } else {
                    curr[v] = prev[v];
                }
            }
        }
    }

    // inclusive
    int query(int left, int right) {
        right++;
        if (right <= left)
            return left;    //invalid range

        int k = H[right - left];
        vector<int>& mink = value[k];

        if (in[mink[left]] < in[mink[right - (1 << k)]])
            return mink[left];
        else
            return mink[right - (1 << k)];
    }
};
