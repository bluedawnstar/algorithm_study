#pragma once

struct SimplePrimeFactor {
    vector<int> primes;

    SimplePrimeFactor() {
    }

    explicit SimplePrimeFactor(int maxX) {
        init(maxX);
    }

    void init(int maxX) {
        int sqrtX = int(sqrt(maxX));
        primes.clear();

        vector<bool> p(sqrtX + 1, true);
        p[0] = false;
        p[1] = false;

        for (int j = 2 * 2; j <= sqrtX; j += 2)
            p[j] = false;

        int root = int(sqrt(sqrtX));
        for (int i = 3; i <= root; i += 2) {
            if (p[i]) {
                for (int j = i * i; j >= 0 && j <= sqrtX; j += i)
                    p[j] = false;
            }
        }

        primes.push_back(2);
        for (int i = 3; i <= sqrtX; i += 2) {
            if (p[i])
                primes.push_back(i);
        }
    }

    bool isPrime(int x) const {
        if (x <= 1)
            return false;

        if (x <= primes.back()) {
            return binary_search(primes.begin(), primes.end(), x);
        } else {
            for (int i = 0; i < int(primes.size()) && primes[i] * primes[i] <= x; i++) {
                if (x % primes[i] == 0)
                    return false;
            }
        }

        return true;
    }

    // O(sqrt(X) / log(X))
    vector<pair<int, int>> getPrimeFactors(int x) const {
        vector<pair<int, int>> res;

        int i = 0;
        int pf = primes[0];

        while (pf * pf <= x) {
            int n = 0;
            while (x % pf == 0) {
                x /= pf;
                n++;
            }
            if (n > 0)
                res.emplace_back(pf, n);

            if (++i >= int(primes.size()))
                break;
            pf = primes[i];
        }

        if (x > 1)
            res.emplace_back(x, 1);

        return res;
    }

    vector<int> getFactors(int x) const {
        if (x <= 1)
            return vector<int>{ 1 };

        auto F = getPrimeFactors(x);

        function<vector<int>(int, int)> dfs;
        dfs = [&](int L, int R) -> vector<int> {
            vector<int> res;
            if (L == R) {
                int x = 1;
                res.push_back(x);
                for (int i = 0; i < F[L].second; i++) {
                    x *= F[L].first;
                    res.push_back(x);
                }
            } else {
                int mid = L + (R - L) / 2;
                auto vL = dfs(L, mid);
                auto vR = dfs(mid + 1, R);
                for (auto a : vL) {
                    for (auto b : vR)
                        res.push_back(a * b);
                }
            }
            return res;
        };

        return dfs(0, int(F.size()) - 1);
    }

    // include 1
    vector<int> getSquareFreeFactors(int x) const {
        if (x <= 1)
            return vector<int>{ 1 };

        auto F = getPrimeFactors(x);

        function<vector<int>(int, int)> dfs;
        dfs = [&](int L, int R) -> vector<int> {
            vector<int> res;
            if (L == R) {
                res.push_back(1);
                res.push_back(F[L].first);
            } else {
                int mid = L + (R - L) / 2;
                auto vL = dfs(L, mid);
                auto vR = dfs(mid + 1, R);

                for (auto a : vL) {
                    for (auto b : vR)
                        res.push_back(a * b);
                }
            }
            return res;
        };

        return dfs(0, int(F.size()) - 1);
    }
};
