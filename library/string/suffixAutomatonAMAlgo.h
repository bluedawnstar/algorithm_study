#pragma once

struct SuffixAutomatonAMAlgo {
    int maxN;
    SuffixAutomatonAM& sa;

    SuffixAutomatonAMAlgo(SuffixAutomatonAM& _sa, int _maxN) : sa(_sa), maxN(_maxN),
        D(maxN * 2, 0), totLen(maxN * 2, 0) {
    }

    // 1. Number of different substrings of a given string .
    vector<long long> D;
    long long countSubstrings(int u) {
        if (D[u])
            return D[u];

        long long res = 1;

        for (int i = 0; i < SuffixAutomatonAM::MaxCharN; i++) {
            if (sa.state[u].hasNext(i))
                res += countSubstrings(sa.state[u].getNext(i));
        }

        return D[u] = res;
    }

    long long countSubstrings() {
        fill(D.begin(), D.end(), 0);
        return countSubstrings(0) - 1;
    }

    // 2. Total length of all distinct substrings.
    vector<long long> totLen;
    long long totalLengthOfAllDistinctSubstrings(int u) {
        long long res = 0;

        if (totLen[u])
            return totLen[u];

        for (int i = 0; i < SuffixAutomatonAM::MaxCharN; i++)
            if (sa.state[u].hasNext(i))
                //res += totalLengthOfAllDistinctSubstrings(sa.state[u].getNext(i)) + gD[sa.state[u].getNext(i)];
                res += totalLengthOfAllDistinctSubstrings(sa.state[u].getNext(i)) + countSubstrings(sa.state[u].getNext(i));

        return totLen[u] = res;
    }

    long long totalLengthOfAllDistinctSubstrings() {
        fill(D.begin(), D.end(), 0);
        countSubstrings();

        fill(totLen.begin(), totLen.end(), 0);
        return totalLengthOfAllDistinctSubstrings(0);
    }

    // 3. Lexographically kth substring.
    // kth >= 1
    bool kthSubstring(string& res, int kth, int u, int& path) {
        for (int i = 0; i < SuffixAutomatonAM::MaxCharN; i++) {
            if (sa.state[u].hasNext(i)) {
                if (++path == kth || kthSubstring(res, kth, sa.state[u].getNext(i), path)) {
                    res.push_back('a' + i);
                    return true;
                }
            }
        }
        return false;
    }

    // kth >= 0
    string kthSubstring(int kth) {
        string res;
        int path = 0;

        kthSubstring(res, kth + 1, 0, path);

        reverse(res.begin(), res.end());
        return res;
    }

    // 4. Smallest Cyclic Shift to obtain lexicographical smallest of All possible  
    static int minShiftAM(const string& s) {
        string ss = s + s;
        SuffixAutomatonAM sa((int)ss.length());
        sa.init();
        sa.extend(ss);

        int res = -1;
        int len = 0, cur = 0;

        do {
            for (int j = 0; j < SuffixAutomatonAM::MaxCharN; j++) {
                if (sa.state[cur].hasNext(j)) {
                    if (++len == (int)s.length())
                        res = sa.state[cur].rightEnd - (int)s.length() + 2;
                    else
                        cur = sa.state[cur].getNext(j);
                    break;
                }
            }
        } while (res < 0);

        return min(res, (int)s.length() - res);
    }

    // 5. position of first occurrence
    int findFirst(const string& pattern) {
        int u = 0;
        for (int i = 0; i < (int)pattern.length(); i++) {
            u = sa.state[u].getNext(SuffixAutomatonAM::ch2i(pattern[i]));
            if (u <= 0)
                return -1;
        }
        return sa.state[u].rightEnd - (int)pattern.length() + 1;
    }

    // 6. Position of all occurrences.
    void dfsFind(vector<vector<int>>& children, int len, vector<int>& res, int u) {
        if (u <= 0)
            return;

        res.push_back(sa.state[u].rightEnd - len + 1);
        for (int v : children[u])
            dfsFind(children, len, res, v);
    }

    vector<int> findAll(const string& pattern) {
        vector<vector<int>> children(sa.N);
        for (int u = 1; u < sa.N; u++) {
            children[sa.state[u].suffixLink].push_back(u);
        }

        int u = 0;
        for (int i = 0; i < (int)pattern.length(); i++) {
            u = sa.state[u].getNext(SuffixAutomatonAM::ch2i(pattern[i]));
            if (u <= 0)
                return{};
        }

        vector<int> res;
        dfsFind(children, (int)pattern.length(), res, u);
        res.erase(unique(res.begin(), res.end()), res.end());

        return res;
    }


