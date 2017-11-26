#pragma once

struct PalindromicNumberLongLong {
    static string makePalindromic(string s) {
        for (int i = 0, j = (int)s.length() - 1; i < j; i++, j--)
            s[j] = s[i];
        return s;
    }

    static string nearestPalindromic(const string& n) {
        if (n == "0")
            return "1";

        long long orgVal = stoll(n);

        // candidate #1 (ex: 123xx -> 12321, 123xxx -> 123321)
        string res = makePalindromic(n);
        long long resVal = stoll(res);
        long long diff = abs(resVal - orgVal);

        long long scale = (long long)pow(10, (int)n.length() / 2);

        // candidate #2 (ex: 123xx -> 12221, 123xxx -> 122221, 100xx -> 9999)
        string smaller = makePalindromic(to_string((orgVal / scale) * scale - 1));
        // candidate #3 (ex: 123xx -> 12421, 123xxx -> 124421, 99xx -> 10001)
        string bigger = makePalindromic(to_string((orgVal / scale) * scale + scale));

        long long smallerVal = stoll(smaller);
        if (diff == 0 || abs(orgVal - smallerVal) <= diff) {
            res = smaller;
            diff = abs(orgVal - smallerVal);
        }

        long long biggerVal = stoll(bigger);
        if (abs(orgVal - biggerVal) < diff)
            res = bigger;

        return res;
    }
};

struct PalindromicNumber {
    static string makePalindromic(string s) {
        for (int i = 0, j = (int)s.length() - 1; i < j; i++, j--)
            s[j] = s[i];
        return s;
    }

    static int compare(const string& s1, const string& s2) {
        if (s1.length() < s2.length())
            return -1;
        else if (s1.length() > s2.length())
            return 1;

        for (int i = 0; i < (int)s1.length(); i++) {
            if (s1[i] < s2[i])
                return -1;
            else if (s1[i] > s2[i])
                return 1;
        }
        return 0;
    }

    static string add(const string& a, const string& b) {
        string res;

        int carry = 0;
        for (int i = (int)a.length() - 1, j = (int)b.length() - 1; i >= 0 || j >= 0; i--, j--) {
            carry += (i >= 0 ? a[i] - '0' : 0) + (j >= 0 ? b[j] - '0' : 0);
            res.push_back(carry % 10 + '0');
            carry /= 10;
        }
        if (carry)
            res.push_back(carry + '0');

        reverse(res.begin(), res.end());

        return res;
    }

    static string subAbs(const string& s1, const string& s2) {
        int c = compare(s1, s2);
        if (c == 0)
            return string("0");

        const string& lo = (c < 0) ? s1 : s2;
        const string& hi = (c > 0) ? s1 : s2;

        string res;
        res.reserve(hi.length());

        int carry = 0;
        for (int i = (int)hi.length() - 1, j = (int)lo.length() - 1; i >= 0 || j >= 0; i--, j--) {
            carry = (i >= 0 ? hi[i] - '0' : 0) - (j >= 0 ? lo[j] - '0' : 0) + carry;
            if (carry < 0) {
                res.push_back(carry + 10 + '0');
                carry = -1;
            } else {
                res.push_back(carry + '0');
                carry = 0;
            }
        }
        while (res.back() == '0' && (int)res.length() > 1)
            res.pop_back();
        reverse(res.begin(), res.end());

        return res;
    }

    static string shift10(const string& s, int n) {
        if (n == 0)
            return s;

        string res = s;
        if (n > 0)
            return res.append(n, '0');
        else
            return res.erase((int)s.length() + n);
    }

    static string nearestPalindromic(const string& n) {
        if (n == "0")
            return "1";

        // candidate #1 (ex: 123xx -> 12321, 123xxx -> 123321)
        string res = makePalindromic(n);
        string diff = subAbs(res, n);

        int scaleN = (int)n.length() / 2;
        string scale = shift10("1", scaleN);

        // candidate #2 (ex: 123xx -> 12221, 123xxx -> 122221, 100xx -> 9999)
        string smaller = makePalindromic(subAbs(shift10(shift10(n, -scaleN), scaleN), "1"));
        // candidate #3 (ex: 123xx -> 12421, 123xxx -> 124421, 99xx -> 10001)
        string bigger = makePalindromic(add(shift10(shift10(n, -scaleN), scaleN), scale));

        if (diff == "0" || compare(subAbs(n, smaller), diff) <= 0) {
            res = smaller;
            diff = subAbs(n, smaller);
        }

        if (compare(subAbs(n, bigger), diff) < 0)
            res = bigger;

        return res;
    }
};


/////////// Palindromic Substrings ////////////////////////////////////////////

// A[i] : starting position of the longest palindromic string to be ended at i
// Manacher's algorithm - O(N)
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
// return the lengths of longest palindromic strings to start from each characters
template <typename T>
vector<int> getLongestPalindromes(T& s, int n) {
    string s2;
    s2.reserve(n * 2);

    s2.push_back(s[0]);
    for (int i = 1; i < n; i++) {
        s2.push_back('#');
        s2.push_back(s[i]);
    }

    vector<int> A = getPalindromes(s2, (int)s2.length());

    vector<int> res(n);

    for (int i = (int)A.size() - 1; i >= 0; i--) {
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
