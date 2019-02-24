#pragma once

struct RollingHashTable {
    static const int P = 31;

    int maxN;
    vector<unsigned long long> hash;

    explicit RollingHashTable(int maxN) : maxN(maxN), hash(maxN + 1) {
        hash[0] = 1ull;
        for (int i = 1; i <= maxN; i++)
            hash[i] = hash[i - 1] * P;
    }

    unsigned long long operator [](int index) const {
        return hash[index];
    }
};

// for lowercase alphabet only
struct DynamicStringHashWithTreap {
    typedef unsigned long long HashT;

    struct Node {
        Node* parent;
        Node* left;
        Node* right;

        int   priority;
        int   cnt;

        int   ch;

        HashT valueL;
        HashT valueR;
        HashT rangeValueL;
        HashT rangeValueR;

        HashT lazyL;
        HashT lazyR;

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

    int   N;
    Node* tree;
    RollingHashTable& hash;

    explicit DynamicStringHashWithTreap(RollingHashTable& h) : hash(h) {
        N = 0;
        tree = nullptr;
    }

    ~DynamicStringHashWithTreap() {
        //TODO: delete nodes
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

        auto rs = split(tree, right + 1);
        auto ls = split(rs.first, left);

        if (!ls.second)
            return 0ull;

        HashT res = prefix ? ls.second->rangeValueL : ls.second->rangeValueR;

        tree = merge(merge(ls.first, ls.second), rs.second);
        if (tree)
            tree->parent = nullptr;

        return res;
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

    Node* insert(int index, int ch, HashT hashL, HashT hashR) {
        index = max(0, min(N, index));

        Node* p = createNode(ch, hashL, hashR);

        auto t = split(tree, index);
        tree = merge(merge(t.first, p), t.second);
        if (tree)
            tree->parent = nullptr;

        N++;

        return p;
    }

    // multiply rolling hash
    // inclusive, O(logN)
    void multiplyHash(int left, int right, HashT hashL, HashT hashR) {
        if (N <= 0 || left > right || right < 0 || left >= N)
            return;

        left = max(0, left);
        right = min(N - 1, right);

        auto rs = split(tree, right + 1);
        auto ls = split(rs.first, left);

        if (ls.second) {
            ls.second->valueL = (ls.second->valueL * hashL);
            ls.second->valueR = (ls.second->valueR * hashR);
            ls.second->rangeValueL = (ls.second->rangeValueL * hashL);
            ls.second->rangeValueR = (ls.second->rangeValueR * hashR);
            if (!ls.second->lazyL) {
                ls.second->lazyL = hashL;
                ls.second->lazyR = hashR;
            }
            else {
                ls.second->lazyL = (ls.second->lazyL * hashL);
                ls.second->lazyR = (ls.second->lazyR * hashR);
            }
            tree = merge(merge(ls.first, ls.second), rs.second);
            if (tree)
                tree->parent = nullptr;
        }
    }

    //--- Treap operations

    const Node* operator [](int nth) const {
        //assert((tree ? tree->cnt : 0) == N);

        if (nth < 0 || nth >= N)
            return nullptr;

        int n = nth;
        const Node* p = tree;
        while (p) {
            while (p->left && p->left->cnt > n)
                p = p->left;
            if (p->left)
                n -= p->left->cnt;
            if (!n--)
                break;
            p = p->right;
        }
        return p;
    }

    int indexOf(const Node* p) {
        //assert((tree ? tree->cnt : 0) == N);

        if (!p)
            return -1;

        int res = p->left ? p->left->cnt : 0;
        while (p->parent) {
            while (p->parent && p->parent->left == p)
                p = p->parent;
            if (p->parent) {
                p = p->parent;
                res++;
                if (p->left)
                    res += p->left->cnt;
            }
        }

        return res;
    }


protected:
    Node* createNode(int ch, HashT hashL, HashT hashR) {
        Node* p = new Node();
        p->init(ch, hashL, hashR);
        return p;
    }

    Node* append(Node* p) {
        auto t = split(tree, N);
        tree = merge(merge(t.first, p), t.second);
        if (tree)
            tree->parent = nullptr;
        N++;
        return p;
    }


    // split with subgraph's size
    pair<Node*, Node*> split(Node* root, int minRight) {
        if (root == nullptr)
            return make_pair(nullptr, nullptr);

        pushDown(root);

        int rootLeftSize = root->left ? root->left->cnt : 0;
        if (rootLeftSize < minRight) {
            auto rs = split(root->right, minRight - rootLeftSize - 1);
            root->right = rs.first;
            if (rs.first)
                rs.first->parent = root;
            update(root);
            return make_pair(root, rs.second);
        }
        else {
            auto ls = split(root->left, minRight);
            root->left = ls.second;
            if (ls.second)
                ls.second->parent = root;
            update(root);
            return make_pair(ls.first, root);
        }
    }

    Node* merge(Node* a, Node* b) {
        pushDown(a);
        pushDown(b);

        if (!a)
            return b;
        if (!b)
            return a;

        if (a->priority < b->priority) {
            b->left = merge(a, b->left);
            if (b->left)
                b->left->parent = b;
            update(b);
            return b;
        }
        else {
            a->right = merge(a->right, b);
            if (a->right)
                a->right->parent = a;
            update(a);
            return a;
        }
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
