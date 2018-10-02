#pragma once

/////////// Palindromic Substrings ////////////////////////////////////////////

// Manacher's algorithm - O(N)
// return radius of longest palindromic substrings to be centered on each characters
// https://algospot.com/wiki/read/Manacher%27s_algorithm
template <typename T>
vector<int> getPalindromes(T& s, int n) {
    vector<int> A(n);

    int R = -1;
    int p = -1;
    for (int i = 0; i < n; i++) {
        if (i <= R)
            A[i] = min(A[2 * p - i], R - i);
        else
            A[i] = 0;
        while ((i - A[i] - 1) >= 0 && (i + A[i] + 1) < n && s[i - A[i] - 1] == s[i + A[i] + 1]) {
            A[i]++;
        }
        if (R < i + A[i]) {
            R = i + A[i];
            p = i;
        }
    }

    return A;
}

// using Manacher's algorithm - O(N)
// return the lengths of longest palindromic substrings to start from each characters
template <typename T>
vector<int> getLongestPalindromes(T& s, int n) {
    string s2;
    s2.reserve(n * 2);

    s2.push_back(s[0]);
    for (int i = 1; i < n; i++) {
        s2.push_back('#');
        s2.push_back(s[i]);
    }

    vector<int> A = getPalindromes(s2, int(s2.length()));

    vector<int> res(n);

    for (int i = int(A.size()) - 1; i >= 0; i--) {
        int curr = i / 2;
        int R = A[i];

        if (i & 1) { // '#'
            if (R < 1)
                continue;
            R = (R + 1) / 2;

            for (int j = curr - R + 1; j <= curr; j++) {
                if (res[j])
                    break;
                res[j] = 2 * R--;
            }
        } else {
            R = R / 2;

            for (int j = curr - R; j <= curr; j++) {
                if (res[j])
                    break;
                res[j] = 2 * R-- + 1;
            }
        }
    }

    return res;
}

// using Manacher's algorithm - O(N)
// return the lengths of longest palindromic substrings to end at each characters
template <typename T>
vector<int> getLongestPalindromesByEnd(T& s, int n) {
    string s2;
    s2.reserve(n * 2);

    s2.push_back(s[0]);
    for (int i = 1; i < n; i++) {
        s2.push_back('#');
        s2.push_back(s[i]);
    }

    vector<int> A = getPalindromes(s2, int(s2.length()));

    vector<int> res(n);

    for (int i = 0; i < int(A.size()); i++) {
        int curr = i / 2;
        int R = A[i];

        if (i & 1) { // '#'
            if (R < 1)
                continue;
            R = (R + 1) / 2;

            for (int j = curr + R; j >= curr; j--) {
                if (res[j])
                    break;
                res[j] = 2 * R--;
            }
        } else {
            R = R / 2;

            for (int j = curr + R; j >= curr; j--) {
                if (res[j])
                    break;
                res[j] = 2 * R-- + 1;
            }
        }
    }

    return res;
}


// DP solution
template <typename T>
int countPalindromicSubstring(T& s, int N) {
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

    for (int i = N - 2; i >= 0; i--) {
        for (int j = i + 2; j < N; j++) {
            if (s[i] == s[j] && dp[i + 1][j - 1]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
                res++;
            }
        }
    }

    return res;
}

// DP solution
template <typename T>
int lengthOfLongestPalindromicSubstring(T& s, int N) {
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

    for (int i = N - 2; i >= 0; i--) {
        for (int j = i + 2; j < N; j++) {
            if (s[i] == s[j] && dp[i + 1][j - 1]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
                res = max(res, dp[i][j]);
            }
        }
    }

    return res;
}

// DP solution
template <typename T>
string longestPalindromicSubstring(T& s, int N) {
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

    for (int i = N - 2; i >= 0; i--) {
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

    int half = int(res.length());

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
        while ((i + L[i] < N && L[i] < i) &&
            (((i + L[i] + 1) & 1) == 0 || s[(i + L[i] + 1) >> 1] == s[(i - L[i] - 1) >> 1]))
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
