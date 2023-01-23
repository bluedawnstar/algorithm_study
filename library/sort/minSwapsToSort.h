#pragma once

struct MinSwapsToSort {
    // minimum number of swaps required to sort the array with distinct values
    template <typename T>
    static int count(const T A[], int size) {
        vector<int> ord(size);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), [A](int a, int b) {
            return A[a] < A[b];
        });

        vector<bool> vis(size);

        int ans = 0;
        for (int i = 0; i < size; i++) {
            if (vis[i] || ord[i] == i)
                continue;

            int cycleSize = 0;
            for (int j = i; !vis[j]; j = ord[j]) {
                vis[j] = true;
                cycleSize++;
            }
            ans += cycleSize - 1;
        }

        return ans;
    }

    template <typename T>
    static int count(const vector<T>& A) {
        return count(A.data(), int(A.size()));
    }
};
