#pragma once

// Given a grid of size NxM.Find number of ways to fill the grid with figures of size 2x1
// (no cell should be left unfilled, and figures should not overlap each other).

// https://cp-algorithms.com/dynamic_programming/profile-dynamics.html
// https://blog.evilbuggy.com/2018/05/broken-profile-dynamic-programming.html

/*
  1. row * col is odd
     --> 0
  2. row == 1
     f(1, col) = col / 2
  3. row == 2
     f(2, col) = f(2, col - 1) + f(2, col - 2)
         f(2, 0) = 1
         f(2, 1) = 1
         f(2, 2) = 2
  4. row == 3
     f(3, col) = 4*f(3, col - 2) - f(3, col - 4)
         f(3, 0) = 1
         f(3, 2) = 3
         f(3, 4) = 11
     // https://math.stackexchange.com/questions/1317885/dominos-2-times-1-on-2-times-n-and-on-3-times-2n
  5. row == 4
     f(4, col) = f(n - 1) + 5*f(n - 2) + f(n - 3) - f(n - 4)
         f(4, 0) = 1
         f(4, 1) = 1
         f(4, 2) = 5
         f(4, 3) = 11
         f(4, 4) = 36
     // https://math.stackexchange.com/questions/664113/count-the-ways-to-fill-a-4-times-n-board-with-dominoes
*/

/*
   Case #1:
              x
            O O #
         y  O O .  <- skip
       y+1  O # .
        
   Case #2:
              x              x
            O O #          O O #
         y  O . .  =>    y O O O  <- put OO
       y+1  O # .      y+1 O # .

   Case #3:
              x              x
            O O #          O O #
         y  O . .  =>    y O O .  <- put O
       y+1  O . .      y+1 O O .         O
*/
struct BrokenProfile2x1 {
    int W;  // width
    int H;  // height
    vector<vector<long long>> dp;   // dp[(i)th column][profile of (i)th column]

    // O(W*H*2^H)
    long long countWays(int W, int H) {
        if ((1ll * W * H) & 1ll)
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
                dfs(x, y + 1, currMask, nextMask);          // case #1
            else {
                dfs(x, y + 1, currMask, nextMask | myMask); // case #2
                if (y + 1 < H && !(currMask & (myMask << 1)))
                    dfs(x, y + 2, currMask, nextMask);      // case #3
            }
        }
    }
};


/*
   Case #1:
             x
           O O # # 
        y  O O # .  <- skip
      y+1  O # # .

   Case #2:
             x               x
           O O # #         O O # #
        y  O . . . =>    y O O O O <- put OOO
      y+1  O # # .     y+1 O # # .

   Case #3:
             x                x
           O O # #          O O # #
        y  O . . .  =>    y O O . . <- put O
      y+1  O . . .      y+1 O O . .        O
      y+2  O . . .      y+2 O O . .        O
*/
struct BrokenProfile3x1 {
    int W;  // width
    int H;  // height
    vector<vector<vector<long long>>> dp;   // dp[(i)th column][profile of (i)th column][profile of (i + 1)th column]

    // O(W*H*3^H)
    long long countWays(int W, int H) {
        if ((1ll * W * H) % 3)
            return 0ll;

        if (W < H)
            swap(W, H);

        this->W = W;
        this->H = H;

        dp.assign(W + 1, vector<vector<long long>>(1 << H, vector<long long>(1 << H)));

        dp[0][0][0] = 1;
        for (int x = 0; x < W; ++x) {
            for (int currMask = 0; currMask < (1 << H); ++currMask) {
                for (int subset = currMask; subset; subset = (subset - 1) & currMask)
                    dfs(x, 0, currMask, subset ^ currMask, subset ^ currMask, 0);
                dfs(x, 0, currMask, currMask, currMask, 0);
            }
        }

        return dp[W][0][0];
    }

private:
    void dfs(int x, int y, int currMask1, int currMask2, int nextMask1, int nextMask2) {
        if (x == W)
            return;

        if (y >= H) {
            dp[x + 1][nextMask1][nextMask2] += dp[x][currMask1][currMask2];
        } else {
            int myMask = 1 << y;
            if (currMask1 & myMask)
                dfs(x, y + 1, currMask1, currMask2, nextMask1, nextMask2);                   // case #1
            else {
                dfs(x, y + 1, currMask1, currMask2, nextMask1 | myMask, nextMask2 | myMask); // case #2
                if (y + 2 < H && !(currMask1 & (myMask << 1)) && !(currMask1 & (myMask << 2)))
                    dfs(x, y + 3, currMask1, currMask2, nextMask1, nextMask2);               // case #3
            }
        }
    }
};
