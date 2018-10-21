#pragma once

// Klee's algorithm : O(nlogn)
// [x1, x2)
int lengthOfLineSegmentUnion(const vector<pair<int, int>>& lines) {
    int n = int(lines.size());
    vector<pair<int, int>> x(n * 2);
    for (int i = 0; i < n; i++) {
        x[i * 2] = make_pair(lines[i].first, -1);
        x[i * 2 + 1] = make_pair(lines[i].second, 1);
    }
    sort(x.begin(), x.end());

    int res = 0;

    int cnt = 0;
    for (int i = 0; i < n * 2; i++) {
        if (cnt && i)
            res += x[i].first - x[i - 1].first;
        cnt += x[i].second;
    }

    return res;
}
