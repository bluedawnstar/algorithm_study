#pragma once

// Is 't' subsequence of 's'
// O(max(|s|, |t|))
inline bool isSubsequence(const string& s, const string& t) {
    int i, j;
    for (i = 0, j = 0; i < (int)s.length() && j < (int)t.length(); i++) {
        if (s[i] == t[j])
            j++;
    }
    return j >= (int)t.length();
}

struct NextPositionTable {
    int N;
    vector<vector<int>> next;

    NextPositionTable() {
    }

    NextPositionTable(const string& s, char first = 'a', char last = 'z') {
        build(s, first, last);
    }

    void build(const string& s, char first = 'a', char last = 'z') {
        N = (int)s.length();
        next.resize(last - first + 1, vector<int>(N + 1));

        for (int i = 0; i <= last - first; i++)
            next[i][N] = N + 1;

        for (int i = N - 1; i >= 0; i--) {
            for (int j = 0; j <= last - first; j++)
                next[j][i] = next[j][i + 1];
            next[s[i] - first][i] = i + 1;
        }
    }


    // Is 't' subsequence of 's'
    // O(|t|), inclusive
    bool isSubsequence(const string& t, char first = 'a', char last = 'z') const {
        int pos = 0;
        for (char c : t) {
            pos = next[c - first][pos];
            if (pos > N)
                return false;
        }
        return true;
    }
};

struct CompactNextPositionTable {
    int N;
    vector<vector<int>> next;

    CompactNextPositionTable() {
    }

    CompactNextPositionTable(const string& s, char first = 'a', char last = 'z') {
        build(s, first, last);
    }

    void build(const string& s, char first = 'a', char last = 'z') {
        N = (int)s.length();
        next.resize(last - first + 1, vector<int>(N + 1));

        for (int i = 0; i < N; i++)
            next[s[i] - first].push_back(i + 1);

        for (int i = 0; i <= last - first; i++)
            next[i].push_back(N + 1);
    }

    // Is 't' subsequence of 's'
    // O(|t|log|s|), inclusive
    bool isSubsequence(const string& t, char first = 'a', char last = 'z') const {
        int pos = 0;
        for (char c : t) {
            pos = *upper_bound(next[c - first].begin(), next[c - first].end(), pos);
            if (pos > N)
                return false;
        }
        return true;
    }
};
