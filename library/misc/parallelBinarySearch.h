#pragma once

/*
 <Usage>

    ParallelBinarySearch solver(N, maxValue);

    while (true) {
        //...

        if (!solver.updateRange())
            break;

        solver.updateValue(
            [](int midValue) {
                //... update operations
            },
            [](int index, int midValue) {
                //... possible operations
                return true; // or false
            }
        );
    }

    // get an answer value
    int ans = solver.get(index);
 */

// 0-based index
struct ParallelBinarySearch {
    typedef int T;

    int N;
    T maxValue;             // [0, maxValue]

    vector<T> L;            // low positions
    vector<T> R;            // high positions
    vector<vector<int>> M;  // mid positions (M[mid] -> index)

    T lastValue;

    ParallelBinarySearch(int n, T maxVal)
        : N(n), maxValue(maxVal), L(n, 0), R(n, maxVal) {
    }

    // O(N)
    bool updateRange() {
        bool changed = false;

        M = vector<vector<int>>(maxValue + 1);
        lastValue = -1;

        for (int i = 0; i < N; i++) {
            if (L[i] <= R[i]) {
                int mid = L[i] + (R[i] - L[i]) / 2;
                M[mid].push_back(i);

                lastValue = max(lastValue, mid);
                changed = true;
            }
        }

        return changed;
    }

    // O(maxValue * updateOpSpeed + N * possibleOpSpeed)
    // void updateOp(int midValue);
    // bool possible(int index, int midValue); 
    void updateValue(const function<void(T)>& updateOp, const function<bool(int, T)>& possibleOp) {
        for (T mid = 0; mid <= lastValue; mid++) {
            updateOp(mid);
            for (int index : M[mid]) {
                if (possibleOp(index, mid))
                    R[index] = mid - 1;
                else
                    L[index] = mid + 1;
            }
        }
    }

    T get(int index) const {
        return L[index];
    }
};
