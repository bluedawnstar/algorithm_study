#pragma once

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
