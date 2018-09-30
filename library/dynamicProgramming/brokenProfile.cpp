#include <vector>
#include <algorithm>

using namespace std;

#include "brokenProfile.h"

/////////// For Testing ///////////////////////////////////////////////////////

#include <time.h>
#include <cassert>
#include <string>
#include <iostream>
#include "../common/iostreamhelper.h"
#include "../common/profile.h"

static long long dfsTiling2x1(vector<vector<bool>>& board, int index) {
    int H = (int)board.size();
    int W = (int)board[0].size();
    while (index < H * W) {
        int r = index / W;
        int c = index % W;

        if (!board[r][c]) {
            long long res = 0;
            if (c < W - 1 && !board[r][c + 1]) {
                board[r][c] = board[r][c + 1] = true;
                res += dfsTiling2x1(board, index + 1);
                board[r][c] = board[r][c + 1] = false;
            }
            if (r < H - 1 && !board[r + 1][c]) {
                board[r][c] = board[r + 1][c] = true;
                res += dfsTiling2x1(board, index + 1);
                board[r][c] = board[r + 1][c] = false;
            }
            return res;
        }

        index++;
    }

    return 1;
}

static long long countTiling2x1(int W, int H) {
    vector<vector<bool>> board(H, vector<bool>(W));
    return dfsTiling2x1(board, 0);
}


static long long dfsTiling3x1(vector<vector<bool>>& board, int index) {
    int H = (int)board.size();
    int W = (int)board[0].size();
    while (index < H * W) {
        int r = index / W;
        int c = index % W;

        if (!board[r][c]) {
            long long res = 0;
            if (c < W - 2 && !board[r][c + 1] && !board[r][c + 2]) {
                board[r][c] = board[r][c + 1] = board[r][c + 2] = true;
                res += dfsTiling3x1(board, index + 1);
                board[r][c] = board[r][c + 1] = board[r][c + 2] = false;
            }
            if (r < H - 2 && !board[r + 1][c] && !board[r + 2][c]) {
                board[r][c] = board[r + 1][c] = board[r + 2][c] = true;
                res += dfsTiling3x1(board, index + 1);
                board[r][c] = board[r + 1][c] = board[r + 2][c] = false;
            }
            return res;
        }

        index++;
    }

    return 1;
}

static long long countTiling3x1(int W, int H) {
    vector<vector<bool>> board(H, vector<bool>(W));
    return dfsTiling3x1(board, 0);
}

void testBrokenProfile() {
    return; //TODO: if you want to test, make this line a comment.

    cout << "--- Broken Profile -----------------------------" << endl;
    {
        int W = 8;
        int H = 5;
#ifndef _DEBUG
        W = 10;
        H = 6;
#endif

        BrokenProfile2x1 dp;

        long long ans = dp.countWays(W, H);
        long long gt = countTiling2x1(W, H);
        if (ans != gt)
            cout << "Mismatched : " << ans << " != " << gt << endl;
        assert(ans == gt);
    }
    {
        int W = 10;
        int H = 6;
#ifndef _DEBUG
        W = 12;
        H = 7;
#endif

        BrokenProfile3x1 dp;

        long long ans = dp.countWays(W, H);
        long long gt = countTiling3x1(W, H);
        if (ans != gt)
            cout << "Mismatched : " << ans << " != " << gt << endl;
        assert(ans == gt);
    }

    cout << "OK!" << endl;
}
