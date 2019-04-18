#pragma once

// Given a grid of size NxM.Find number of ways to fill the grid with figures of some sizes
// (no cell should be left unfilled, and figures should not overlap each other).

// https://cp-algorithms.com/dynamic_programming/profile-dynamics.html
// https://blog.evilbuggy.com/2018/05/broken-profile-dynamic-programming.html

/*
   Case #1:
              x
            O O #
         y  O O #  <- skip
       y+1  O # #

   Case #2:
     <#2-1>
              x              x
            O O .          O O O          O
         y  O . .  =>    y O O O  <- put OO
       y+1  O # .      y+1 O # .

     <#2-2>
              x              x
            O O #          O O #          
         y  O . .  =>    y O O O  <- put OO
       y+1  O # .      y+1 O # O          O

   Case #3:
     <#3-1>
              x              x
            O O #          O O #
         y  O . .  =>    y O O O  <- put OO
       y+1  O . .      y+1 O O .         O

     <#3-2>
              x              x
            O O #          O O #
         y  O . #  =>    y O O #  <- put O
       y+1  O . .      y+1 O O O         OO
*/
struct BrokenProfileTileL {
    int W;  // width
    int H;  // height
    vector<vector<long long>> dp;   // dp[(i)th column][profile of (i)th column]

    // O(W*H*2^H)
    long long countWays(int W, int H) {
        if ((1ll * W * H) % 3ll)
            return 0ll;

        if (W < H)
            swap(W, H);

        this->W = W;
        this->H = H;

        dp.assign(W + 1, vector<long long>(1 << H));

        dp[0][0] = 1;
        for (int x = 0; x < W; ++x)
            for (int currMask = 0; currMask < (1 << H); ++currMask)
                dfs(x, 0, currMask, 0);

        return dp[W][0];
    }

private:
    void dfs(int x, int y, int currMask, int nextMask) {
        if (x == W)
            return;

        if (y >= H)
            dp[x + 1][nextMask] += dp[x][currMask];
        else {
            int myMask = 1 << y;
            if (currMask & myMask)
                dfs(x, y + 1, currMask, nextMask);                      // case #1
            else {
                int myMask1 = myMask | (myMask >> 1);
                int myMask2 = myMask | (myMask << 1);

                if (y > 0 && (nextMask & myMask1) == 0)
                    dfs(x, y + 1, currMask, nextMask | myMask1);        // case #2-1
                if (y + 1 < H && (nextMask & myMask2) == 0)
                    dfs(x, y + 1, currMask, nextMask | myMask2);        // case #2-2

                if (y + 1 < H && (currMask & (myMask << 1)) == 0) {
                    if ((nextMask & myMask) == 0)
                        dfs(x, y + 2, currMask, nextMask | myMask);     // case #3-1
                    dfs(x, y + 2, currMask, nextMask | (myMask << 1));  // case #3-2
                }
            }
        }
    }
};
