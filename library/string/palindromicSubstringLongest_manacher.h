#pragma once

struct LongestPalindromicSubstring {
    // Manacher's algorithm - O(N)
    static string longestPalindromicSubstring(const char* s, int n) {
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

    static string longestPalindromicSubstring(const string& s) {
        return longestPalindromicSubstring(s.c_str(), int(s.length()));
    }


    // using Manacher's algorithm - O(N)
    // return the lengths of longest palindromic substrings to start from each characters
    static vector<int> getLongestPalindromes(const char* s, int n) {
        string s2;
        s2.reserve(n * 2);

        s2.push_back(s[0]);
        for (int i = 1; i < n; i++) {
            s2.push_back('#');
            s2.push_back(s[i]);
        }

        vector<int> A = getPalindromes(s2);

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

    static vector<int> getLongestPalindromes(const string& s) {
        return getLongestPalindromes(s.c_str(), int(s.length()));
    }


    // using Manacher's algorithm - O(N)
    // return the lengths of longest palindromic substrings to end at each characters
    static vector<int> getLongestPalindromesByEnd(const char* s, int n) {
        string s2;
        s2.reserve(n * 2);

        s2.push_back(s[0]);
        for (int i = 1; i < n; i++) {
            s2.push_back('#');
            s2.push_back(s[i]);
        }

        vector<int> A = getPalindromes(s2);

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

    static vector<int> getLongestPalindromesByEnd(const string& s) {
        return getLongestPalindromesByEnd(s.c_str(), int(s.length()));
    }

private:
    // Manacher's algorithm - O(N)
    //   return radius of longest palindromic substrings to be centered on each characters
    // https://algospot.com/wiki/read/Manacher%27s_algorithm
    static vector<int> getPalindromes(const char* s, int n) {
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

    static vector<int> getPalindromes(const string& s) {
        return getPalindromes(s.c_str(), int(s.length()));
    }
};
