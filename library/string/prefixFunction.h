#pragma once

struct PrefixFunction {
    vector<int> pi;
    string p;

    PrefixFunction() {
    }

    PrefixFunction(const char* s, int n) {
        build(s, n);
    }

    PrefixFunction(const string& s) {
        build(s);
    }

    PrefixFunction(const string& s, int start) {
        build(&s[start], int(s.length()) - start);
    }


    void build(const char* s, int n) {
        pi.resize(n);
        p.assign(s, n);

        int j = 0;
        for (int i = 1; i < n; i++) {
            while (j > 0 && s[j] != s[i])
                j = pi[j - 1];

            if (s[j] == s[i])
                j++;
            pi[i] = j;
        }
    }

    void build(const string& s) {
        build(&s[0], int(s.length()));
    }

    void build(const string& s, int start) {
        build(&s[start], int(s.length()) - start);
    }

    //---

    // (pattern length, pattern count)
    pair<int, int> checkRepeatedString() const {
        if (pi.empty())
            return make_pair(-1, -1);

        int len = int(pi.size());
        int n = len - pi[len - 1];
        if (pi[len - 1] && pi[len - 1] % n == 0)
            return make_pair(n, len / n);

        return make_pair(-1, -1);
    }

    // find maximum repeated prefix string (ex: "abababY" => (2, 3))
    // (pattern length, pattern count)
    pair<int, int> getMaxRepeatedPrefixString() const {
        if (!pi.empty()) {
            for (int i = int(pi.size()) - 1; i >= 0; i--) {
                int n = i + 1 - pi[i];
                if (pi[i] && pi[i] % n == 0)
                    return make_pair(n, (i + 1) / n);
            }
        }

        return make_pair(-1, -1);
    }

    // find maximum repeated string (ex: "XabababY" => (1, 2, 3) - "ab" x 3 at position 1)
    // (first pos, pattern length, pattern count)
    static tuple<int, int, int> getMaxRepeatedSubstring(const string& s) {
        int repLen = 0;
        tuple<int, int, int> res(-1, -1, -1);
        for (int i = 0; i < int(s.length()); i++) {
            int len = int(s.length()) - i;
            if (len <= repLen)
                break;

            PrefixFunction pref(s, i);
            pair<int, int> p = pref.getMaxRepeatedPrefixString();
            if (p.first > 0 && p.first * p.second > repLen) {
                repLen = p.first * p.second;
                res = make_tuple(i, p.first, p.second);
            }
        }

        return res;
    }


    // lengths of all substrings which are suffix and prefix (including original string's length)
    vector<int> getPrefixSuffix() const {
        vector<int> res;

        int n = int(p.length());
        while (n > 0) {
            res.push_back(n);
            n = pi[n - 1];
        }
        return res;
    }

    // lengths of all substrings which are suffix and prefix in substring s[0..i]
    vector<int> getPrefixSuffix(int i) const {
        vector<int> res;

        int n = i + 1;
        while (n > 0) {
            res.push_back(n);
            n = pi[n - 1];
        }
        return res;
    }

    //---

    // length of max common proper suffix-prefix substring between substring s[0..i] and s[0..j]
    //   ex) "bcabcabc" : getMaxCommonProperPrefixSuffix(4, 7) = 2
    // [CAUTION] exclude s[0..i] == s[(j-i)..j]
    int getMaxCommonProperPrefixSuffix(int i, int j) const {
        if (p[i] != p[j])
            return 0;
        else if (i == j)
            return pi[i];

        int n1 = pi[i];
        int n2 = pi[j];
        while (n1 > 0 && n2 > 0) {
            if (n1 == n2)
                return n1;
            else if (n1 > n2)
                n1 = pi[n1 - 1];
            else
                n2 = pi[n2 - 1];
        }
        return 0;
    }

    // length of max common proper suffix-prefix substring between substring s[0..i] and s[0..j]
    //   ex) "bcabcabc" : getMaxCommonProperPrefixSuffix(4, 7) = 2
    // [CAUTION] exclude s[0..i] == s[(j-i)..j]
    int getMaxCommonProperPrefixSuffixFast(int i, int j) const {
        if (p[i] != p[j])
            return 0;
        else if (i == j)
            return pi[i];

        int n1 = pi[i];
        int n2 = pi[j];
        while (n1 > 0 && n2 > 0) {
            if (n1 == n2)
                return n1;

            int repeat1 = n1 - pi[n1 - 1];
            int repeat2 = n2 - pi[n2 - 1];
            if (repeat1 == repeat2 && n1 % repeat1 == n2 % repeat2)
                return min(n1, n2);

            if (n1 > n2)
                n1 %= repeat1;
            else
                n2 %= repeat2;
        }
        return 0;
    }

    //---

    // length of max common proper suffix-prefix substring between substring s[0..i] and s[0..j]
    //   ex) "bcabcabc" : getMaxCommonPrefixSuffix(4, 7) = 2, getMaxCommonPrefixSuffix(3, 6) = 4
    int getMaxCommonPrefixSuffix(int i, int j) const {
        if (p[i] != p[j])
            return 0;
        else if (i == j)
            return i + 1;

        int n1 = i + 1;
        int n2 = j + 1;
        while (n1 > 0 && n2 > 0) {
            if (n1 == n2)
                return n1;
            else if (n1 > n2)
                n1 = pi[n1 - 1];
            else
                n2 = pi[n2 - 1];
        }
        return 0;
    }

    // length of max common proper suffix-prefix substring between substring s[0..i] and s[0..j]
    //   ex) "bcabcabc" : getMaxCommonPrefixSuffix(4, 7) = 2, getMaxCommonPrefixSuffix(3, 6) = 4
    int getMaxCommonPrefixSuffixFast(int i, int j) const {
        if (p[i] != p[j])
            return 0;
        else if (i == j)
            return i + 1;

        if (i > j)
            swap(i, j);
        if (i == 0)
            return 1;

        int n1 = i + 1;
        int n2 = j + 1;
        while (n1 > 0 && n2 > 0) {
            if (n1 == n2)
                return n1;

            int repeat1 = n1 - pi[n1 - 1];
            int repeat2 = n2 - pi[n2 - 1];
            if (repeat1 == repeat2 && n1 % repeat1 == n2 % repeat2)
                return min(n1, n2);

            if (n1 > n2)
                if (n1 == i + 1)
                    n1 = pi[i];
                else
                    n1 %= repeat1;
            else {
                if (n2 == j + 1)
                    n2 = pi[j];
                else
                    n2 %= repeat2;
            }
        }
        return 0;
    }
};
