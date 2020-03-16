#pragma once

// https://e-maxx-eng.appspot.com/algebra/discrete-log.html

// find x satisfying  a^x = b (mod mod), a and mod are coprime
struct DiscreteLog {
    // mod is 64bit integer, O(sqrt(mod))
    static long long solve64bit(long long a, long long b, long long mod) {
        int n = int(ceil(sqrt(mod))) + 1;

        long long an = 1;
        for (int i = 0; i < n; i++) {
            if (an == b)
                return i;
            an = modMul(an, a, mod);
        }
        {
            // try to find smallest value
            auto t = an;
            for (int i = n, maxi = int(min(10000ll, mod)); i <= maxi; i++) {
                if (t == b)
                    return i;
                t = modMul(t, a, mod);
            }
        }

        // baby-step
        unordered_map<long long, int> vals;
        long long cur = b;
        for (int i = 0; i <= n; i++) {
            if (vals.find(cur) == vals.end())
                vals[cur] = i;
            cur = modMul(cur, a, mod);
        }

        // giant-step
        cur = an;
        for (int i = 1; i <= n; i++) {
            auto it = vals.find(cur);
            if (it != vals.end())
                return 1ll * n * i - it->second;
            cur = modMul(cur, an, mod);
        }

        return -1;
    }

    // mod is 32bit integer, O(sqrt(mod))
    static int solve32bit(long long a, long long b, int mod) {
        int am = int(a % mod);
        int bm = int(b % mod);
        int n = int(ceil(sqrt(mod))) + 1;

        long long an = 1;
        for (int i = 0; i < n; i++) {
            if (an == bm)
                return i;
            an = (an * am) % mod;
        }
        {
            // try to find smallest value
            auto t = an;
            for (int i = n, maxi = min(100, mod); i <= maxi; i++) {
                if (t == bm)
                    return i;
                t = (t * am) % mod;
            }
        }

        // baby-steps
        unordered_map<long long, int> vals;
        long long cur = bm;
        for (int i = 0; i <= n; i++) {
            if (vals.find(cur) == vals.end())
                vals[cur] = i;
            cur = (cur * am) % mod;
        }

        // giant-steps
        cur = an;
        for (int i = 1; i <= n; i++) {
            auto it = vals.find(cur);
            if (it != vals.end())
                return n * i - it->second;
            cur = (cur * an) % mod;
        }

        return -1;
    }
};
