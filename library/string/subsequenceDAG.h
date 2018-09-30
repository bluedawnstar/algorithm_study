#pragma once

// Directed Acyclic Graph to recognize all the subsequences of a string.

struct SubsequenceDAG {
    static const int MaxCharN = 26;
    static int ch2i(int c) { return c - 'a'; }
    static int i2ch(int i) { return i + 'a'; }

    struct NodeT {
        int next[MaxCharN]; // 0'th is null node

        NodeT() {
            memset(next, 0, sizeof(next));
        }
    };
    
    vector<NodeT> nodes;
    int nodeN;

    int root;
    int last[MaxCharN];

    SubsequenceDAG() : nodeN(0), root(0) {
        memset(last, 0, sizeof(last));
    }

    explicit SubsequenceDAG(int maxN) {
        init(maxN);
    }

    SubsequenceDAG(const string& s) {
        init(int(s.length()));
        build(s);
    }

    SubsequenceDAG(const char s[], int n) {
        init(n);
        build(s, n);
    }

    void init(int maxN) {
        nodes = vector<NodeT>(max(1, maxN));
        nodeN = 1;
        root = 0;
        memset(last, 0, sizeof(last));
    }

    void build(const string& s) {
        init(int(s.length()) * 2 + 1);
        for (auto ch : s)
            add(ch);
    }

    void build(const char s[], int n) {
        init(n * 2 + 1);
        for (int i = 0; i < n; i++)
            add(s[i]);
    }


    void add(char ch) {
        int nodeIndex = nodeN++;

        int chIdx = ch2i(ch);
        for (int i = last[chIdx]; i < nodeIndex; i++) {
            if (!nodes[i].next[chIdx])
                nodes[i].next[chIdx] = nodeIndex;
        }

        last[chIdx] = nodeIndex;
    }


    template <typename FuncT = function<void(int,string)>>
    void iterateAll(const FuncT& f) {
        string s;
        dfsIterate(0, s, f);
    }

private:
    template <typename FuncT = function<void(int, string)>>
    void dfsIterate(int nodeIndex, string& s, const FuncT& f) {
        f(nodeIndex, s);
        for (int i = 0; i < MaxCharN; i++) {
            if (!nodes[nodeIndex].next[i])
                continue;

            s.push_back(i2ch(i));
            dfsIterate(nodes[nodeIndex].next[i], s, f);
            s.pop_back();
        }
    }
};
