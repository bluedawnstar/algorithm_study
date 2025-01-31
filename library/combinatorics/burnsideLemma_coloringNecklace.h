#pragma once

// https://www.acmicpc.net/problem/6567
// https://www.acmicpc.net/problem/9817
struct ColoringNecklace {
    /*
       The number of ways to make unique necklaces, O(N*loglogN)
       - N : the number of beads
       - colorCount : the number of colors
       - flipAllowed : indicates whether flipping is allowed
    */
    static long long count(int N, int colorCount, bool flipAllowed) {
        long long ans = 0;
        int G = N;
        for (int i = 0; i < N; i++)
            ans += pow(colorCount, gcd(i, N));

        if (flipAllowed) {
            if (N & 1) {
                ans += 1ll * N * pow(colorCount, N / 2 + 1);
                G += N;
            } else {
                ans += 1ll * (N / 2) * pow(colorCount, N / 2 + 1);
                G += N / 2;

                ans += 1ll * (N / 2) * pow(colorCount, N / 2);
                G += N / 2;
            }
        }

        return ans / G;
    }

    //---

    static long long pow(int x, int n) {
        if (n == 0)
            return 1;

        long long t = x;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t;
            t = t * t;
        }
        return res;
    }

    static int gcd(int a, int b) {
        return (b == 0) ? a : gcd(b, a % b);
    }
};
