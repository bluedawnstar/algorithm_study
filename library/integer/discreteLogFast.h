#pragma once

// from https://blog.csdn.net/Fallen_Breath/article/details/52758419
/*
https://www.spoj.com/problems/MOD/

       a^x = b (mod m)
  <=>  a^x = m*i + b
  <=>  a^x / g = m*i / g + b/g                       , g = gcd(a, m)
  <=>  a^x / g = b/g  (mod m/g)                      , g = gcd(a, m)

  <=>  a/g * a^(x-1) = b/g  (mod m/g)                , g = gcd(a, m)
  <=>  a/g * a/g' * a^(x-2) = (b/g)/g' (mod (m/g)/g')
                                                     , g = gcd(a, m), g' = gcd(a, m/g)
  <=>  a/g * a/g' * a/g'' * a^(x-2) = ((b/g)/g')/g'' (mod ((m/g)/g')/g'')
                                                     , g = gcd(a, m), g' = gcd(a, m/g), g'' = gcd(a, (m/g)/g')
       ...

  <=>  D * a^(x-cnt) = B (mod C)
*/
// find x satisfying  a^x = b (mod m), a and m need not be coprime!
struct FastDiscreteLog {
    const int N = 100000;
    const int mod = 99991;

    vector<int> val, id, stk;
    int top;

    FastDiscreteLog() : top(0) {
        val.assign(N, -1);
        id.resize(N);
        stk.resize(N);
    }

    // O(log(m) + sqrt(m))
    int solve(int a, int b, int m) {
        long long x = 1 % m, d = 1;
        for (int i = 0; i <= 100; i++) {
            if (x == b)
                return i;
            x = x * a % m;
        }

        int g, cnt = 0;
        while ((g = gcd(a, m)) != 1) {
            if (b % g != 0)
                return -1;
            b /= g;
            m /= g;
            d = d * (a / g) % m;
            cnt++;
        }

        int ans = bsgs(a, b, m, d);
        if (ans == -1)
            return -1;
        else
            return ans + cnt;
    }

private:
    static int gcd(int a, int b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    void clear() {
        memset(val.data(), -1, sizeof(val[0]) * val.size());
        top = 0;
    }

    void insert(int x, int y) {
        int h = x % mod;
        while (val[h] != -1 && val[h] != x) {
            if (++h >= mod)
                h -= mod;
        }
        val[h] = x;
        id[h] = y;
        stk[++top] = h;
    }

    int get(int x) {
        int h = x % mod;
        while (val[h] != -1 && val[h] != x) {
            if (++h >= mod)
                h -= mod;
        }
        if (val[h] == -1)
            return -1;
        return id[h];
    }

    // Baby-step giant-step, O(sqrt(C))
    int bsgs(int A, int B, int C, long long D) {
        long long m = static_cast<long long>(ceil(sqrt(C)));
        long long am = 1;
        clear();
        for (int i = 1; i <= m; i++) {
            am = (am * A) % C;
            insert(int(am * B % C), i);
        }
        for (int i = 1; i <= m; i++) {
            D = D * am % C;
            int j = get(int(D));
            if (j != -1)
                return int(i * m - j);
        }
        return -1;
    }
};
