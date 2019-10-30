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


//---

// https://www.geeksforgeeks.org/longest-arithmetic-progression-dp-35/

inline int lenghtOfLongestArithmeticSubsequence(int A[], int n) { 
    if (n <= 2)
        return n; 

    int res = 2;

    // dp[i][j] = LLAP with A[i] and A[j] as first two elements of AP, i < j
    vector<vector<int>> dp(n, vector<int>(n));

    for (int i = 0; i < n; i++) 
        dp[i][n - 1] = 2;

    for (int i = n - 2; i >= 1; i--) { 
        // ... A[L] ... A[i] ... A[R] ...
        int L = i - 1, R = i + 1; 
        while (L >= 0 && R < n) {
            if (A[L] + A[R] < 2 * A[i]) 
                R++;
            else if (A[L] + A[R] > 2 * A[i]) {
                dp[L][i] = 2;
                L--;
            } else {
                dp[L][i] = dp[i][R] + 1; 
                res = max(res, dp[L][i]); 
                L--;
                R++; 
            } 
        } 

        while (L >= 0)
            dp[L--][i] = 2; 
    } 

    return res;
} 

inline int lenghtOfLongestArithmeticSubsequence(vector<int>& A) {
    return lenghtOfLongestArithmeticSubsequence(A.data(), int(A.size()));
}