    vector<vector<int>> makeLinkTree() {
        vector<vector<int>> children(sa.N);
        for (int u = 1; u < sa.N; u++) {
            children[sa.state[u].suffixLink].push_back(u);
        }
        return children;
    }

    vector<int> findAll(vector<vector<int>>& children, const string& pattern) {
        int u = 0;
        for (int i = 0; i < (int)pattern.length(); i++) {
            u = sa.state[u].getNext(SuffixAutomatonAM::ch2i(pattern[i]));
            if (u <= 0)
                return{};
        }

        vector<int> res;
        dfsFind(children, (int)pattern.length(), res, u);
        res.erase(unique(res.begin(), res.end()), res.end());

        return res;
    }

    // 7. count number of occurrences
    vector<int> makeCountTable() {
        vector<pair<int, int>> base;
        vector<int> cnt(sa.N);

        for (int i = 1; i < sa.N; i++) {
            cnt[i] = !sa.state[i].cloned;
            base.emplace_back(sa.state[i].len, i);
        }
        sort(base.begin(), base.end());

        for (int i = (int)base.size() - 1; i >= 0; i--)
            cnt[sa.state[base[i].second].suffixLink] += cnt[base[i].second];

        return cnt;
    }

    int countPattern(const vector<int>& countTable, const string& pattern) {
        int u = 0;
        for (int i = 0; i < (int)pattern.length(); i++) {
            u = sa.state[u].getNext(SuffixAutomatonAM::ch2i(pattern[i]));
            if (u <= 0)
                return 0;
        }
        return countTable[u];
    }

    // 8. Longest common substring.

    // It's better that t's length is shorter than SuffixAutomation's length
    int lengthOfLcs(string& t) {
        int v = 0, len = 0, bestPos = 0, bestLen = 0;
        for (int i = 0; i < (int)t.length(); ++i) {
            int ch = SuffixAutomatonAM::ch2i(t[i]);
            while (v && !sa.state[v].hasNext(ch)) {
                v = sa.state[v].suffixLink;
                len = sa.state[v].len;
            }
            if (sa.state[v].hasNext(ch)) {
                v = sa.state[v].getNext(ch);
                ++len;
            }
            if (len > bestLen) {
                bestLen = len;
                bestPos = i;
            }
        }
        return bestLen;
    }

    string lcs(string& t) {
        int v = 0, len = 0, bestPos = 0, bestLen = 0;
        for (int i = 0; i < (int)t.length(); ++i) {
            int ch = SuffixAutomatonAM::ch2i(t[i]);
            while (v && !sa.state[v].hasNext(ch)) {
                v = sa.state[v].suffixLink;
                len = sa.state[v].len;
            }
            if (sa.state[v].hasNext(ch)) {
                v = sa.state[v].getNext(ch);
                ++len;
            }
            if (len > bestLen) {
                bestLen = len;
                bestPos = i;
            }
        }
        return t.substr(bestPos - bestLen + 1, bestLen);
    }

    // 9. Longest common substring of multiple substring.
    // step1) make SuffixAutomation instances of all strings
    // step2) call lengthOfLcs(sa[L], str[R]) - length(str[R]) <= length(str[L])
    //        if length(str[L]) < length(str[R]) then swap(L, R)

    // 10. Find whether a given string s is a suffix of sa.
    bool isSuffix(const string& s) {
        vector<int> term = sa.getTerminals();

        int u = 0;
        for (int i = 0; i < (int)s.length(); i++) {
            u = sa.state[u].getNext(SuffixAutomatonAM::ch2i(s[i]));
            if (u <= 0)
                return false;
        }
        return find(term.begin(), term.end(), u) != term.end();
    }

    bool isSuffix(vector<int>& term, const string& s) {
        int u = 0;
        for (int i = 0; i < (int)s.length(); i++) {
            u = sa.state[u].getNext(SuffixAutomatonAM::ch2i(s[i]));
            if (u <= 0)
                return false;
        }
        return find(term.begin(), term.end(), u) != term.end();
    }

    bool isSuffixWithSortedTerm(vector<int>& term, const string& s) {
        int u = 0;
        for (int i = 0; i < (int)s.length(); i++) {
            u = sa.state[u].getNext(SuffixAutomatonAM::ch2i(s[i]));
            if (u <= 0)
                return false;
        }
        return binary_search(term.begin(), term.end(), u);
    }
};