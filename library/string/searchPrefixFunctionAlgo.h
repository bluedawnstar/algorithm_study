#pragma once

// O(n*C)
inline string makeSmallestAlphabetStringFromPrefixArray(const vector<int>& pi) {
    if (pi.empty())
        return "";

    int n = int(pi.size());

    string s(n, ' ');
    s[0] = 'a';

    for (int i = 1; i < n; i++) {
        if (pi[i] > 0) {
            s[i] = s[pi[i] - 1];
            continue;
        }

        int flag = 0;
        int j = pi[i - 1];
        while (j > 0) {
            flag |= 1 << (s[j] - 'a');
            j = pi[j - 1];
        }

        // add the smallest alphabet to empty positions
        for (int j = 1; j < 26; j++) {
            if ((flag & (1 << j)) == 0) {
                s[i] = 'a' + j;
                break;
            }
        }
    }

    return s;
}


//--- repeated prefix string

// return (pattern length, pattern count), O(n)
inline pair<int, int> checkRepeatedString(const vector<int>& pi) {
    if (pi.empty())
        return make_pair(-1, -1);

    int len = int(pi.size());
    int patLen = len - pi[len - 1];
    if (pi[len - 1] && pi[len - 1] % patLen == 0)
        return make_pair(patLen, len / patLen);

    return make_pair(-1, -1);
}

inline pair<int, int> checkRepeatedString(string s) {
    return checkRepeatedString(prefixFunction(s));
}


// find maximum repeated prefix string (ex: "abababY" => (2, 3))
// return (pattern length, pattern count), O(n)
inline pair<int, int> getMaxRepeatedPrefixString(const vector<int>& pi) {
    for (int i = int(pi.size()) - 1; i >= 0; i--) {
        int patLen = i + 1 - pi[i];
        if (pi[i] && pi[i] % patLen == 0)
            return make_pair(patLen, (i + 1) / patLen);
    }

    return make_pair(-1, -1);
}

inline pair<int, int> getMaxRepeatedPrefixString(const string& s, int start) {
    return getMaxRepeatedPrefixString(prefixFunction(s, start));
}


//--- repeated substring

// find maximum repeated substring (ex: "XabababY" => (1, 2, 3) - "ab" x 3 at position 1)
// return (first pos, (pattern length, pattern count)), O(n^2)
inline pair<int, pair<int, int>> getMaxRepeatedSubstring(const string& s) {
    int repLen = 0;
    pair<int, pair<int, int>> res(-1, pair<int, int>(-1, -1));
    for (int i = 0; i < int(s.length()); i++) {
        int len = int(s.length()) - i;
        if (len <= repLen)
            break;

        vector<int> pi = prefixFunction(s, i);
        pair<int, int> p = getMaxRepeatedPrefixString(pi);
        if (p.first > 0 && p.first * p.second > repLen) {
            repLen = p.first * p.second;
            res.first = i;
            res.second = p;
        }
    }

    return res;
}


//--- prefix-suffix

/*
    prefix and suffix of s, O(n)

    -- example --

    s = "aabaa"
    pi = [0, 1, 0, 1, 2]

    result = [5(="aabaa"), 2(="aa"), 1(="a")]
*/
inline vector<int> getAllPrefixSuffixLengths(const string& s) {
    vector<int> pi = prefixFunction(s);
    vector<int> res;

    int n = int(s.length());
    while (n > 0) {
        res.push_back(n);
        n = pi[n - 1];
    }
    return res;
}

/*
    suffix of a and prefix of b, O(n)

    -- example --

    a = "dcbabcd"
            ^^^^
    b = "abcdefg"
         ^^^^

    result = "abcd"
*/
inline int getMaxOverlapLength(const string& a, const string& b) {
    vector<int> pi = prefixFunction(b);

    int n = int(a.length());
    int m = int(b.length());
    int j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && a[i] != b[j])
            j = pi[j - 1];

        if (a[i] == b[j]) {
            if (i + 1 == n)
                return j + 1;

            if (j + 1 == m)
                j = pi[j];
            else
                j++;
        }
    }

    return 0;
}
