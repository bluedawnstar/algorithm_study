#pragma once

template <typename T>
struct ReservoirSampling {
    static vector<T> select(const T in[], int n, int k) {
        vector<T> res;
        res.reserve(k);

        int i;

        for (i = 0; i < k; i++)
            res.push_back(in[i]);

        for (; i < n; i++) {
            int j = rand() % (i + 1);

            if (j < k)
                res[j] = in[i];
        }

        return res;
    }

    static vector<T> select(const vector<T>& in, int k) {
        return select(&in[0], int(in.size()), k);
    }
};