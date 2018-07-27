#pragma once

inline vector<string> split(string s, char c) {
    int i, j = 0;
    vector<string> res;
    for (i = 0; i < int(s.length()); i++) {
        if (s[i] == c) {
            res.push_back(s.substr(j, i - j));
            j = i + 1;
        }
    }
    res.push_back(s.substr(j, i - j));

    return res;
}

// (start position, length)
inline vector<pair<int, int>> split2(string s, char c) {
    int i, j = 0;
    vector<pair<int, int>> res;
    for (i = 0; i < int(s.length()); i++) {
        if (s[i] == c) {
            res.push_back(make_pair(j, i - j));
            j = i + 1;
        }
    }
    res.push_back(make_pair(j, i - j));

    return res;
}
