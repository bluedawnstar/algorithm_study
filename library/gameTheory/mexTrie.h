#pragma once

// http://codeforces.com/blog/entry/58316
// https://www.geeksforgeeks.org/combinatorial-game-theory-set-3-grundy-numbersnimbers-and-mex/
// https://www.geeksforgeeks.org/combinatorial-game-theory-set-2-game-nim/
// https://en.wikipedia.org/wiki/Mex_(mathematics)
// http://letuskode.blogspot.kr/2014/08/grundy-numbers.html

struct MexTrie {
    struct Node {
        Node* child[2];
        int cnt;
        Node() : cnt(0) { child[0] = child[1] = nullptr; }
    };

    Node* root;
    vector<Node*> nodes;

    MexTrie() {
        root = createNode();
    }

    ~MexTrie() {
        for (auto* p : nodes)
            delete p;
    }

    void insert(int x) {
        if (exists(x))
            return;

        Node* v = root;
        v->cnt++;
        for (int i = 30; i >= 0; i--) {
            int b = (x >> i) & 1;
            if (v->child[b] == nullptr)
                v->child[b] = createNode();
            v = v->child[b];
            v->cnt++;
        }
    }

    // return mex{A[0]^xorAdd, A[1]^xorAdd, ..., A[n-1]^xorAdd}
    int mex(int xorAdd = 0) const {
        Node *v = root;
        int m = 0;
        for (int i = 30; i >= 0; i--) {
            int b = (xorAdd >> i) & 1;
            if (v->child[b] == nullptr)
                return m;

            if (v->child[b]->cnt == (1 << i)) {
                b ^= 1;
                m ^= 1 << i;
            }
            if (v->child[b] == nullptr)
                return m;

            v = v->child[b];
        }

        //assert(false);
        return 0;
    }

private:
    Node* createNode() {
        Node* p = new Node();
        nodes.push_back(p);
        return p;
    }

    bool exists(int x) const {
        Node* v = root;
        for (int i = 30; i >= 0; i--) {
            int b = (x >> i) & 1;
            if (v->child[b] == nullptr)
                return false;
            v = v->child[b];
        }
        return true;
    }
};
