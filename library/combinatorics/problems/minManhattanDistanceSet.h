#pragma once

/*
  https://www.codechef.com/problems/MEOW1234
  https://discuss.codechef.com/t/meow1234-editorial/85251

  1. Problem
    - N different cells are checkered; for each valid i, the coordinates of the i-th checkered cell are (Xi, Yi).
    - A set of cells is SUITABLE if it meets the following conditions:
      . All initial marked cells belong to this set.
      . For any two cells a and b belonging to this set,
         there is a path between them with length |Xa - Xb| + |Ya - Yb| which only visits cells in this set.
        Each pair of consecutive cells in a path must have a common side.

  2. Answer
    - the smallest possible size of a SUITABLE set of cells
    - number of sets with this smallest size
*/
// Set with Min Manhattan Distance among All Pairs
template <int mod = 1'000'000'007>
struct MinManhattanDistanceSet {
    static const int INF = 0x3f3f3f3f;

    int MAXX, MAXY;

    MinManhattanDistanceSet() {
    }

    MinManhattanDistanceSet(int MAXX, int MAXY) {
        init(MAXX, MAXY);
    }

    void init(int MAXX, int MAXY) {
        this->MAXX = MAXX;
        this->MAXY = MAXY;

        int N = MAXX + MAXY;
        factorial.resize(N + 1);
        factInverse.resize(N + 1);
        inverse.resize(N + 1);

        inverse[0] = 0;
        inverse[1] = 1;
        for (int i = 2; i <= N; i++)
            inverse[i] = int((mod - 1ll * (mod / i) * inverse[mod % i] % mod) % mod);

        factorial[0] = 1;
        factInverse[0] = 1;

        factorial[1] = 1;
        factInverse[1] = 1;

        for (int i = 2; i <= N; i++) {
            factorial[i] = int(1ll * factorial[i - 1] * i % mod);
            factInverse[i] = int(1ll * factInverse[i - 1] * inverse[i] % mod);
        }
    }

    // initialCells = { (x0, y0), (x1, y1), ... }, 0 <= x <= MAXX, 0 <= y <= MAXY
    // return (the min size of set, the number of set)
    // O(N*logN)
    pair<long long, long long> calcMinSizeAndCount(const vector<pair<int, int>>& initialCells) const {
        vector<pair<int, int>> rangeY(MAXX + 1, make_pair(INF, -INF));  // rangeY[x] = { minY, maxY }
        vector<vector<int>> YofX(MAXX + 1);     // YofX[x] = { y0, y1, ... }

        vector<int> X;
        vector<int> countY(MAXY + 1);
        for (auto& pt : initialCells) {
            rangeY[pt.first].first = min(rangeY[pt.first].first, pt.second);
            rangeY[pt.first].second = max(rangeY[pt.first].second, pt.second);
            YofX[pt.first].push_back(pt.second);
            X.push_back(pt.first);
            countY[pt.second]++;
        }
        sort(X.begin(), X.end());
        X.erase(unique(X.begin(), X.end()), X.end());

        // set current rangeY for first column
        pair<int, int> curr = rangeY[X[0]];
        long long minSetSize = 0;
        long long countWays = 1;

        int minY = 0, maxY = MAXX;
        for (int i = 0; i < int(X.size()); i++) {
            minSetSize += curr.second - curr.first + 1;
            if (i + 1 >= int(X.size()))
                break;

            // remove Ys of current X
            for (int y : YofX[X[i]])
                countY[y]--;
            while (countY[minY] <= 0)
                minY++;
            while (countY[maxY] <= 0)
                maxY--;

            int lo = curr.first;
            int hi = curr.second;

            if (maxY < lo) {
                // case #1
                /*
                      hi +
                         |
                    curr |
                         |
                      lo + . . . . . . . . . . . . . . +
                                                       |
                                 *------- maxY         | curr'
                                 +                     |
                                 |                     |
                                 + . . . . . . . . . . +

                                 -------- minY

                        X[i]    X[i+1]
                */
                minSetSize += X[i + 1] - X[i] - 1;
                countWays = countWays * comb(X[i + 1] - X[i] + lo - maxY, X[i + 1] - X[i]) % mod;
                curr = { rangeY[X[i + 1]].first, lo };
            } else if (minY > hi) {
                // case #2
                /*
                                 -------- maxY
                                 + . . . . . . . . . . +
                                 |                     |
                                 +                     |
                                                       | curr'
                                 *------- minY         |
                                                       |
                      hi + . . . . . . . . . . . . . . +
                         |
                    curr |
                         |
                      lo +

                        X[i]    X[i+1]
                */
                minSetSize += X[i + 1] - X[i] - 1;
                countWays = countWays * comb(X[i + 1] - X[i] + minY - hi, X[i + 1] - X[i]) % mod;
                curr = { hi, rangeY[X[i + 1]].second };
            } else {
                // case #3
                /*
                 #3-1)
                                 ------- maxY
                                 + . . . . . . . . . . +
                                 |                     |
                                 +                     | curr'
                      hi +.......                      |
                         |///////                      |
                         |.......------ minY . . . . . +
                    curr |
                         |
                      lo +

                        X[i]    X[i+1]


                 #3-2)

                      hi +
                         |
                    curr |
                         |.......------- maxY  . . . . +
                         |///////                      |
                      lo +.......                      | curr'
                                 +                     |
                                 |                     |
                                 + . . . . . . . . . . +
                                 ------ minY

                        X[i]    X[i+1]


                 #3-3)

                                 ------- maxY
                                 + . . . . . . . . . . +
                      hi +.......|                     |
                         |///////|                     | curr'
                    curr |///////|                     |
                         |///////+                     |
                      lo +.......  . . . . . . . . . . +
                                 ------ minY

                        X[i]    X[i+1]


                 #3-4)

                      hi +
                         |
                         |.......------- maxY  . . . . +
                         |///////+                     |
                         |///////|                     |
                         |///////|                     | curr'
                    curr |///////|                     |
                         |///////+                     |
                         |///////                      |
                         |.......------ minY . . . . . +
                         |
                      lo +

                        X[i]    X[i+1]
                */
                minSetSize += 1ll * (min(hi, maxY) - max(lo, minY) + 1) * (X[i + 1] - X[i] - 1);
                curr = { min(max(lo, minY), rangeY[X[i + 1]].first), max(min(hi, maxY), rangeY[X[i + 1]].second) };
            }
        }

        return make_pair(minSetSize, countWays);
    }

private:
    vector<int> factorial;      // factorial
    vector<int> factInverse;    // inverse factorial
    vector<int> inverse;

    int comb(int n, int r) const {
        if (n < r)
            return 0;

        if (n == 0 || r == 0 || n == r)
            return 1;

        if (n - r < r)
            r = n - r;

        return int(1ll * factorial[n] * factInverse[n - r] % mod * factInverse[r] % mod);
    }
};
