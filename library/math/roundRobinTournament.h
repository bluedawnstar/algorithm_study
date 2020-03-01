#pragma once

/*
  <Round-robin tournament - Scheduling algorithm>
    https://en.wikipedia.org/wiki/Round-robin_tournament#Scheduling_algorithm

    the number of total game : n * (n - 1) / 2

    "If n is even, then in each of (n - 1) rounds, n / 2 games can be run concurrently,
      provided there exist sufficient resources (e.g. courts for a tennis tournament)."
    "If n is odd, there will be n rounds, each with (n - 1) / 2 games,
      and one competitor having no game in that round."
*/
struct RoundRobinTournament {
    // Circle method, O(n)
    // - N is even, 1 <= round <= N - 1
    static vector<pair<int, int>> scheduleEven(const int A[], int N, int round) {
        int last = N - round;

        int gameCount = N / 2;

        vector<pair<int, int>> res;
        res.reserve(gameCount);
        res.emplace_back(A[0], A[last]);

        int i = last;
        int j = last;
        while (--gameCount > 0) {
            if (++i >= N)
                i = 1;
            if (--j <= 0)
                j = N - 1;
            res.emplace_back(min(A[i], A[j]), max(A[i], A[j]));
        }

        return res;
    }

    static vector<pair<int, int>> scheduleEven(const vector<int>& A, int round) {
        return scheduleEven(A.data(), int(A.size()), round);
    }
};
