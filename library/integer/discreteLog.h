#pragma once

// https://e-maxx-eng.appspot.com/algebra/discrete-log.html

// find x satisfying  a^x = b (mod mod), a and mod are coprime
struct DiscreteLog {
    // mod is 64bit integer, O(sqrt(mod))
    static long long solve64bit(long long a, long long b, long long mod) {
        int n = int(ceil(sqrt(mod))) + 1;

        long long an = 1;
        for (int i = 0; i < n; i++)
            an = modMul(an, a, mod);

        unordered_map<long long, long long> vals;
        long long cur = an;
        for (int i = 1; i <= n; i++) {
            if (vals.find(cur) == vals.end())
                vals[cur] = i;
            cur = modMul(cur, an, mod);
        }

        cur = b;
        for (int i = 0; i <= n; i++) {
            if (vals.find(cur) != vals.end()) {
                long long ans = vals[cur] * n - i;
                if (ans < mod)
                    return ans;
            }
            cur = modMul(cur, a, mod);
        }

        return -1;
    }

    // mod is 32bit integer, O(sqrt(mod))
    static int solve32bit(long long a, long long b, int mod) {
        int am = int(a % mod);
        int bm = int(b % mod);
        int n = int(ceil(sqrt(mod))) + 1;

        long long an = 1;
        for (int i = 0; i < n; i++)
            an = (an * am) % mod;

        unordered_map<long long, int> vals;
        long long cur = an;
        for (int i = 1; i <= n; i++) {
            if (vals.find(cur) == vals.end())
                vals[cur] = i;
            cur = (cur * an) % mod;
        }

        cur = bm;
        for (int i = 0; i <= n; i++) {
            if (vals.find(cur) != vals.end()) {
                int ans = vals[cur] * n - i;
                if (ans < mod)
                    return ans;
            }
            cur = (cur * am) % mod;
        }

        return -1;
    }
};
