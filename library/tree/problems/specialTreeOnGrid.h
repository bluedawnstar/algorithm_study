#pragma once

/*
  1. question
     https://www.codechef.com/problems/VILLNET
     https://discuss.codechef.com/t/villnet-editorial/78917

     The graph has a vertex for each pair (x,y) where x and y are odd co-prime integers, and y is positive.
     A vertex (a,b) has four edges, namely:

      1) (a, b)-(a + 2b, b),
      2) (a, b)-(a - 2b, b),
      3) (a, b)-(a, b + 2a) or (a, b)-(-a, -(b + 2a)) depending on which has positive y-coordinate,
      4) (a, b)-(a, b - 2a) or (a, b)-(-a, -(b - 2a)) depending on which has positive y-coordinate.

     Determine the minimum distance between two vertices of this graph.

  2. properties
     - It's a tree
*/
struct SpecialTreeOnGrid1 {
    // O(log(MAXX+MAXY)^2)
    static long long calculateDistance(long long x1, long long y1, long long x2, long long y2) {
        Position p1(x1, y1);
        Position p2(x2, y2);

        long long lo = 2, hi = max(p1.distance(), p2.distance()) * 2;

        long long steps = 0;
        while (lo <= hi) {
            long long mid = lo + (hi - lo) / 2;

            Position lca1 = p1;
            Position lca2 = p2;
            long long steps1 = moveInsideDiamond(lca1, mid);
            long long steps2 = moveInsideDiamond(lca2, mid);

            if (lca1 == lca2) {
                lo = mid + 1;
            } else {
                hi = mid - 1;

                steps += steps1;
                steps += steps2;
                p1 = lca1;
                p2 = lca2;
            }
        }

        // find the lengths of the paths to the LCA
        if (lo > 2) {
            steps += moveInsideDiamond(p1, lo - 1);
            steps += moveInsideDiamond(p2, lo - 1);
        }

        // crossover (1,1) - (-1,1)
        if (p1.distance() == 2 && p1 != p2)
            steps++;

        return steps;
    }

private:
    struct Position {
        long long x, y;

        Position() : x(0), y(0) {
        }

        Position(long long x, long long y) : x(x), y(y) {
        }

        long long distance() const {
            return abs(x) + abs(y);
        }

        bool operator ==(const Position& rhs) const {
            return (x == rhs.x && y == rhs.y) || (x == -rhs.x && y == -rhs.y);
        }

        bool operator !=(const Position& rhs) const {
            return !operator ==(rhs);
        }
    };

    // delta >= 0
    static long long moveToZero(long long value, long long delta) {
        return (value > 0) ? (value - delta) : (value + delta);
    }

    /*

          .       |       .
            . 2<--|-----.-1                 P[2].X = P[1].X - 2 * stepSize = -P[1].Y + stepSize
              .   |   .
              : . | . 3                     | P[3].X - P[1].X | = 2 * stepSize
        ----------+------------             | P[2].Y - P[1].Y | = 2 * stepSize
              3 . | .
              .   |   .
            .     |     .
          .       |       .

    */
    // move to the first point on the path to (1,1) that is inside the diamond |x|+|y| <= d
    // - return the number of steps, O(log(MAXX+MAXY))
    static long long moveInsideDiamond(Position& pos, long long d) {
        long long totalSteps = 0;

        while (pos.distance() > d) {
            // spiral movements
            {
                long long stepSize = abs(abs(pos.x) - abs(pos.y));
                long long steps = (pos.distance() - d) / (2 * stepSize);    // diagonal moves

                pos.x = moveToZero(pos.x, steps * stepSize);
                pos.y = moveToZero(pos.y, steps * stepSize);

                // rotate if an odd number of steps (move back one step)
                if (steps & 1) {
                    swap(pos.x, pos.y);
                    pos.y = -pos.y;
                }

                totalSteps += steps;
            }
            // linear movements
            {
                long long stepSize = min(abs(pos.x), abs(pos.y)) * 2;
                long long steps = (pos.distance() - d + stepSize - 1) / stepSize;

                if (abs(pos.x) > abs(pos.y))
                    pos.x = moveToZero(pos.x, steps * stepSize);
                else
                    pos.y = moveToZero(pos.y, steps * stepSize);
                totalSteps += steps;
            }
        }
        return totalSteps;
    }
};

struct SpecialTreeOnGrid2 {
    // O(log(MAXX+MAXY)^2)
    static long long calculateDistance(long long x1, long long y1, long long x2, long long y2) {
        vector<pair<long long, int>> path1 = getPath(x1, y1);
        vector<pair<long long, int>> path2 = getPath(x2, y2);

        // erase path to lca
        while (!path1.empty() && !path2.empty() && path1.back().second == path2.back().second) {
            auto e1 = path1.back();
            auto e2 = path2.back();
            path1.pop_back();
            path2.pop_back();

            if (e1.first < e2.first)
                path2.emplace_back(e2.first - e1.first, e2.second);
            else if (e1.first > e2.first)
                path1.emplace_back(e1.first - e2.first, e1.second);
        }

        long long res = 0;

        for (auto& it : path1)
            res += it.first;

        for (auto& it : path2)
            res += it.first;

        return res;
    }

    // O(log(MAXX+MAXY))
    // result = { (step, type}, ...  }, compressed path
    static vector<pair<long long, int>> getPath(long long x, long long y) {
        vector<pair<long long, int>> res;

        bool needSwapRoot = false;
        if (x < 0) {
            needSwapRoot = true;
            x = -x;
        }

        while (!(x <= 1 && y <= 1)) {
            if (x >= y) {
                if (x > 2ll * y) {
                    long long steps = x / (2ll * y);
                    x -= steps * (2ll * y);
                    res.emplace_back(steps, 0);         // 'W'
                } else {
                    long long delta = x - y;
                    long long steps = y / delta;

                    x -= delta * steps;
                    y -= delta * steps;

                    if ((steps & 1) == 0)
                        res.emplace_back(steps, 2);     // 'N'
                    else {
                        res.emplace_back(steps, 3);     // 'E', move back one step

                        needSwapRoot = !needSwapRoot;
                        swap(x, y);
                    }
                }
            } else {
                if (y > 2ll * x) {
                    long long steps = y / (2ll * x);
                    y -= steps * (2ll * x);
                    res.emplace_back(steps, 1);         // 'S'
                } else {
                    long long delta = y - x;
                    long long steps = x / delta;

                    x -= delta * steps;
                    y -= delta * steps;

                    if ((steps & 1) == 0)
                        res.emplace_back(steps, 3);     // 'E'
                    else {
                        res.emplace_back(steps, 2);     // 'N', move back one step

                        needSwapRoot = !needSwapRoot;
                        swap(x, y);
                    }
                }
            }
        }

        if (needSwapRoot)
            res.emplace_back(1ll, 4);   // (-1, 1) -> (1, 1)

        return res;
    }
};
