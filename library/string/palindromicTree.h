#pragma once

const int MAXN = 105000;

struct PalindromicTree {
    static const int MaxCharN = 26;
    static int ch2i(int c) { return c - 'a'; }

    struct NodeT {
        int next[26];
        int len;
        int suffixLink;
        int count;
    };

    string s;
    vector<NodeT> tree;

    int nodeN;          // node 1 - root with len -1, node 2 - root with len 0
    int lastSuffix;

    PalindromicTree() {
    }

    explicit PalindromicTree(int maxN) {
        init(maxN);
    }

    void init(int maxN) {
        s.clear();
        tree.clear();

        tree.resize(maxN * 2 + 3);
        s.reserve(maxN);

        nodeN = 2;
        lastSuffix = 2;
        tree[1].len = -1; tree[1].suffixLink = 1;
        tree[2].len = 0; tree[2].suffixLink = 1;
    }

    pair<int,bool> extend(char ch) {
        int pos = (int)s.length();
        int cur = lastSuffix, curlen = 0;
        int let = ch2i(ch);

        s += ch;

        while (true) {
            curlen = tree[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == ch)
                break;
            cur = tree[cur].suffixLink;
        }
        if (tree[cur].next[let]) {
            lastSuffix = tree[cur].next[let];
            return make_pair(tree[lastSuffix].count, false);
        }

        lastSuffix = ++nodeN;
        tree[nodeN].len = tree[cur].len + 2;
        tree[cur].next[let] = nodeN;

        if (tree[nodeN].len == 1) {
            tree[nodeN].suffixLink = 2;
            tree[nodeN].count = 1;
            return make_pair(tree[lastSuffix].count, true);
        }

        while (true) {
            cur = tree[cur].suffixLink;
            curlen = tree[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) {
                tree[nodeN].suffixLink = tree[cur].next[let];
                break;
            }
        }

        tree[nodeN].count = 1 + tree[tree[nodeN].suffixLink].count;

        return make_pair(tree[lastSuffix].count, true);
    }

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
