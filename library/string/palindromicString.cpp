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


/////////// Palindromic Substrings ////////////////////////////////////////////

template <typename T>
int countPalindromicSubstring(T s, int N) {
    vector<vector<int>> dp(N, vector<int>(N));

    int res = 0;

    for (int i = 0; i < N; i++) {
        dp[i][i] = 1;
        res++;
    }

    for (int i = 0; i < N - 1; i++) {
        if (s[i] == s[i + 1]) {
            dp[i][i + 1] = 2;
            res++;
        }
    }

    for (int i = N - 1; i >= 0; i--) {
        for (int j = i + 2; j < N; j++) {
            if (s[i] == s[j] && dp[i + 1][j - 1]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
                res++;
            }
        }
    }

    return res;
}

template <typename T>
int lengthOfLongestPalindromicSubstring(T s, int N) {
    vector<vector<int>> dp(N, vector<int>(N));

    int res = 1;

    for (int i = 0; i < N; i++)
        dp[i][i] = 1;

    for (int i = 0; i < N - 1; i++) {
        if (s[i] == s[i + 1]) {
            dp[i][i + 1] = 2;
            res = 2;
        }
    }

    for (int i = N - 1; i >= 0; i--) {
        for (int j = i + 2; j < N; j++) {
            if (s[i] == s[j] && dp[i + 1][j - 1]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
                res = max(res, dp[i][j]);
            }
        }
    }

    return res;
}

template <typename T>
string longestPalindromicSubstring(T s, int N) {
    vector<vector<int>> dp(N, vector<int>(N));

    int maxLen = 1;
    int maxPosR = 0;

    for (int i = 0; i < N; i++)
        dp[i][i] = 1;

    for (int i = 0; i < N - 1; i++) {
        if (s[i] == s[i + 1]) {
            dp[i][i + 1] = 2;
            if (maxLen < 2) {
                maxLen = 2;
                maxPosR = i + 1;
            }
        }
    }

    for (int i = N - 1; i >= 0; i--) {
        for (int j = i + 2; j < N; j++) {
            if (s[i] == s[j] && dp[i + 1][j - 1]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
                if (maxLen < dp[i][j]) {
                    maxLen = dp[i][j];
                    maxPosR = j;
                }
            }
        }
    }

    // construct longest palindromic substring
    string res;
    res.reserve(maxLen);

    int i, j;
    for (i = maxPosR - maxLen + 1, j = maxPosR; i < j && dp[i][j] == dp[i + 1][j - 1] + 2; i++, j--) {
        res.append(1, s[i]);
    }

    int half = (int)res.length();

    if ((maxLen & 1) != 0)
        res.append(1, s[i]);

    for (i = half - 1; i >= 0; i--)
        res.append(1, res[i]);

    return res;
}

// Manacher's algorithm - O(N)
template <typename T>
string fastLongestPalindromicSubstring(T& s, int n) {
    if (n == 0)
        return string();

    int N = 2 * n + 1;
    vector<int> L(N);   //LPS length array

    L[0] = 0;
    L[1] = 1;

    int C = 1;
    int R = 2;

    int maxLPSLen = 1;
    int maxLPSCenter = 1;

    for (int i = 2; i < N; i++) {
        int mirror = 2 * C - i;

        L[i] = (i < R) ? min(L[mirror], R - i) : 0;

        // Attempt to expand palindrome centered at i
        while ( (i + L[i] < N && L[i] < i) &&
                (((i + L[i] + 1) & 1) == 0 || s[(i + L[i] + 1) >> 1] == s[(i - L[i] - 1) >> 1]) )
            L[i]++;

        if (L[i] > maxLPSLen) {
            maxLPSLen = L[i];
            maxLPSCenter = i;
        }

        // If palindrome centered at i expand past R,
        // adjust center based on expanded palindrome.
        if (i + L[i] > R) {
            C = i;
            R = i + L[i];
        }
    }

    return string(&s[(maxLPSCenter - maxLPSLen) / 2], maxLPSLen);
}


/////////// For Testing ///////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include "../common/iostreamhelper.h"

void testPalindromicString() {
    //return; //TODO: if you want to test string functions, make this line to a comment.

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

    cout << "-- The number of palindromic substrings ---" << endl;
    cout << countPalindromicSubstring("a", 1) << endl;
    cout << countPalindromicSubstring("aa", 2) << endl;
    cout << countPalindromicSubstring("aaa", 3) << endl;
    cout << countPalindromicSubstring("aba", 3) << endl;
    cout << countPalindromicSubstring("abc", 3) << endl;
    cout << countPalindromicSubstring("abcca", 5) << endl;
    cout << countPalindromicSubstring("abca", 4) << endl;

    cout << "-- Length of longest palindromic substring ---" << endl;
    cout << lengthOfLongestPalindromicSubstring("a", 1) << endl;
    cout << lengthOfLongestPalindromicSubstring("aa", 2) << endl;
    cout << lengthOfLongestPalindromicSubstring("aaa", 3) << endl;
    cout << lengthOfLongestPalindromicSubstring("aba", 3) << endl;
    cout << lengthOfLongestPalindromicSubstring("abc", 3) << endl;
    cout << lengthOfLongestPalindromicSubstring("abcca", 5) << endl;
    cout << lengthOfLongestPalindromicSubstring("abca", 4) << endl;

    cout << "-- Longest palindromic substring ---" << endl;
    cout << longestPalindromicSubstring("a", 1) << endl;
    cout << longestPalindromicSubstring("aa", 2) << endl;
    cout << longestPalindromicSubstring("aaa", 3) << endl;
    cout << longestPalindromicSubstring("aba", 3) << endl;
    cout << longestPalindromicSubstring("abc", 3) << endl;
    cout << longestPalindromicSubstring("abcca", 5) << endl;
    cout << longestPalindromicSubstring("abca", 4) << endl;

    cout << "-- Fast longest palindromic substring ---" << endl;
    cout << fastLongestPalindromicSubstring("a", 1) << endl;
    cout << fastLongestPalindromicSubstring("aa", 2) << endl;
    cout << fastLongestPalindromicSubstring("aaa", 3) << endl;
    cout << fastLongestPalindromicSubstring("aba", 3) << endl;
    cout << fastLongestPalindromicSubstring("abc", 3) << endl;
    cout << fastLongestPalindromicSubstring("abcca", 5) << endl;
    cout << fastLongestPalindromicSubstring("abca", 4) << endl;

#define SS(s)   s, sizeof(s) - 1
    cout << fastLongestPalindromicSubstring(SS("babcbabcbaccba")) << endl;
    cout << fastLongestPalindromicSubstring(SS("abaaba")) << endl;
    cout << fastLongestPalindromicSubstring(SS("abababa")) << endl;
    cout << fastLongestPalindromicSubstring(SS("abcbabcbabcba")) << endl;
    cout << fastLongestPalindromicSubstring(SS("forgeeksskeegfor")) << endl;
    cout << fastLongestPalindromicSubstring(SS("caba")) << endl;
    cout << fastLongestPalindromicSubstring(SS("abacdfgdcaba")) << endl;
    cout << fastLongestPalindromicSubstring(SS("abacdfgdcabba")) << endl;
    cout << fastLongestPalindromicSubstring(SS("abacdedcaba")) << endl;
}
