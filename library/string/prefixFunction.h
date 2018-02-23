#pragma once

struct PrefixFunction {
    vector<int> pi;
    string p;

    PrefixFunction(const string& s) {
        build(&s[0], (int)s.length());
    }

    PrefixFunction(const string& s, int start) {
        build(&s[start], (int)s.length() - start);
    }

    PrefixFunction(const char* s, int n) {
        build(s, n);
    }

    // (pattern length, pattern count)
    pair<int, int> checkRepeatedString() const {
        if (pi.empty())
            return make_pair(-1, -1);

        int len = (int)pi.size();
        int n = len - pi[len - 1];
        if (pi[len - 1] && pi[len - 1] % n == 0)
            return make_pair(n, len / n);

        return make_pair(-1, -1);
    }

    // find maximum repeated prefix string (ex: "abababY" => (2, 3))
    // (pattern length, pattern count)
    pair<int, int> getMaxRepeatedPrefixString() const {
        if (!pi.empty()) {
            for (int i = (int)pi.size() - 1; i >= 0; i--) {
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
        for (int i = 0; i < (int)s.length(); i++) {
            int len = (int)s.length() - i;
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

        int n = (int)p.length();
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

private:
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
};
