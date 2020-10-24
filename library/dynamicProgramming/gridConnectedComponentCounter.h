#pragma once

/*
  Grid Connected Components
  - https://www.hackerearth.com/challenges/competitive/october-circuits-20/algorithm/expectation-2-4eb0b2e0/

  1. Question
    - N = the number of columns
    - M = the number of rows (=2)
    - color = block and white (2 colors)
       +---+---+---+---+---+---+
       |   |   |   |   |  ...  |
       +---+---+---+---+---+---+
       |   |   |   |   |  ...  |
       +---+---+---+---+---+---+

    1) every cell is painted by black or white
    2) two cells are connected if they share a side and both are painted in the same color

    3) the number of all connected components in all combinations

  2. Solution
    ...
*/
template <int mod = 1'000'000'007>
struct GridConnectedComponentCounter2xN {
    static int count(long long N) {
        // [log(n)][left-side B/W][right-side B/W] = { black comp-count, white comp-count, combination }
        static int sDP[64][4][4][3];
        static bool initialized = false;

        if (!initialized) {
            sDP[0][0][0][0] = 1;
            sDP[0][0][0][1] = 0;
            sDP[0][0][0][2] = 1;

            sDP[0][1][1][0] = 1;
            sDP[0][1][1][1] = 1;
            sDP[0][1][1][2] = 1;

            sDP[0][2][2][0] = 1;
            sDP[0][2][2][1] = 1;
            sDP[0][2][2][2] = 1;

            sDP[0][3][3][0] = 0;
            sDP[0][3][3][1] = 1;
            sDP[0][3][3][2] = 1;

            for (int i = 1; i < 64; i++)
                multiply(sDP[i], sDP[i - 1], sDP[i - 1]);

            initialized = true;
        }

        int dp[2][4][4][3];
        memset(dp, 0, sizeof(dp));

        bool first = true;
        int from = 0, to = 1;
        for (int i = 0; N; i++, N >>= 1) {
            if (N & 1) {
                if (first) {
                    memcpy(dp[to], sDP[i], sizeof(sDP[i]));
                    first = false;
                } else {
                    multiply(dp[to], dp[from], sDP[i]);
                }
                swap(from, to);
            }
        }

        int res = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                res += dp[from][i][j][0];
                if (res >= mod)
                    res -= mod;
                res += dp[from][i][j][1];
                if (res >= mod)
                    res -= mod;
            }
        }

        return res;
    }

private:
    // [left-side B/W][right-side B/W][0] = black components
    // [left-side B/W][right-side B/W][1] = white components
    // [left-side B/W][right-side B/W][2] = the number of combinations
    static void multiply(int out[4][4][3], const int L[4][4][3], const int R[4][4][3]) {
        // [L][R][black(0)/white(1)] = delta
        static int delta[4][4][2]{
            {
                { -1,  0 }, // bb * bb
                { -1,  0 }, // bb * wb
                { -1,  0 }, // bb * bw
                {  0,  0 }  // bb * ww
            },
            {
                { -1,  0 }, // wb * bb
                { -1, -1 }, // wb * wb
                {  0,  0 }, // wb * bw
                {  0, -1 }, // wb * ww
            },
            {
                { -1,  0 }, // bw * bb
                {  0,  0 }, // bw * wb
                { -1, -1 }, // bw * bw
                {  0, -1 }, // bw * ww
            },
            {
                {  0,  0 }, // ww * bb
                {  0, -1 }, // ww * wb
                {  0, -1 }, // ww * bw
                {  0, -1 }, // ww * ww
            }
        };

        memset(out, 0, sizeof(out));
        for (int aL = 0; aL < 4; aL++) {
            for (int bR = 0; bR < 4; bR++) {
                for (int aR = 0; aR < 4; aR++) {
                    for (int bL = 0; bL < 4; bL++) {
                        int c = int(1ll * L[aL][aR][2] * R[bL][bR][2] % mod);
                        if (c == 0)
                            continue;

                        // out[aL][bR] += L[aL][aR].componentCount * R[bL][bR].combinationCount
                        //              + L[aL][aR].combinationCount * R[bL][bR].componentCount
                        //              + delta[aR][bL] * L[aL][aR].combinationCount * R[bL][bR].combinationCount
                        int d0 = int(1ll * L[aL][aR][0] * R[bL][bR][2] % mod + 1ll * L[aL][aR][2] * R[bL][bR][0] % mod
                                     + delta[aR][bL][0] * c);
                        if (d0 < 0)
                            d0 += mod;
                        else if (d0 >= mod)
                            d0 -= mod;
                        out[aL][bR][0] += d0;
                        if (out[aL][bR][0] >= mod)
                            out[aL][bR][0] -= mod;

                        // out[aL][bR] += L[aL][aR].componentCount * R[bL][bR].combinationCount
                        //              + L[aL][aR].combinationCount * R[bL][bR].componentCount
                        //              + delta[aR][bL] * L[aL][aR].combinationCount * R[bL][bR].combinationCount
                        int d1 = int(1ll * L[aL][aR][1] * R[bL][bR][2] % mod + 1ll * L[aL][aR][2] * R[bL][bR][1] % mod
                                     + delta[aR][bL][1] * c);
                        if (d1 < 0)
                            d1 += mod;
                        else if (d1 >= mod)
                            d1 -= mod;
                        out[aL][bR][1] += d1;
                        if (out[aL][bR][1] >= mod)
                            out[aL][bR][1] -= mod;

                        out[aL][bR][2] += c;
                        if (out[aL][bR][2] >= mod)
                            out[aL][bR][2] -= mod;
                    }
                }
            }
        }
    }
};
