#pragma once

// Coloring a necklace with fixed positions on a circle
//   when only rotation is allowed and flipping is not permitted
// 
// https://www.acmicpc.net/problem/3835
// 
template <int mod = 1'000'000'007>
struct ColoringNecklaceNonUniformFixedPosition {
    // - angles     : sorted angle offset
    // - colorCount : the number of colors
    // - maxAngle   : 0 <= angle < maxAngle
    static int count(const vector<int>& angles, int colorCount, int maxAngle) {
        int N = int(angles.size());

        int ans = 0;
        auto rotationIndexes = findRotationPositions(angles, maxAngle);
        for (auto i : rotationIndexes) {
            ans += modPow(colorCount, gcd(i, N));   // SUM { colorCount^gcd(i,N) }
            if (ans >= mod)
                ans -= mod;
        }
        int G = int(rotationIndexes.size());

        return modDiv(ans, G);                      // ans / |G|
    }

    static vector<int> findRotationPositions(const vector<int>& angles, int maxAngle) {
        int N = int(angles.size());

        vector<int> rotationDelta(N * 2 - 1);
        for (int i = 1; i < N; i++) {
            rotationDelta[i - 1] = angles[i] - angles[i - 1];
            rotationDelta[i - 1 + N] = rotationDelta[i - 1];
        }
        rotationDelta[N - 1] = (angles[0] + maxAngle) - angles[N - 1];

        vector<int> res;
        auto pi = prefixFunction(rotationDelta, N);
        for (int i = N - 1, j = N - 1; i < N * 2 - 1; i++) {
            while (j > 0 && rotationDelta[i] != rotationDelta[j])
                j = pi[j - 1];

            if (rotationDelta[i] == rotationDelta[j]) {
                if (j == N - 1) {
                    res.push_back(i - N + 1);
                    j = pi[j];
                } else {
                    j++;
                }
            }
        }

        return res;
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

    static int modDiv(int a, int b) {
        return int(1ll * a * modPow(b, mod - 2) % mod);
    }

    static int gcd(int a, int b) {
        return (b == 0) ? a : gcd(b, a % b);
    }

    static vector<int> prefixFunction(const vector<int>& s, int n) {
        vector<int> pi(n);
        int j = 0;
        for (int i = 1; i < n; i++) {
            while (j > 0 && s[j] != s[i])
                j = pi[j - 1];

            if (s[j] == s[i])
                j++;
            pi[i] = j;
        }
        return pi;
    }
};
