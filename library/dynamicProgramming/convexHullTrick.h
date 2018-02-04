#pragma once

// 1) O(n^2) => O(n)
//    dp[i] = min { b[j] + m[j] * x[i] }
//            j<i
//    => m[j] is 'slope', x[i] is 'x', b[j] is 'y-intercept' (b[j] = dp[j])
//
// 2) O(kn^2) => O(kn)
//    dp[i][j] = min { b[i - 1][k] + m[k] * x[j] }
//               k<j
//    => m[k] is 'slope', x[j] is 'x', b[i - 1][k] is 'y-intercept'
template <typename T>
struct DPConvexHullTrickMin {
    struct Line {
        T m, b;     // f(x) = m * x + b

        template <typename U>
        inline T get(U x) const {
            return m * x + b;
        }
    };

    deque<Line> lines;

    // when Xs of queries (not insert) are ascending, x[j] <= x[j + 1]
    // PRECONDITION: m[k] >= m[k + 1]
    void insert(T m, T b) {
        Line l{ m, b };

        while (lines.size() > 1 && ccw(lines[lines.size() - 2], lines[lines.size() - 1], l))
            lines.pop_back();

        lines.push_back(l);
    }

    // when Xs of queries (not insert) are descending, x[j] >= x[j + 1]
    // PRECONDITION: m[k] <= m[k + 1]
    void insertReverse(T m, T b) {
        Line l{ m, b };

        while (lines.size() > 1 && cw(lines[lines.size() - 2], lines[lines.size() - 1], l))
            lines.pop_back();

        lines.push_back(l);
    }

    template <typename U>
    T query(U x) {
        if (lines.empty())
            return 0;

        while (lines.size() > 1 && lines[0].get(x) >= lines[1].get(x))
            lines.pop_front();

        return lines[0].get(x);
    }

private:
    static T area(const Line& a, const Line& b, const Line& c) {
        T ax = (b.m - a.m);
        T bx = (c.b - a.b);
        T ay = (c.m - a.m);
        T by = (b.b - a.b);
        return ax * bx - ay * by;
    }

    static bool ccw(const Line& a, const Line& b, const Line& c) {
        return area(a, b, c) >= 0;
    }

    static bool cw(const Line& a, const Line& b, const Line& c) {
        return area(a, b, c) <= 0;
    }
};
