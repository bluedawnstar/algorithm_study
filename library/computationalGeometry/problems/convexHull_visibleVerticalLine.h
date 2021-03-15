#pragma once

/*
               +
       +       |   +
       |   +   |   |   +
       |   |   |   |   |
   ----+---+---+---+---+----
       0   1   2   3   4
   =>
      -1   1   0   2   2
*/
struct VisibleVerticalLine {
    // - H[i] : height of i
    // return the position of farthest v-line to be visible from each top point
    static vector<int> findPreviousVisibleVLine(const int H[], int N) {
        vector<int> prev(N);
        prev[0] = -1;
        prev[1] = 0;
        for (int i = 2; i < N; i++) {
            int j = i - 1;
            while (prev[j] >= 0 && !ccw(make_pair(i, H[i]), make_pair(j, H[j]), make_pair(prev[j], H[prev[j]])))
                j = prev[j];
            prev[i] = j;
        }
        return prev;
    }

    static vector<int> findPreviousVisibleVLine(const vector<int>& H) {
        return findPreviousVisibleVLine(H.data(), int(H.size()));
    }

    static long long cross(const pair<int, int>& base, const pair<int, int>& a, const pair<int, int>& b) {
        return static_cast<long long>(a.first - base.first) * (b.second - base.second)
             - static_cast<long long>(a.second - base.second) * (b.first - base.first);
    }

    static bool ccw(const pair<int, int>& base, const pair<int, int>& a, const pair<int, int>& b) {
        return cross(base, a, b) > 0;
    }

    static bool cw(const pair<int, int>& base, const pair<int, int>& a, const pair<int, int>& b) {
        return cross(base, a, b) < 0;
    }
};
