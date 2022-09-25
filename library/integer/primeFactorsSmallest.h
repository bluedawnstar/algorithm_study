#pragma once

// all minimum prime factors of [0, n]
struct SmallestPrimeFactors {
    vector<int> spf;    // smallest prime factors of [0, n]

    SmallestPrimeFactors() {
    }

    explicit SmallestPrimeFactors(int n) {
        build(n);
    }

    void build(int n) {
        spf.resize(n + 1);
        spf[0] = -1;
        spf[1] = -1;

        for (int i = 2; i <= n; i++)
            spf[i] = i;

        if (n >= 4) {
            for (int j = 2 * 2; j <= n; j += 2)
                spf[j] = 2;
        }

        int root = int(sqrt(n));
        for (int i = 3; i <= root; i += 2) {
            if (spf[i] == i && 1ll * i * i <= n) {
                for (int j = i * i; j <= n; j += i) {
                    if (spf[j] == j)
                        spf[j] = i;
                }
            }
        }
    }

    bool isPrimeNumber(int x) const {
        return spf[x] == x;
    }

    vector<int> getPrimeFactors(int x) const {
        vector<int> res;
        while (x > 1) {
            res.push_back(spf[x]);
            x /= spf[x];
        }
        return res;
    }

    vector<pair<int, int>> getPrimeFactorsEx(int x) const {
        vector<pair<int, int>> res;
        while (x > 1) {
            if (!res.empty() && spf[x] == res.back().first)
                res.back().second++;
            else
                res.emplace_back(spf[x], 1);
            x /= spf[x];
        }

        return res;
    }

    // O(N) (the time complexity to get min prime factors is not included)
    // return : factorCount - the number of factors of each number from 0 to n
    vector<int> getAllFactorCount() const {
        int n = int(spf.size());

        vector<int> spfPower(n);
        vector<int> factorCount(n);

        factorCount[0] = 0;
        factorCount[1] = 1;

        for (int i = 2; i < n; i++) {
            if (spf[i] == i) { // prime number
                spfPower[i] = 1;
                factorCount[i] = 2;
            } else {
                int p = spf[i];
                int j = i / p;

                if (spf[j] != p)
                    spfPower[i] = 1;
                else
                    spfPower[i] = spfPower[j] + 1;
                int a = spfPower[i];
                factorCount[i] = (factorCount[j] / a) * (a + 1);
            }
        }

        return factorCount;
    }
};
