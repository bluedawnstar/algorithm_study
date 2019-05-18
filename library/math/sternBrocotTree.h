#pragma once

// https://en.wikipedia.org/wiki/Stern%E2%80%93Brocot_tree
/*
  0:   0/1|                                                               |1/0

  1:   0/1|                              1/1                              |1/0
                          +---------------|---------------+                   
  2:   0/1|              1/2             1/1             2/1              |1/0
                  +-------|-------+       |       +-------|-------+
  3:   0/1|      1/3     1/2     2/3     1/1     3/2     2/1     3/1      |1/0
              +---|---+   |   +---|---+   |   +---|---+   |   +---|---+    
  4:   0/1|  1/4 1/3 2/5 1/2 3/5 2/3 3/4 1/1 4/3 3/2 5/3 2/1 5/2 3/1 4/1  |1/0

  ...

*/

struct SternBrocotTree {
    // f(T num, T denom, T level)
    template <typename T = long long>
    static void build(const function<void(T,T,T)>& f, T maxLevel, T a = 0, T b = 1, T c = 1, T d = 0) {
        buildSub(f, maxLevel, a, b, c, d, 1);
    }

    template <typename T = long long>
    static string findPath(T num, T denom) {
        string res;

        pair<T,T> L(0, 1);
        pair<T,T> R(1, 0);
        while (true) {
            pair<T, T> mid(L.first + R.first, L.second + R.second);
            if (num == mid.first && denom == mid.second)
                break;

            if (num * mid.second < denom * mid.first) {
                res.push_back('L');
                R = mid;
            } else {
                res.push_back('R');
                L = mid;
            }
        }

        return res;
    }

    template <typename T = long long>
    static pair<T,T> fractionFromPath(const string& path) {
        pair<T, T> L(0, 1);
        pair<T, T> R(1, 0);

        pair<T, T> mid(L.first + R.first, L.second + R.second);
        for (auto c : path) {
            if (c == 'L')
                R = mid;
            else
                L = mid;
            mid.first = L.first + R.first;
            mid.second = L.second + R.second;
        }

        return mid;
    }


    template <typename T = long long>
    static pair<T,T> findNearestFraction(T denomRangeMin, T denomRangeMax, T targetNum, T targetDenom) {
        pair<T, T> res;
        double resDiff = numeric_limits<double>::max();

        priority_queue<tuple<double, pair<T,T>, pair<T,T>>> Q;      // (-diff, denom min, denom max)
        Q.emplace(0.0, make_pair(0, 1), make_pair(1, 0));

        double diff;
        pair<T, T> L, R, mid;
        while (!Q.empty()) {
            tie(diff, L, R) = Q.top();
            Q.pop();

            pair<T, T> mid(L.first + R.first, L.second + R.second);
            if (mid.second > denomRangeMax)
                continue;

            // update result
            auto x = mid.first;
            auto y = mid.second;
            if (mid.second < denomRangeMin) {
                T scale = (denomRangeMin + mid.second - 1) / mid.second;
                x = mid.first * scale;
                y = mid.second * scale;
            }

            diff = fabs(1.0 * x / y - 1.0 * targetNum / targetDenom);
            if (denomRangeMin <= y && y <= denomRangeMax && diff < resDiff) {
                res.first = x;
                res.second = y;
                resDiff = diff;
            }

            // update range
            if (targetNum * mid.second < targetDenom * mid.first)
                Q.emplace(-diff, L, mid);
            else
                Q.emplace(-diff, mid, R);
        }

        return res;
    }

    template <typename T = long long, typename RealT = double>
    static pair<T,T> findNearestFraction(T denomRangeMin, T denomRangeMax, RealT target) {
        pair<T, T> res;
        double resDiff = numeric_limits<double>::max();

        priority_queue<tuple<double, pair<T,T>, pair<T,T>>> Q;      // (-diff, denom min, denom max)
        Q.emplace(0.0, make_pair(0, 1), make_pair(1, 0));

        double diff;
        pair<T, T> L, R, mid;
        while (!Q.empty()) {
            tie(diff, L, R) = Q.top();
            Q.pop();

            pair<T, T> mid(L.first + R.first, L.second + R.second);
            if (mid.second > denomRangeMax)
                continue;

            // update result
            auto x = mid.first;
            auto y = mid.second;
            if (mid.second < denomRangeMin) {
                T scale = (denomRangeMin + mid.second - 1) / mid.second;
                x = mid.first * scale;
                y = mid.second * scale;
            }

            diff = fabs(1.0 * x / y - 1.0 * target);
            if (denomRangeMin <= y && y <= denomRangeMax && diff < resDiff) {
                res.first = x;
                res.second = y;
                resDiff = diff;
            }

            // update range
            if (target * mid.second < mid.first)
                Q.emplace(-diff, L, mid);
            else
                Q.emplace(-diff, mid, R);
        }

        return res;
    }

private:
    template <typename T = long long>
    static void buildSub(const function<void(T,T,T)>& f, T maxLevel, T a, T b, T c, T d, T level) {
        if (level > maxLevel)
            return;

        T x = a + c;
        T y = b + d;

        f(x, y, level);

        buildSub(f, maxLevel, a, b, x, y, level + 1);
        buildSub(f, maxLevel, x, y, c, d, level + 1);
    }
};
