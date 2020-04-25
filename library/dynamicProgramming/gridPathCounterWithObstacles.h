#pragma once

// the number of paths from (0, 0) to (width - 1, height - 1) in a grid with obstacles
// - It's allowed to move either right or down in a single move.

template <int mod = 1000000007>
struct GridPathCounterWithObstaclesMod {
    GridPathCounterWithObstaclesMod() {
    }

    GridPathCounterWithObstaclesMod(int maxW, int maxH) {
        init(maxW + maxH);
    }

    // O(N)
    void init(int N) {
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

    // the number of paths from (0, 0) to (x, y) without obstacles, O(1)
    long long countPath(int x, int y) {
        return comb(x + y, y);
    }

    // O(K^2), K = the number of obstacles
    // obstacles = { (x0, y0), (x1, y1), ... }
    long long countValidPath(int width, int height, vector<pair<int, int>> obstacles) {
        // sort obstacles by distance from (0, 0)
        sort(obstacles.begin(), obstacles.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return (a.first + a.second) < (b.first + b.second);
        });

        int ex = width - 1;
        int ey = height - 1;

        int n = int(obstacles.size());
        if (n == 0)
            return countPath(ex, ey);

        if (obstacles[0].first == 0 && obstacles[0].second == 0
            || obstacles.back().first == ex && obstacles.back().second == ey)
            return 0;

        long long res = 0;

        vector<long long> dp(n);
        for (int i = 0; i < n; i++) {
            long long s0 = countPath(obstacles[i].first, obstacles[i].second);
            long long s1 = countPath(ex - obstacles[i].first, ey - obstacles[i].second);

            for (int j = 0; j < i; j++) {
                if (obstacles[j].first > obstacles[i].first || obstacles[j].second > obstacles[i].second)
                    continue;

                s0 -= dp[j] * countPath(obstacles[i].first - obstacles[j].first, obstacles[i].second - obstacles[j].second) % mod;
                if (s0 >= mod)
                    s0 -= mod;
                else if (s0 < 0)
                    s0 += mod;
            }

            if (s0 == 0)
                continue;

            res += s0 * s1 % mod;
            if (res >= mod)
                res -= mod;
            else if (res < 0)
                res += mod;

            dp[i] = s0;
        }

        res = countPath(ex, ey) - res;
        if (res < 0)
            res += mod;

        return res;
    }


    // O(W*H), with obstacles
    long long countValidPathNaive(int width, int height, const vector<vector<bool>>& mat) {
        vector<vector<long long>> dp(height, vector<long long>(width));

        dp[0][0] = mat[0][0] ? 0 : 1;
        for (int i = 1; i < width; i++)
            dp[0][i] = mat[0][i] ? 0 : dp[0][i - 1];
        for (int i = 1; i < height; i++)
            dp[i][0] = mat[i][0] ? 0 : dp[i - 1][0];
        for (int i = 1; i < height; i++) {
            for (int j = 1; j < width; j++) {
                if (!mat[i][j]) {
                    dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
                    if (dp[i][j] >= mod)
                        dp[i][j] -= mod;
                }
            }
        }

        return dp[height - 1][width - 1];
    }

    // obstacles = { (x0, y0), (x1, y1), ... }
    long long countValidPathNaive(int width, int height, const vector<pair<int, int>>& obstacles) {
        vector<vector<bool>> mat(height, vector<bool>(width));
        for (auto& it : obstacles)
            mat[it.second][it.first] = true;
        return countValidPathNaive(width, height, mat);
    }

private:
    vector<int> factorial;      // factorial
    vector<int> factInverse;    // inverse factorial
    vector<int> inverse;

    // nCr, O(1)
    int comb(int n, int r) {
        if (n < r)
            return 0;

        if (n == 0 || r == 0 || n == r)
            return 1;

        if (n - r < r)
            r = n - r;

        return int(1ll * factorial[n] * factInverse[n - r] % mod * factInverse[r] % mod);
    }
};

// <Related problems>
// https://www.hackerearth.com/challenges/competitive/april-circuits-20/algorithm/avoid-corona-paths-32f67235/
