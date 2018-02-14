#pragma once

template <typename T>
struct PersistentStack {
    int N;
    vector<vector<int>> times;
    vector<vector<T>> values;
    vector<int> timeSize;
    int currTime;

    PersistentStack(int maxTime) : N(maxTime + 1), times(N), values(N), timeSize(N), currTime(1) {
    }

    // O(1)
    int size() {
        return timeSize[currTime - 1];
    }

    // O(1)
    int size(int time) {
        return timeSize[time];
    }

    // O(logN)
    const T& top() const {
        int sz = timeSize[currTime - 1];
        return get(currTime - 1, sz - 1);
    }

    // O(logN)
    const T& top(int time) const {
        int sz = timeSize[time];
        return get(time, sz - 1);
    }

    // O(1)
    // return current time (> 0)
    int push(const T& val) {
        int sz = timeSize[currTime - 1];
        times[sz].push_back(currTime);
        values[sz].push_back(val);
        timeSize[currTime] = sz + 1;
        return currTime++;
    }

    // O(1)
    // return current time (> 0)
    int pop() {
        int sz = timeSize[currTime - 1];
        timeSize[currTime] = sz - 1;
        return currTime++;
    }

//private:
    // O(logN)
    // 0 < time, 0 <= index < N
    const T& get(int time, int index) const {
        int i = int(upper_bound(times[index].begin(), times[index].end(), time) - times[index].begin());
        return values[index][i - 1];
    }

    // O(logN)
    const T& get(int index) const {
        return get(currTime, index);
    }
};
