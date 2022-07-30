#pragma once

// https://www.hackerearth.com/problem/algorithm/modulus-of-grid-b690bf11/

// for the case where N is fixed
struct CircularMoveDP {
    int N;
    vector<int> dp;

    CircularMoveDP() : N(0) {
    }

    explicit CircularMoveDP(int N) {
        prepare(N);
    }

    // O(N*loglogN)
    void prepare(int N) {
        this->N = N;

        vector<int> cnt(N);
        for (int jumpStep = 1; jumpStep < N; jumpStep++) {
            cnt[gcd(jumpStep, N)]++;
        }

        dp = vector<int>(N);
        for (int jumpStep = 1; jumpStep < N; jumpStep++) {
            for (int i = 0; i < N; i += jumpStep) {
                dp[i] += cnt[jumpStep];
            }
        }
    }

    // jump_step * a + srcX = dstX (mod N) ,   (1 <= jump_step < N, 0 <= c,  0 <= srcX, dstX < N)
    // return the number of jump_step

    //                     jump_step * a = N * b + dstX - srcX
    // <=>         jump_step * a + N * b = dstX - srcX
    // <=>         jump_step * a + N * b = gcd(jump_step, N) * c
    // 
    // ==>         dstX - srcX = gcd(jump_step, N) * c
    // 
    int countJumpSteps(int srcX, int dstX) const {
        return dp[abs(dstX - srcX)];
    }

private:
    static int gcd(int p, int q) {
        return q == 0 ? p : gcd(q, p % q);
    }
};
