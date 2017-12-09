#pragma once

// get all prime numbers
inline vector<bool> eratosthenes(int n) {
    vector<bool> res(n + 1, true);
    res[0] = false;
    res[1] = false;

    if (n >= 4) {
        for (int j = 2 * 2; j <= n; j += 2)
            res[j] = false;
    }

    int root = (int)sqrt(n);
    for (int i = 3; i <= root; i += 2) {
        if (res[i]) {
            for (int j = i * i; j >= 0 && j <= n; j += i)
                res[j] = false;
        }
    }

    return res;
}

//--------- Min Prime Factors -------------------------------------------------

inline vector<int> getMinFactors(int n) {
    vector<int> res(n + 1);
    res[0] = -1;
    res[1] = -1;

    for (int i = 2; i <= n; i++)
        res[i] = i;

    if (n >= 4) {
        for (int j = 2 * 2; j <= n; j += 2)
            res[j] = 2;
    }

    int root = (int)sqrt(n);
    for (int i = 3; i <= root; i += 2) {
        if (res[i] == i) {
            for (int j = i * i; j >= 0 && j <= n; j += i) {
                if (res[j] == j)
                    res[j] = i;
            }
        }
    }

    return res;
}

inline bool isPrimeNumber(const vector<int>& minFactors, int x) {
    return minFactors[x] == x;
}

inline vector<int> getPrimeFactors(const vector<int>& minFactors, int x) {
    vector<int> res;
    while (x > 1) {
        res.push_back(minFactors[x]);
        x /= minFactors[x];
    }

    return res;
}

inline vector<pair<int, int>> getPrimeFactors2(const vector<int>& minFactors, int x) {
    vector<pair<int, int>> res;

    while (x > 1) {
        if (!res.empty() && res.back().first == minFactors[x])
            res.back().second++;
        else
            res.push_back(make_pair(minFactors[x], 1));
        x /= minFactors[x];
    }

    return res;
}
