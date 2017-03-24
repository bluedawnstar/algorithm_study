#pragma once

// 1) O(n^2) => O(n)
//    dp[i] = min { dp[j] + b[j] * a[i] }
//            j<i
//    => b[j] is slope, dp[j] is xterm
//
// 2) O(kn^2) => O(kn)
//    dp[i][j] = min { dp[i - 1][k] + b[k] * a[j] }
//               k<j
//    => b[k] is slope, dp[i - 1][k] is xterm
template <typename T>
struct DPConvexHullTrick {
    struct Line {
        T slope, xterm;

        template <typename U>
        inline T get(U x) const {
            return slope * x + xterm;
        }
    };

    deque<Line> lines;

    // a[j] <= a[j + 1], b[k] >= b[k + 1]
    void insert(T slope, T xterm) {
        Line l{ slope, xterm };

        while (lines.size() > 1 && ccw(lines[lines.size() - 2], lines[lines.size() - 1], l))
            lines.pop_back();

        if (lines.size() == 1 && lines.front().xterm > l.xterm)
            lines.pop_back();
        lines.push_back(l);
    }

    // a[j] >= a[j + 1], b[k] <= b[k + 1]
    void insertReverse(T slope, T xterm) {
        Line l{ slope, xterm };

        while (lines.size() > 1 && cw(lines[lines.size() - 2], lines[lines.size() - 1], l))
            lines.pop_back();

        if (lines.size() != 1 || lines.front().xterm > l.xterm)
            lines.push_back(l);
    }

    template <typename U>
    T query(U x) {
        if (lines.empty())
            return 0;

        while (lines.size() > 1 && lines[0].get(x) > lines[1].get(x))
            lines.pop_front();

        return lines[0].get(x);
    }

private:
    static T area(const Line& a, const Line& b, const Line& c) {
        T ax = (b.slope - a.slope);
        T bx = (c.xterm - a.xterm);
        T ay = (c.slope - a.slope);
        T by = (b.xterm - a.xterm);
        return ax * bx - ay * by;
    }

    static bool ccw(const Line& a, const Line& b, const Line& c) {
        return area(a, b, c) > 0;
    }

    static bool cw(const Line& a, const Line& b, const Line& c) {
        return area(a, b, c) < 0;
    }
};
