#pragma once

#include "dynamicStringHashWithTreap.h"

// for lowercase alphabet only
struct DynamicStringHashWithSplayTree {
    typedef unsigned long long HashT;

    struct Node {
        Node* parent;
        Node* left;
        Node* right;

        int   priority;
        int   cnt;

        int   ch;

        HashT     valueL;
        HashT     valueR;
        HashT     rangeValueL;
        HashT     rangeValueR;

        HashT     lazyL;
        HashT     lazyR;

        void init(int c, HashT hashL, HashT hashR) {
            parent = left = right = nullptr;
            priority = (rand() & 0x7fff) * (rand() & 0x7fff);

            cnt = 1;
            ch = c;

            rangeValueL = valueL = hashL * c;
            rangeValueR = valueR = hashR * c;
            lazyL = lazyR = 0;
        }

        Node* next() const {
            const Node* node = this;
            if (node->right != nullptr)
                return node->right->minimum();
            else {
                Node *cur, *parent;

                cur = const_cast<Node*>(node);
                parent = node->parent;
                while ((parent != nullptr) && (cur == parent->right)) {
                    cur = parent;
                    parent = parent->parent;
                }
                return parent;
            }
        }

        Node* prev() const {
            const Node* node = this;
            if (node->left != nullptr)
                return node->left->maximum();
            else {
                Node *cur, *parent;

                cur = const_cast<Node*>(node);
                parent = node->parent;
                while ((parent != nullptr) && (cur == parent->left)) {
                    cur = parent;
                    parent = parent->parent;
                }
                return parent;
            }
        }

        Node* minimum() const {
            Node* p = const_cast<Node*>(this);
            while (p->left != nullptr)
                p = p->left;
            return p;
        }

        Node* maximum() const {
            Node* p = const_cast<Node*>(this);
            while (p->right != nullptr)
                p = p->right;
            return p;
        }
    };

    int N;
    Node* tree;
    RollingHashTable& hash;

    explicit DynamicStringHashWithSplayTree(RollingHashTable& h) : hash(h) {
        N = 0;
        tree = nullptr;
    }


    const Node* begin() const {
        return tree ? tree->minimum() : nullptr;
    }

    Node* begin() {
        return tree ? tree->minimum() : nullptr;
    }

    // inclusive
    const Node* end() const {
        return tree ? tree->maximum() : nullptr;
    }

    // inclusive
    Node* end() {
        return tree ? tree->maximum() : nullptr;
    }


    //--- query

    // inclusive, O(logN)
    HashT query(int left, int right, bool prefix) {
        if (N <= 0 || left > right || right < 0 || left >= N)
            return 0ull;

        left = max(0, left);
        right = min(N - 1, right);

        if (left == 0) {
            if (right == N - 1)
                return prefix ? tree->rangeValueL : tree->rangeValueR;
            else {
                Node* x = operator [](right + 1);
                return prefix ? x->left->rangeValueL : x->left->rangeValueR;
            }
        }
        else {
            if (right == N - 1) {
                Node* x = operator [](left - 1);
                return prefix ? x->right->rangeValueL : x->right->rangeValueR;
            }
            else {
                Node* x = operator [](left - 1);
                tree = x->right;
                tree->parent = nullptr;

                operator [](right - left + 1);
                x->right = tree;
                tree->parent = x;
                tree = x;

                return prefix ? x->right->left->rangeValueL : x->right->left->rangeValueR;
            }
        }
    }

    // inclusive, O(logN)
    HashT query(bool prefix) {
        return prefix ? tree->rangeValueL : tree->rangeValueR;
    }


    // O(N)
    bool isPalindromSlow() {
        if (!tree)
            return false;

        auto* it = begin();
        auto* itR = end();
        int n2 = N / 2;
        while (n2-- > 0) {
            if (it->ch != itR->ch)
                return false;
            it = it->next();
            itR = itR->prev();
        }
        return true;
    }

    // O(1)
    bool isPalindrom() const {
        return tree && tree->rangeValueL == tree->rangeValueR;
    }

    //--- update

    // inclusive, O(logN)
    // 0 <= pos <= N
    void insert(int pos, char ch) {
        if (pos > 0)
            multiplyHash(0, pos - 1, hash[0], hash[1]);
        if (pos < N)
            multiplyHash(pos, N - 1, hash[1], hash[0]);

        int L = pos;
        int R = N - pos;

        //insert(pos, ch - 'a' + 1, hash[L], hash[R]);
        insert(pos, ch, hash[L], hash[R]);
    }

    void assign(const string& s) {
        int N = int(s.length());
        for (int i = 0, j = N - 1; i < N; i++, j--)
            insert(i, s[i], hash[i], hash[j]);
    }

    //--- Hash operations

    void insert(int pos, int ch, HashT hashL, HashT hashR) {
        if (!tree) {
            tree = createNode(ch, hashL, hashR);
            N++;
            return;
        }

        Node* x = createNode(ch, hashL, hashR);
        if (pos >= N) {
            append(x);
            return;
        }

        Node* p = operator[](pos);
        if (!p->left) {
            p->left = x;
        }
        else {
            p = p->left;
            pushDown(p);
            while (p->right) {
                p = p->right;
                pushDown(p);
            }
            p->right = x;
        }
        x->parent = p;

        //pushDown(p);
        //pushDown(x);
        splay(x);

        N++;
    }

