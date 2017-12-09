#pragma once


// return factors of a number x
// O(sqrt(N))
template <typename T>
vector<T> getFactors(T x) {
    vector<T> res;
    if (x <= 0)
        return res;

    int i;
    for (i = 1; T(i) * i < x; i++) {
        if (x % i == 0) {
            res.push_back(T(i));
            res.push_back(x / i);
        }
    }

    if (T(i) * i == x)
        res.push_back(i);

    sort(res.begin(), res.end());

    return res;
}

// O(N logN)
// It's fast because it's very simple.
inline vector<int> getAllFactorCountSimple(int n) {
    vector<int> factorCount(n + 1, 1);
    factorCount[0] = 0;

    for (int div = 2; div <= n; div++) {
        for (int m = div; m <= n; m += div)
            factorCount[m]++;
    }

    return factorCount;
}
