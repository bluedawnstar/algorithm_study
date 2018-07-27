#pragma once

inline vector<int> prefixFunction(const string& s) {
    vector<int> p(s.size());
    int j = 0;
    for (int i = 1; i < int(s.size()); i++) {
        while (j > 0 && s[j] != s[i])
            j = p[j - 1];

        if (s[j] == s[i])
            j++;
        p[i] = j;
    }
    return p;
}

inline vector<int> prefixFunction(const string& s, int start) {
    int n = int(s.size()) - start;

    vector<int> p(n);
    int j = 0;
    for (int i = 1; i < n; i++) {
        while (j > 0 && s[j + start] != s[i + start])
            j = p[j - 1];

        if (s[j + start] == s[i + start])
            j++;
        p[i] = j;
    }
    return p;
}


// (pattern length, pattern count)
inline pair<int, int> checkRepeatedString(const vector<int>& prefix, int len) {
    if (prefix.empty())
        return make_pair(-1, -1);

    int n = len - prefix[len - 1];
    if (prefix[len - 1] && prefix[len - 1] % n == 0)
        return make_pair(n, len / n);

    return make_pair(-1, -1);
}

pair<int, int> checkRepeatedString(string s) {
    return checkRepeatedString(prefixFunction(s), int(s.length()));
}


// find maximum repeated prefix string (ex: "abababY" => (2, 3))
// (pattern length, pattern count)
inline pair<int, int> getMaxRepeatedPrefixString(const vector<int>& prefix) {
    if (!prefix.empty()) {
        for (int i = int(prefix.size()) - 1; i >= 0; i--) {
            int n = i + 1 - prefix[i];
            if (prefix[i] && prefix[i] % n == 0)
                return make_pair(n, (i + 1) / n);
        }
    }

    return make_pair(-1, -1);
}

inline pair<int, int> getMaxRepeatedPrefixString(const string& s, int start) {
    return getMaxRepeatedPrefixString(prefixFunction(s, start));
}


// find maximum repeated string (ex: "XabababY" => (1, 2, 3) - "ab" x 3 at position 1)
// (first pos, (pattern length, pattern count))
inline pair<int, pair<int, int>> getMaxRepeatedSubstring(const string& s) {
    int repLen = 0;
    pair<int, pair<int, int>> res(-1, pair<int, int>(-1, -1));
    for (int i = 0; i < int(s.length()); i++) {
        int len = int(s.length()) - i;
        if (len <= repLen)
            break;

        vector<int> pref = prefixFunction(s, i);
        pair<int, int> p = getMaxRepeatedPrefixString(pref);
        if (p.first > 0 && p.first * p.second > repLen) {
            repLen = p.first * p.second;
            res.first = i;
            res.second = p;
        }
    }

    return res;
}


// return matched lengths at each position
inline vector<int> kmpL(const string& s, const string& p) {
    vector<int> res;
    vector<int> pref = prefixFunction(p);

    int n = int(s.size()), m = int(p.size()), j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && s[i] != p[j])
            j = pref[j - 1];

        if (s[i] == p[j]) {
            res.push_back(j + 1);
            if (j == m - 1) {
                //P is found at i - m + 1
                j = pref[j];
            } else {
                j++;
            }
        } else {
            res.push_back(j);
        }
    }

    return res;
}

inline vector<int> kmp(const string& s, const string& p) {
    vector<int> res;
    vector<int> pref = prefixFunction(p);

    int n = int(s.size()), m = int(p.size()), j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && s[i] != p[j])
            j = pref[j - 1];

        if (s[i] == p[j]) {
            if (j == m - 1) {
                res.push_back(i - m + 1);
                j = pref[j];
            } else {
                j++;
            }
        }
    }

    return res;
}

inline vector<int> kmp(const string& s, const string& p, const vector<int>& prefix) {
    vector<int> res;

    int n = int(s.size()), m = int(p.size()), j = 0;
    for (int i = 0; i < n; i++) {
        while (j > 0 && s[i] != p[j])
            j = prefix[j - 1];

        if (s[i] == p[j]) {
            if (j == m - 1) {
                res.push_back(i - m + 1);
                j = prefix[j];
            } else {
                j++;
            }
        }
    }

    return res;
}

//----------------------

inline vector<int> getAllPartialMatch(const string& s) {
    int N = int(s.length());

    vector<int> res(N);
    int begin = 1, matched = 0;
    while (begin + matched < N) {
        if (s[begin + matched] == s[matched]) {
            ++matched;
            res[begin + matched - 1] = matched;
        } else {
            if (matched == 0)
                ++begin;
            else {
                begin += matched - res[matched - 1];
                matched = res[matched - 1];
            }
        }
    }

    return res;
}

vector<int> getPrefixSuffix(const string& s) {
    vector<int> res, pi = getAllPartialMatch(s);

    int n = int(s.length());
    while (n > 0) {
        res.push_back(n);
        n = pi[n - 1];
    }
    return res;
}

int maxOverlap(const string& a, const string& b) {
    int n = int(a.length()), m = int(b.length());
    vector<int> pi = getAllPartialMatch(b);

    int begin = 0, matched = 0;
    while (begin < n) {
        if (matched < m && a[begin + matched] == b[matched]) {
            ++matched;
            if (begin + matched == n)
                return matched;
        } else {
            if (matched == 0)
                ++begin;
            else {
                begin += matched - pi[matched - 1];
                matched = pi[matched - 1];
            }
        }
    }

    return 0;
}