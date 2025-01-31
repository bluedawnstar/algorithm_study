#pragma once

// https://www.acmicpc.net/problem/6567
// https://www.acmicpc.net/problem/16052

template <int mod = 1'000'000'007>
struct ColoringNecklaceMod {
    /*
       The number of ways to make unique necklaces, O(N*loglogN)
       - N : the number of beads
       - colorCount : the number of colors
       - flipAllowed : indicates whether flipping is allowed
    */
    static int count(int N, int colorCount, bool flipAllowed) {
        int ans = 0;
        int G = N;
        for (int i = 0; i < N; i++) {
            ans = modAdd(ans, modPow(colorCount, gcd(i, N)));
        }

        if (flipAllowed) {
            if (N & 1) {
                ans = modAdd(ans, modMul(N, modPow(colorCount, N / 2 + 1)));
                G += N;
            } else {
                ans = modAdd(ans, modMul(N / 2, modPow(colorCount, N / 2 + 1)));
                G += N / 2;

                ans = modAdd(ans, modMul(N / 2, modPow(colorCount, N / 2)));
                G += N / 2;
            }
        }

        return modDiv(ans, G);
    }

    //---

    static int modPow(int x, int n) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return int(res);
    }

    static int modMul(int a, int b) {
        return int(1ll * a * b % mod);
    }

    static int modDiv(int a, int b) {
        return int(1ll * a * modPow(b, mod - 2) % mod);
    }

    static int modAdd(int a, int b) {
        a += b;
        if (a >= mod)
            a -= mod;
        return a;
    }

    static int gcd(int a, int b) {
        return (b == 0) ? a : gcd(b, a % b);
    }
};
