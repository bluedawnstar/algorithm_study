#pragma once

// intervals = { (startTime, endTime), ... }, O(N)
template <typename T>
inline int countOverlapIntervals(vector<pair<T,T>> intervals) {
    sort(intervals.begin(), intervals.end(), [](const pair<T,T>& a, const pair<T,T>& b) {
        return (a.second != b.second) ? (a.second < b.second) : (a.first < b.first);
    });

    int res = 0, lastTime = numeric_limits<T>::min();
    for (int i = 0; i < int(intervals.size()); i++) {
        if (lastTime <= intervals[i].first) {
            res++;
            lastTime = intervals[i].second;
        }
    }

    return res;
}
