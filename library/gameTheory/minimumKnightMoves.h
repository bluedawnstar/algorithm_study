#pragma once

// In an infinite chess board,
//   the minimum number of steps needd to move the knight from (0, 0) to (x, y)

struct MinimumKnightMove {
    static const int INF = 0x3f3f3f3f;

    //--- BFS, O(n^2)

    static int calculateMinStepsBFS(int x, int y) {
        x = abs(x);
        y = abs(y);

        int maxx = abs(x) + 2;
        int maxy = abs(y) + 2;

        vector<vector<int>> dp(maxy + 1, vector<int>(maxx + 1, INF));

        static int dxdy[8][2] = {
            {  1, -2 }, {  1, 2 },
            { -1, -2 }, { -1, 2 },
            {  2, -1 }, {  2, 1 },
            { -2, -1 }, { -2, 1 },
        };

        priority_queue<tuple<int,int,int>> Q; // (dist, x, y)
        dp[0][0] = 0;
        Q.emplace(0, 0, 0);

        while (!Q.empty()) {
            int d, xx, yy;
            tie(d, xx, yy) = Q.top();
            Q.pop();

            d = -d;

            if (xx == x && yy == y)
                return d;

            for (int i = 0; i < 8; i++) {
                int x2 = xx + dxdy[i][0];
                int y2 = yy + dxdy[i][1];
                if (x2 < 0 || x2 > maxx || y2 < 0 || y2 > maxy || d + 1 >= dp[y2][x2])
                    continue;
                dp[y2][x2] = d + 1;
                Q.emplace(-(d + 1), x2, y2);
            }
        }

        return dp[y][x];
    }


    //--- DFS, O(n^2)

    static int dfs(vector<vector<int>>& dp, int x, int y) {
        if (x >= int(dp[0].size()) || y >= int(dp.size()))
            return INF;
        else if (dp[y][x] != INF)
            return dp[y][x];
        return dp[y][x] = min(dfs(dp, abs(x - 2), abs(y - 1)), dfs(dp, abs(x - 1), abs(y - 2))) + 1;
    }

    static int calculateMinStepsDFS(int x, int y) {
        int maxx = abs(x) + 2;
        int maxy = abs(y) + 2;

        vector<vector<int>> dp(maxy + 1, vector<int>(maxx + 1, INF));
        dp[0][0] = 0;
        dp[0][1] = 3;
        dp[1][0] = 3;
        return dfs(dp, abs(x), abs(y));
    }


    //--- math, O(1)

    static int calculateMinSteps(int x, int y) {
        x = abs(x);
        y = abs(y);

        // symmetry for diagonal
        if (x < y)
            swap(x, y);

        if (x == 1 && y == 0)
            return 3;
        else if (x == 2 && y == 2)
            return 4;

        int delta = x - y;
        if (y > delta)
            //return delta + 2 * ceil((y - delta) / 3.0);
            return delta + 2 * ((y - delta + 2) / 3);
        else
            return delta - 2 * ((delta - y) / 4);
    }
};
