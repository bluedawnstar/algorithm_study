#include <vector>
#include <algorithm>

using namespace std;

#include "brokenProfileEx.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static long long dfsTilingL(vector<vector<bool>>& board, int index) {
    int H = (int)board.size();
    int W = (int)board[0].size();
    while (index < H * W) {
        int r = index / W;
        int c = index % W;

        if (!board[r][c]) {
            long long res = 0;
            /*if (c < W - 1 && r > 0 && !board[r][c + 1] && !board[r - 1][c + 1]) {
                board[r][c] = board[r][c + 1] = board[r - 1][c + 1] = true;
                res += dfsTilingL(board, index + 1);
                board[r][c] = board[r][c + 1] = board[r - 1][c + 1] = false;
            }*/
            if (c < W - 1 && r + 1 < H && !board[r][c + 1] && !board[r + 1][c + 1]) {
                board[r][c] = board[r][c + 1] = board[r + 1][c + 1] = true;
                res += dfsTilingL(board, index + 1);
                board[r][c] = board[r][c + 1] = board[r + 1][c + 1] = false;
            }
            if (c < W - 1 && r < H - 1 && !board[r + 1][c] && !board[r][c + 1]) {
                board[r][c] = board[r + 1][c] = board[r][c + 1] = true;
                res += dfsTilingL(board, index + 1);
                board[r][c] = board[r + 1][c] = board[r][c + 1] = false;
            }
            if (c < W - 1 && r < H - 1 && !board[r + 1][c] && !board[r + 1][c + 1]) {
                board[r][c] = board[r + 1][c] = board[r + 1][c + 1] = true;
                res += dfsTilingL(board, index + 1);
                board[r][c] = board[r + 1][c] = board[r + 1][c + 1] = false;
            }
            if (c > 0 && r < H - 1 && !board[r + 1][c] && !board[r + 1][c - 1]) {
                board[r][c] = board[r + 1][c] = board[r + 1][c - 1] = true;
                res += dfsTilingL(board, index + 1);
                board[r][c] = board[r + 1][c] = board[r + 1][c - 1] = false;
            }
            return res;
        }

        index++;
    }

    return 1;
}

static long long countTilingL(int W, int H) {
    vector<vector<bool>> board(H, vector<bool>(W));
    return dfsTilingL(board, 0);
}

void testBrokenProfileEx() {
    //return; //TODO: if you want to test, make this line a comment.

    cout << "--- Broken Profile - Extended Algorithms -----------------------------" << endl;
    {
        int W = 8;
        int H = 6;
#ifndef _DEBUG
        W = 12;
        H = 6;
#endif

        BrokenProfileTileL dp;

        PROFILE_START(0);
        long long ans = dp.countWays(W, H);
        PROFILE_STOP(0);

        PROFILE_START(1);
        long long gt = countTilingL(W, H);
        PROFILE_STOP(1);

        if (ans != gt)
            cout << "Mismatched : " << ans << " != " << gt << endl;
        else
            cout << ans << endl;
        assert(ans == gt);
    }
    {
        int W = 12;
        int H = 6;
#ifndef _DEBUG
        W = 21;
        H = 10;
#endif

        BrokenProfileTileL dp;

        PROFILE_START(0);
        long long ans = dp.countWays(W, H);
        PROFILE_STOP(0);

        if (ans < 0)
            cout << "This message is to prevent optimization" << endl;
    }

    cout << "OK!" << endl;
}
