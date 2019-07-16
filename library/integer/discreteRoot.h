#pragma once

// https://cp-algorithms.com/algebra/discrete-root.html

// find x satisfying  x^a = b (mod mod), mod is a prime number
template <typename T>
struct DiscreteRoot {
    static T rootAny(T a, T b, T mod) {
        if (a == 0)
            return -1;
        T g = generator(mod);
        T sq = T(sqrt(mod)) + 1;

        vector<pair<T, T>> dec(sq);
        for (T i = 1; i <= sq; i++)
            dec[i - 1] = make_pair(modPow(g, T(1ll * i * sq * a % (mod - 1)), mod), i);
        sort(dec.begin(), dec.end());

        for (T i = 0; i < sq; i++) {
            T my = T(1ll * modPow(g, T(1ll * i * a % (mod - 1)), mod) * b % mod);
            auto it = lower_bound(dec.begin(), dec.end(), make_pair(my, T(0)));
            if (it != dec.end() && it->first == my) {
                //T x = it->second * sq - i;
                //T delta = (mod - 1) / gcd(a, mod - 1);
                //return modPow(g, x % delta, mod);
                return it->second * sq - i;
            }
        }
        return T(-1);
    }

    static vector<T> rootAll(T a, T b, T mod) {
        if (a == 0)
            return{};
        T g = generator(mod);
        T sq = T(sqrt(mod)) + 1;

        vector<pair<T, T>> dec(sq);
        for (T i = 1; i <= sq; i++)
            dec[i - 1] = make_pair(modPow(g, T(1ll * i * sq * a % (mod - 1)), mod), i);
        sort(dec.begin(), dec.end());

        for (int i = 0; i < sq; i++) {
            T my = T(1ll * modPow(g, T(1ll * i * a % (mod - 1)), mod) * b % mod);
            auto it = lower_bound(dec.begin(), dec.end(), make_pair(my, T(0)));
            if (it != dec.end() && it->first == my) {
                T x = it->second * sq - i;
                T delta = (mod - 1) / gcd(a, mod - 1);

                vector<T> res;
                for (T cur = x % delta; cur < mod - 1; cur += delta)
                    res.push_back(modPow(g, cur, mod));
                sort(res.begin(), res.end());

                return res;
            }
        }
        return{};
    }

private:
    static T gcd(T p, T q) {
        return q == 0 ? p : gcd(q, p % q);
    }

    // finds the primitive root modulo p
    static T generator(T mod) {
        if (mod == 2)
            return 1;
        vector<T> factors;

        T phi = mod - 1;
        T n = phi;
        for (T i = 2; i * i <= n; ++i) {
            if (n % i == 0) {
                factors.push_back(i);
                while (n % i == 0)
                    n /= i;
            }
        }
        if (n > 1)
            factors.push_back(n);

        for (T res = 2; res <= mod; res++) {
            if (gcd(res, mod) != 1)
                continue;

            bool ok = true;
            for (T f : factors) {
                if (modPow(res, phi / f, mod) == 1) {
                    ok = false;
                    break;
                }
            }

            if (ok)
                return res;
        }

        return -1;
    }

    static T modPow(T x, T n, T mod) {
        if (n == 0)
            return 1;

        long long t = x % mod;
        long long res = 1;
        for (; n > 0; n >>= 1) {
            if (n & 1)
                res = res * t % mod;
            t = t * t % mod;
        }
        return T(res);
    }
};
