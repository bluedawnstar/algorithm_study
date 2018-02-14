#pragma once

template <typename T>
struct PartiallyPersistentArray {
    int N;
    vector<vector<int>> times;
    vector<vector<T>> values;
    int currTime;

    // time 0 : initial value
    PartiallyPersistentArray(int n, T initValue)
        : N(n), times(n, vector<int>(1)), values(n, vector<T>(1, initValue)) {
        currTime = 1;
    }

    // O(logN)
    // 0 <= time (time 0 is initial state), 0 <= index < N
    const T& get(int time, int index) const {
        int i = int(upper_bound(times[index].begin(), times[index].end(), time) - times[index].begin());
        return values[index][i - 1];
    }

    const T& getLast(int index) const {
        return values[index].back();
    }

    // O(1)
    // return current time
    int set(int index, const T& val) {
        times[index].push_back(currTime);
        values[index].push_back(val);
        return currTime++;
    }
};
