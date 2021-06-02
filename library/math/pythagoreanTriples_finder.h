#pragma once

/*
  "Pythagorean Right-Angled Triangles"
    http://www.maths.surrey.ac.uk/hosted-sites/R.Knott/Pythag/pythag.html
    https://www.mathsisfun.com/numbers/pythagorean-triples.html
      a = m^2 - n^2
      b = 2mn
      c = m^2 + n^2
*/
struct IntegerSidedPythagoreanTripleFinder {
    static vector<tuple<long long, long long, long long>> findTriplesWithOneSide(long long X) {
        auto factors = getAllFactors(X);
        sort(factors.begin(), factors.end());

        vector<tuple<long long, long long, long long>> triples;
        findC(X, factors, triples);

        //---

        // factors of X^2
#ifdef __GNUC__
        vector<long long> factors2;
        for (auto f1 : factors) {
            for (auto f2 : factors) {
                auto t = __int128_t(f1) * f2;
                if (t >= X)
                    break;
                factors2.push_back(t);
            }
        }

        set<tuple<__int128_t, __int128_t, __int128_t>> triplesAB;
#else
        vector<long long> factors2;
        for (auto f1 : factors) {
            for (auto f2 : factors) {
                auto t = f1 * f2;
                if (t >= X)
                    break;
                factors2.push_back(t);
            }
        }

        set<tuple<long long, long long, long long>> triplesAB;
#endif
        findAB(X, factors2, triplesAB);

        for (auto& it : triplesAB)
            triples.push_back(it);

        return triples;
    }

private:
    // a = d * (m^2 - n^2)
    // b = d * 2mn
#ifdef __GNUC__
    static void findAB(long long X, const vector<long long>& factors2, set<tuple<__int128_t, __int128_t, __int128_t>>& triples) {
        for (auto d : factors2) {
            __int128_t p = __int128_t(X) * X / d;
            __int128_t q = d;

            __int128_t a = X;
            __int128_t b = (p - q) >> 1;
            __int128_t c = (p + q) >> 1;

            if (a * a + b * b == c * c) {
                if (a > b)
                    swap(a, b);
                triples.emplace(a, b, c);
            }
        }
    }
#else
    static void findAB(long long X, const vector<long long>& factors2, set<tuple<long long, long long, long long>>& triples) {
        for (auto d : factors2) {
            long long p = X * X / d;
            long long q = d;

            long long a = X;
            long long b = (p - q) >> 1;
            long long c = (p + q) >> 1;

            if (a * a + b * b == c * c) {
                if (a > b)
                    swap(a, b);
                triples.emplace(a, b, c);
            }
        }
    }
#endif

    // c = d * (m^2 + n^2)
    static void findC(long long X, const vector<long long>& factors, vector<tuple<long long, long long, long long>>& triples) {
        unordered_set<long long> S;

        for (auto d : factors) {
            auto c = X / d;

            for (long long n = 1; n * n < c; n++) {
                long long m = static_cast<long long>(sqrt(c - n * n));
                if (m <= n)
                    break;
                if ((m * m + n * n) == c && gcd(m, n) == 1) {
                    auto a = d * (m * m - n * n);
                    auto b = d * 2ll * m * n;
                    if (a > b)
                        swap(a, b);

                    if (S.find(a) == S.end()) {
                        triples.emplace_back(a, b, X);
                        S.insert(a);
                    }
                }
            }
        }
    }

    //---

    static vector<long long> getAllFactors(long long x) {
        vector<long long> res;
        if (x <= 1)
            return res;

        int sqrtX = int(sqrt(x));
        for (int i = 1; i <= sqrtX; i++) {
            if (x % i == 0) {
                long long j = x / i;
                if (i < j) {
                    res.emplace_back((long long)(i));
                    res.emplace_back(j);
                } else if (i == j) {
                    res.emplace_back(j);
                }
            }
        }

        return res;
    }

    template <typename T>
    static T gcd(T p, T q) {
        return q == 0 ? p : gcd(q, p % q);
    }
};
