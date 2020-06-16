#pragma once

/*
    https://www.codechef.com/problems/TTUPLE

    X = { X[0], X[1], ..., X[N-1] }
    Y = { Y[0], Y[1], ..., Y[N-1] }

    <Operations>
      - add any integer D to a subset of elements of X
      - multiply any integer D to a subset of elements of X

    <Goal>
      - Find the minimum number of operations needed to transform X into Y.
*/
struct TransformOperationByAddOrMult {
    int answer;

    int solve(const vector<int>& X, const vector<int>& Y) {
        vector<pair<long long, long long>> XY2;
        for (int i = 0; i < int(X.size()); i++) {
            if (X[i] != Y[i])
                XY2.emplace_back(X[i], Y[i]);
        }
        sort(XY2.begin(), XY2.end());
        XY2.erase(unique(XY2.begin(), XY2.end()), XY2.end());

        answer = int(XY2.size());
        solve(XY2, 0);
        return answer;
    }

private:
    void solve(const vector<pair<long long, long long>>& XY, int count) {
        if (count < answer && XY.empty()) {
            answer = min(answer, count);
            return;
        }

        if (count + 1 >= answer)
            return;

        int N = int(XY.size());

        vector<long long> add;
        for (int i = 0; i < N; i++) {
            if (XY[i].first != XY[i].second)
                add.push_back(XY[i].second - XY[i].first);
        }

        vector<long long> mult;
        for (int i = 0; i < N; i++) {
            if (XY[i].first != 0 && XY[i].second % XY[i].first == 0 && XY[i].first != XY[i].second)
                mult.push_back(XY[i].second / XY[i].first);
        }
        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                long long s = scale(XY[i].first, XY[j].first, XY[i].second, XY[j].second);
                if (s != 1)
                    mult.push_back(s);
            }
        }
        mult.push_back(0);

        sort(add.begin(), add.end());
        add.resize(unique(add.begin(), add.end()) - add.begin());

        sort(mult.begin(), mult.end());
        mult.resize(unique(mult.begin(), mult.end()) - mult.begin());

        vector<pair<long long, long long>> XY2;
        XY2.reserve(N);

        for (int mask = 1; mask < (1 << N); mask++) {
            for (auto a : add) {
                XY2.clear();
                for (int i = 0; i < N; i++) {
                    if (mask & (1 << i)) {
                        if (XY[i].first + a != XY[i].second)
                            XY2.emplace_back(XY[i].first + a, XY[i].second);
                    } else {
                        XY2.push_back(XY[i]);
                    }
                }
                solve(XY2, count + 1);
            }

            for (auto s : mult) {
                XY2.clear();
                for (int i = 0; i < N; i++) {
                    if (mask & (1 << i)) {
                        if (XY[i].first * s != XY[i].second) {
                            XY2.emplace_back(XY[i].first * s, XY[i].second);
                        }
                    } else {
                        XY2.push_back(XY[i]);
                    }
                }
                solve(XY2, count + 1);
            }
        }
    }

    static long long scale(long long x1, long long x2, long long y1, long long y2) {
        if (x1 != x2 && (y2 - y1) % (x2 - x1) == 0)
            return (y2 - y1) / (x2 - x1);
        else
            return 1;
    }
};
