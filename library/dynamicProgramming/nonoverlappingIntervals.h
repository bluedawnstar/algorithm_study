#pragma once

// https://leetcode.com/problems/non-overlapping-intervals/
// intervals = { (startTime, endTime), ... }, O(NlogN)
template <typename T>
int maximumNonoverlappingIntervals(vector<pair<T,T>> intervals) {
    int N = int(intervals.size());

    sort(intervals.begin(), intervals.end(), [&](const pair<T,T>& a, const pair<T,T>& b) {
        return (a.second != b.second) ? (a.second < b.second) : (a.first < b.first);
    });

    int res = 0;
    T lastTime = numeric_limits<T>::min();
    for (auto& x : intervals) {
        if (lastTime <= x.first) {
            res++;
            lastTime = x.second;
        }
    }

    return res;
}

// https://leetcode.com/problems/maximum-profit-in-job-scheduling/
// intervals = { (startTime, endTime, weight), ... }, O(NlogN)
template <typename T, typename U>
U maximumWeightedNonoverlappingIntervals(vector<tuple<T,T,U>> intervals) {
    int N = int(intervals.size());

    sort(intervals.begin(), intervals.end(), [&](const tuple<T,T,U>& a, const tuple<T,T,U>& b) {
        return (get<1>(a) != get<1>(b)) ? (get<1>(a) < get<1>(b)) : (get<0>(a) < get<0>(b));
    });

    map<T,U> M;

    U res = 0;
    for (auto& x : intervals) {
        auto it = M.upper_bound(get<0>(x));

        U c = get<2>(x);
        if (it != M.begin()) {
            --it;
            res = max(res, c + it->second);
        }
        res = max(res, c);

        M[get<1>(x)] = res;
    }

    return res;
}
