#pragma once

// Manacher's Algorithm - Finding all sub-palindromes in O(N)
//   https://cp-algorithms.com/string/manacher.html

struct AllPalindromicSubstring {
    // Manacher's algorith, O(N)
    //   result[i] = the number of odd length palindromes with center i
    static vector<int> getAllOddLengthPalindromes(const char* s, int n) {
        vector<int> d1(n);
        for (int i = 0, l = 0, r = -1; i < n; i++) {
            int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);
            while (0 <= i - k && i + k < n && s[i - k] == s[i + k]) {
                k++;
            }
            d1[i] = k--;
            if (i + k > r) {
                l = i - k;
                r = i + k;
            }
        }
        return d1;
    }

    static vector<int> getAllOddLengthPalindromes(const string& s) {
        return getAllOddLengthPalindromes(s.c_str(), int(s.length()));
    }

    // Manacher's algorith, O(N)
    //   result[i] = the number of even length palindromes with center between i - 1 and i
    static vector<int> getAllEvenLengthPalindromes(const char* s, int n) {
        vector<int> d2(n);
        for (int i = 0, l = 0, r = -1; i < n; i++) {
            int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
            while (0 <= i - k - 1 && i + k < n && s[i - k - 1] == s[i + k]) {
                k++;
            }
            d2[i] = k--;
            if (i + k > r) {
                l = i - k - 1;
                r = i + k;
            }
        }
        return d2;
    }

    static vector<int> getAllEvenLengthPalindromes(const string& s) {
        return getAllEvenLengthPalindromes(s.c_str(), int(s.length()));
    }
};
