#pragma once

// for query to find overlapped ranges

// inclusive operation
template <typename T, T INF = 0x3f3f3f3f>
struct RangeSet {
    int N;
    vector<pair<T, T>> ranges;

    vector<int> order;      // sorted order by (left, right)
    vector<T> maxRight;     // maxRight[i] = max{ ranges[order[0]].second, ranges[order[1]].second, ..., ranges[order[i]].second }
    set<pair<T, int>> S;    // (left, index of order)
    vector<int> next, prev; // next[index of order] = index of order;

    // ranges[i] = (left, right), inclusive
    void build(vector<pair<T, T>>&& ranges) {
        this->ranges = ranges;
        build();
    }

    void build(const vector<pair<T, T>>& ranges) {
        this->ranges = ranges;
        build();
    }

    void build(const pair<T, T> ranges[], int n) {
        this->ranges.assign(ranges, ranges + n);
        build();
    }


    // get overlapped ranges
    // - overlapped[i] = index of ranges, inclusive
    void getOverlapped(T left, T right, vector<int>& overlapped) const {
        overlapped.clear();

        auto it = S.lower_bound(make_pair(right + 1, -1));
        for (int i = prev[it->second]; i >= 0 && left <= maxRight[i]; i = prev[i]) {
            int v = order[i];
            if (left <= ranges[v].second && ranges[v].first <= right)
                overlapped.push_back(v);
        }
    }

    // get and erase overlapped ranges
    // - overlapped[i] = index of ranges, inclusive
    // amortized O(logN)
    void popOverlapped(T left, T right, vector<int>& overlapped) {
        overlapped.clear();

        auto it = S.lower_bound(make_pair(right + 1, -1));
        for (int i = prev[it->second]; i >= 0 && left <= maxRight[i]; i = prev[i]) {
            int v = order[i];
            if (left <= ranges[v].second && ranges[v].first <= right) {
                overlapped.push_back(v);

                prev[next[i]] = prev[i];
                if (prev[i] >= 0)
                    next[prev[i]] = next[i];
                S.erase(make_pair(ranges[v].first, i));
            }
        }
    }

private:
    // O(N*logN)
    void build() {
        N = int(ranges.size());

        order = vector<int>(N);
        maxRight = vector<int>(N);
        next = vector<int>(N);
        prev = vector<int>(N + 1);
        S.clear();

        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [this](int a, int b) {
            return ranges[a] < ranges[b];
        });

        maxRight[0] = ranges[order[0]].second;
        prev[0] = -1;
        next[0] = 1;
        S.emplace(ranges[order[0]].first, 0);
        for (int i = 1; i < N; i++) {
            maxRight[i] = max(maxRight[i - 1], ranges[order[i]].second);
            prev[i] = i - 1;
            next[i] = i + 1;
            S.emplace(ranges[order[i]].first, i);
        }
        prev[N] = N - 1;
        S.emplace(INF, N);
    }
};
