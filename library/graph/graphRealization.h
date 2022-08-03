#pragma once

struct GraphRealization {
    //--- Erdos-Gallai theorem (https://en.wikipedia.org/wiki/Erd%C5%91s%E2%80%93Gallai_theorem)

    // O(N^2)
    static bool possibleSlow(const vector<int>& degree) {
        int N = int(degree.size());

        vector<int> d = degree;
        sort(d.begin(), d.end(), greater<int>());

        vector<long long> sum(N + 1);
        for (int i = 0; i < N; i++)
            sum[i + 1] = sum[i] + d[i];

        if (sum.back() & 1)
            return false;

        for (int k = 1; k <= N; k++) {
            long long right = 1ll * k * (k - 1);
            for (int i = k; i < N; i++)
                right += min(d[i], k);

            if (sum[k] > right)
                return false;
        }
        return true;
    }

    // O(NlogN)
    static bool possible(const vector<int>& degree) {
        int N = int(degree.size());
        
        vector<int> d = degree;
        sort(d.begin(), d.end(), greater<int>());

        vector<long long> sum(N + 1);
        for (int i = 0; i < N; i++)
            sum[i + 1] = sum[i] + d[i];

        if (sum.back() & 1)
            return false;

        for (int k = 1; k <= N; k++) {
            auto r = int(upper_bound(d.begin() + k, d.end(), k, greater<int>()) - d.begin());
            long long right = 1ll * k * (k - 1) +  1ll * k * (r - k) + (sum[N] - sum[r]);

            if (sum[k] > right)
                return false;
        }
        return true;
    }

    // O(NlogN)
    static bool possibleFast(const vector<int>& degree) {
        int N = int(degree.size());

        vector<int> d = degree;
        sort(d.begin(), d.end(), greater<int>());

        vector<long long> sum(N + 1);
        for (int i = 0; i < N; i++)
            sum[i + 1] = sum[i] + d[i];

        if (sum.back() & 1)
            return false;

        int r = N;
        for (int k = 1; k <= N; k++) {
            if (r < k) {
                r = k;
            } else {
                while (k < r && d[r - 1] < k)
                    r--;
            }

            long long right = 1ll * k * (k - 1) + 1ll * k * (r - k) + (sum[N] - sum[r]);

            if (sum[k] > right)
                return false;
        }
        return true;
    }


    //--- Havel-Hakimi algorithm (https://en.wikipedia.org/wiki/Havel%E2%80%93Hakimi_algorithm)

    // O(N^2)
    static bool existGraph(vector<int> degree) {
        int N = int(degree.size());

        sort(degree.begin(), degree.end(), greater<>());

        vector<int> aux(N);
        for (int i = 0; i < N && degree[i] > 0; ) {
            int v = degree[i++];

            int right = i + v;
            if (right > N || degree[right - 1] <= 0)
                return false;

            // copy
            memcpy(aux.data(), degree.data() + i, sizeof(degree[0]) * v);

            // merge
            int j = 0, k = i;
            while (j < v && right < N) {
                if (aux[j] > degree[right])
                    degree[k++] = aux[j++] - 1;
                else
                    degree[k++] = degree[right++];
            }
            while (j < v)
                degree[k++] = aux[j++] - 1;
        }

        return true;
    }
};
