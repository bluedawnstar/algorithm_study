#pragma once

// with Suffix Automaton
struct PalindromicTree {
    static const int MaxCharN = 26;
    static int ch2i(int c) { return c - 'a'; }

    struct NodeT {
        int start, end;     // start and end indexes of current node inclusively
        int len;            // length of substring
        int suffixLink;     // maximum palindromic suffix node for the current node
        int next[MaxCharN]; // 
        int count;          // the number of palindromic strings to be ended at the current node
    };

    string s;
    vector<NodeT> nodes;

    int nodeN;              
    int lastSuffix;         // special nodes: nodes[1] - root with len -1, nodes[2] - root with len 0

    PalindromicTree() {
    }

    explicit PalindromicTree(int maxN) {
        init(maxN);
    }

    void init(int maxN) {
        s.clear();

        nodes = vector<NodeT>(maxN + 3, NodeT{ 0, 0, 0, 0, { 0, }, 0 });
        s.reserve(maxN);

        nodeN = 2;
        lastSuffix = 2;
        nodes[1].len = -1; nodes[1].suffixLink = 1;
        nodes[2].len = 0; nodes[2].suffixLink = 1;
    }

    // return (the number of added palindrom strings, if new palindromic substring)
    pair<int,bool> extend(char ch) {
        int pos = int(s.length());
        int cur = lastSuffix, curlen = 0;
        int let = ch2i(ch);

        s += ch;

        while (true) {
            curlen = nodes[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == ch)
                break;
            cur = nodes[cur].suffixLink;
        }
        if (nodes[cur].next[let]) {
            lastSuffix = nodes[cur].next[let];
            return make_pair(nodes[lastSuffix].count, false);
        }

        lastSuffix = ++nodeN;
        nodes[cur].next[let] = nodeN;

        nodes[nodeN].len = nodes[cur].len + 2;
        nodes[nodeN].start = pos - nodes[nodeN].len + 1;
        nodes[nodeN].end = pos;

        if (nodes[nodeN].len == 1) {
            nodes[nodeN].suffixLink = 2;
            nodes[nodeN].count = 1;
            return make_pair(nodes[lastSuffix].count, true);
        }

        while (true) {
            cur = nodes[cur].suffixLink;
            curlen = nodes[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) {
                nodes[nodeN].suffixLink = nodes[cur].next[let];
                break;
            }
        }
        nodes[nodeN].count = 1 + nodes[nodes[nodeN].suffixLink].count;

        return make_pair(nodes[lastSuffix].count, true);
    }

    int countLastSuffixPalindromes() const {
        return nodes[lastSuffix].count;
    }

    vector<string> getLastSuffixPalindromes() const {
        vector<string> pals;
        int curr = lastSuffix;
        while (curr > 2) {
            pals.push_back(s.substr(nodes[curr].start, nodes[curr].len));
            curr = nodes[curr].suffixLink;
        }
        return pals;
    }

    //---

    long long count(const string& s) {
        long long res = 0;
        for (char ch : s)
            res += get<0>(extend(ch));
        return res;
    }

    long long count(const char* s, int N) {
        long long res = 0;
        while (N-- > 0)
            res += get<0>(extend(*s++));
        return res;
    }

    // result[i] = count(s, i + 1) - count(s, i)
    vector<int> countAll(const string& s) {
        vector<int> res;
        res.reserve(s.size());
        for (char ch : s)
            res.push_back(get<0>(extend(ch)));
        return res;
    }

    // result[i] = count(s, i + 1) - count(s, i)
    vector<int> countAll(const char* s, int N) {
        vector<int> res;
        res.reserve(N);
        while (N-- > 0)
            res.push_back(get<0>(extend(*s++)));
        return res;
    }
};
