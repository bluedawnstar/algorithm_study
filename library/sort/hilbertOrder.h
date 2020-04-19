#pragma once

template <int LogN = 20>
struct HilbertOrder {
    static const int MaxN = 1 << LogN;

    static long long get(int x, int y, int pow = LogN, int rotate = 0) {
        if (pow == 0)
            return 0;

        int hpow = 1 << (pow - 1);
        int seg = (x < hpow) ? ((y < hpow) ? 0 : 3)
                             : ((y < hpow) ? 1 : 2);
        seg = (seg + rotate) & 3;
        const int rotateDelta[4] = { 3, 0, 0, 1 };
        int nx = x & (x ^ hpow);
        int ny = y & (y ^ hpow);
        int nrot = (rotate + rotateDelta[seg]) & 3;
        long long subSquareSize = 1ll << (2 * pow - 2);
        long long ans = seg * subSquareSize;
        long long add = get(nx, ny, pow - 1, nrot);
        ans += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
        return ans;
    }

    static long long get2(int x, int y) {
        long long d = 0;
        for (int s = LogN - 1; s >= 0; s--) {
            int rx = (x >> s) & 1;
            int ry = (y >> s) & 1;
            d = (d << 2) | ((rx * 3) ^ ry);
            if (!ry) {
                if (rx) {
                    x = MaxN - x;
                    y = MaxN - y;
                }
                swap(x, y);
            }
        }
        return d;
    }
};