    // multiply rolling hash
    // inclusive, O(logN)
    void multiplyHash(int left, int right, HashT hashL, HashT hashR) {
        if (N <= 0 || left > right || right < 0 || left >= N)
            return;

        left = max(0, left);
        right = min(N - 1, right);

        Node* x = nullptr;
        if (left == 0) {
            if (right == N - 1)
                x = tree;
            else {
                x = operator [](right + 1);
                x = x->left;
            }
        }
        else {
            if (right == N - 1) {
                x = operator [](left - 1);
                x = x->right;
            }
            else {
                x = operator [](left - 1);
                tree = x->right;
                tree->parent = nullptr;

                operator [](right - left + 1);
                x->right = tree;
                tree->parent = x;
                tree = x;

                x = x->right->left;
            }
        }

        if (x) {
            pushDown(x);

            x->valueL = (x->valueL * hashL);
            x->valueR = (x->valueR * hashR);
            x->rangeValueL = (x->rangeValueL * hashL);
            x->rangeValueR = (x->rangeValueR * hashR);
            if (!x->lazyL) {
                x->lazyL = hashL;
                x->lazyR = hashR;
            }
            else {
                x->lazyL = (x->lazyL * hashL);
                x->lazyR = (x->lazyR * hashR);
            }

            while (x->parent) {
                x = x->parent;
                updateValue(x);
            }
        }
    }

    //--- Splay-Tree operations

    Node* operator [](int nth) {
        if (nth < 0 || nth >= N)
            return nullptr;

        Node* x = tree;
        while (1) {
            pushDown(x);
            while (x->left && x->left->cnt > nth) {
                x = x->left;
                pushDown(x);
            }
            if (x->left)
                nth -= x->left->cnt;
            if (!nth--)
                break;
            x = x->right;
        }
        splay(x);
        return x;
    }

protected:
    Node* createNode(int ch, HashT hashL, HashT hashR) {
        Node* p = new Node();
        p->init(ch, hashL, hashR);
        return p;
    }

    void append(Node* x) {
        N++;

        if (!tree) {
            tree = x;
            return;
        }

        Node* p = tree;

        pushDown(p);
        while (p->right) {
            p = p->right;
            pushDown(p);
        }
        p->right = x;
        x->parent = p;

        //pushDown(p);
        //pushDown(x);
        splay(x);
    }


    void splay(Node* x) {
        while (x->parent) {
            Node* p = x->parent;
            Node* g = p->parent;
            if (g)
                rotate((x == p->left) == (p == g->left) ? p : x);
            rotate(x);
        }
    }

    void rotate(Node* x) {
        Node* p = x->parent;
        Node* child;
        if (x == p->left) {
            p->left = child = x->right;
            x->right = p;
        }
        else {
            p->right = child = x->left;
            x->left = p;
        }
        x->parent = p->parent;
        p->parent = x;
        if (child)
            child->parent = p;

        if (!x->parent)
            tree = x;
        else if (p == x->parent->left)
            x->parent->left = x;
        else
            x->parent->right = x;

        update(p);
        update(x);
    }

    void updateCnt(Node* x) {
        x->cnt = 1;
        if (x->left)
            x->cnt += x->left->cnt;
        if (x->right)
            x->cnt += x->right->cnt;
    }

    void updateValue(Node* x) {
        x->rangeValueL = x->valueL;
        x->rangeValueR = x->valueR;
        if (x->left) {
            x->rangeValueL = (x->rangeValueL + x->left->rangeValueL);
            x->rangeValueR = (x->rangeValueR + x->left->rangeValueR);
        }
        if (x->right) {
            x->rangeValueL = (x->rangeValueL + x->right->rangeValueL);
            x->rangeValueR = (x->rangeValueR + x->right->rangeValueR);
        }
    }

    void update(Node* x) {
        updateCnt(x);
        updateValue(x);
    }

    void pushDown(Node* x) {
        if (!x || !x->lazyL || !x->lazyR)
            return;

        if (x->left) {
            x->left->valueL = x->left->valueL * x->lazyL;
            x->left->valueR = x->left->valueR * x->lazyR;

            x->left->rangeValueL = x->left->rangeValueL * x->lazyL;
            x->left->rangeValueR = x->left->rangeValueR * x->lazyR;

            if (!x->left->lazyL) {
                x->left->lazyL = x->lazyL;
                x->left->lazyR = x->lazyR;
            }
            else {
                x->left->lazyL = x->left->lazyL * x->lazyL;
                x->left->lazyR = x->left->lazyR * x->lazyR;
            }
        }
        if (x->right) {
            x->right->valueL = x->right->valueL * x->lazyL;
            x->right->valueR = x->right->valueR * x->lazyR;

            x->right->rangeValueL = x->right->rangeValueL * x->lazyL;
            x->right->rangeValueR = x->right->rangeValueR * x->lazyR;

            if (!x->right->lazyL) {
                x->right->lazyL = x->lazyL;
                x->right->lazyR = x->lazyR;
            }
            else {
                x->right->lazyL = x->right->lazyL * x->lazyL;
                x->right->lazyR = x->right->lazyR * x->lazyR;
            }
        }
        x->lazyL = x->lazyR = 0;
    }
};
