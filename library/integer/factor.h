#pragma once


// return factors of a number x
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

vector<int> getFactors(const vector<pair<int, int>>& primeFactors);

int getFactorCount(const vector<pair<int, int>>& primeFactors);
vector<int> getAllFactorCount(const vector<int>& minFactors);
vector<int> getAllFactorCountSimple(int n);
