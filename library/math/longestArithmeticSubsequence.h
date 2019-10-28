#pragma once

// An arithmetic progression (AP) or arithmetic sequence is a sequence of numbers
//   such that the difference between the consecutive terms is constant.

// https://www.hackerearth.com/practice/basic-programming/recursion/recursion-and-backtracking/practice-problems/approximate/longest-arithmetic-subsequence-0cca3167/

inline vector<int> buildLongestArithmeticSubsequence2(int n) {
    if (n == 1)
        return vector<int>{ 1 };

    auto p = buildLongestArithmeticSubsequence2((n + 1) / 2);
    auto q = buildLongestArithmeticSubsequence2(n / 2);

    vector<int> res;

    for (auto i : p)
        res.push_back(2 * i - 1);

    for (auto i : q)
        res.push_back(2 * i);

    return res;
}

// k = the length of longest arithmetic subsequence
inline vector<int> buildLongestArithmeticSubsequenceK(int n, int k) {
    vector<int> res;
    res.reserve(n);

    if (k == 1) {
        if (n == 1)
            res.push_back(1);
    } else if (k == 2) {
        res = buildLongestArithmeticSubsequence2(n);
    } else {
        vector<int> p = buildLongestArithmeticSubsequence2(n - k + 1);
        if ((n - k + 1) % 2 == 0) {
            for (int i = n; i >= n - k + 2; i--)
                res.push_back(i);
            for (auto i : p)
                res.push_back(i);
        } else {
            for (auto i : p)
                res.push_back(i);
            for (int i = n - k + 2; i <= n; i++)
                res.push_back(i);
        }
    }

    return res;
}
