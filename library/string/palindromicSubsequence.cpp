#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/////////// Palindromic Subsequences //////////////////////////////////////////

template <typename T>
int countPalindromicSubsequence(T s, int N) {
    vector<vector<int>> dp(N, vector<int>(N));

    for (int i = 0; i < N; i++)
        dp[i][i] = 1;

    for (int i = 0; i < N - 1; i++)
        dp[i][i + 1] = 2 + (s[i] == s[i + 1]);

    for (int i = N - 1; i >= 0; i--) {
        for (int j = i + 2; j < N; j++) {
            if (s[i] == s[j])
                dp[i][j] = dp[i + 1][j] + dp[i][j - 1] + 1;
            else
                dp[i][j] = dp[i + 1][j] + dp[i][j - 1] - dp[i + 1][j - 1];
        }
    }

    return dp[0][N - 1];
}

template <typename T>
int lengthOfLongestPalindromicSubsequence(T s, int N) {
    vector<vector<int>> dp(N, vector<int>(N));

    for (int i = 0; i < N; i++)
        dp[i][i] = 1;

    for (int i = 0; i < N - 1; i++)
        dp[i][i + 1] = 1 + (s[i] == s[i + 1]);

    for (int i = N - 1; i >= 0; i--) {
        for (int j = i + 2; j < N; j++) {
            if (s[i] == s[j])
                dp[i][j] = dp[i + 1][j - 1] + 2;
            else
                dp[i][j] = max(dp[i][j - 1], dp[i + 1][j]);
        }
    }

    return dp[0][N - 1];
}

template <typename T>
string longestPalindromicSubsequence(T s, int N) {
    vector<vector<int>> dp(N, vector<int>(N));

    for (int i = 0; i < N; i++)
        dp[i][i] = 1;

    for (int i = 0; i < N - 1; i++)
        dp[i][i + 1] = 1 + (s[i] == s[i + 1]);

    for (int i = N - 1; i >= 0; i--) {
        for (int j = i + 2; j < N; j++) {
            if (s[i] == s[j])
                dp[i][j] = dp[i + 1][j - 1] + 2;
            else
                dp[i][j] = max(dp[i][j - 1], dp[i + 1][j]);
        }
    }

    int len = dp[0][N - 1];

    // construct longest palindromic subsequence
    string res;
    res.reserve(len);

    int i, j;
    for (i = 0, j = N - 1; i < j; ) {
        if (dp[i][j] == dp[i + 1][j - 1] + 2) {
            res.append(1, s[i++]);
            j--;
        } else if (dp[i][j - 1] <= dp[i + 1][j])
            i++;
        else
            j--;
    }

    int half = (int)res.length();

    if ((len & 1) != 0)
        res.append(1, s[i]);

    for (i = half - 1; i >= 0; i--)
        res.append(1, res[i]);

    return res;
}


/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testPalindromicSubsequence() {
    return; //TODO: if you want to test string functions, make this line a comment.

    cout << "-- The number of palindromic subsequences ---" << endl;
    cout << countPalindromicSubsequence("a", 1) << endl;
    cout << countPalindromicSubsequence("aa", 2) << endl;
    cout << countPalindromicSubsequence("aaa", 3) << endl;
    cout << countPalindromicSubsequence("aba", 3) << endl;
    cout << countPalindromicSubsequence("abc", 3) << endl;
    cout << countPalindromicSubsequence("abcca", 5) << endl;

    cout << "-- Length of longest palindromic subsequence ---" << endl;
    cout << lengthOfLongestPalindromicSubsequence("a", 1) << endl;
    cout << lengthOfLongestPalindromicSubsequence("aa", 2) << endl;
    cout << lengthOfLongestPalindromicSubsequence("aaa", 3) << endl;
    cout << lengthOfLongestPalindromicSubsequence("aba", 3) << endl;
    cout << lengthOfLongestPalindromicSubsequence("abc", 3) << endl;
    cout << lengthOfLongestPalindromicSubsequence("abcca", 5) << endl;

    cout << "-- Longest palindromic subsequence ---" << endl;
    cout << longestPalindromicSubsequence("a", 1) << endl;
    cout << longestPalindromicSubsequence("aa", 2) << endl;
    cout << longestPalindromicSubsequence("aaa", 3) << endl;
    cout << longestPalindromicSubsequence("aba", 3) << endl;
    cout << longestPalindromicSubsequence("abc", 3) << endl;
    cout << longestPalindromicSubsequence("abcca", 5) << endl;
}
