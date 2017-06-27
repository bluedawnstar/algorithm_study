#pragma once

#include <algorithm>

template <typename T>
struct Treap {
    struct Node {
        Node* parent;
        Node* left;
        Node* right;

        int   priority;
        int   cnt;

        T     value;

        void init() {
            parent = left = right = nullptr;
            priority = rand() * rand();
            cnt = 1;
        }
    };

    int     count;
    Node*   tree;

    Treap() {
        count = 0;
        tree = nullptr;
    }

    ~Treap() {
        deleteRecursive(tree);
    }

    int size() const {
        return count;
    }

    Node* find(const T& key) {
        Node *p = tree;

        while (p && !(p->value == key)) {
            if (key < p->value)
                p = p->left;
            else
                p = p->right;
        }

        return p;
    }

    Node* operator [](int nth) {
        assert((tree ? tree->cnt : 0) == count);

        if (nth < 0 || nth >= count)
            return nullptr;

        int n = nth;
        Node* p = tree;
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

    int indexOf(Node* p) {
        assert((tree ? tree->cnt : 0) == count);

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

    Node* insert(const T& value) {
        Node* p = createNode(value);
        tree = insert(tree, p);
        tree->parent = nullptr;
        return p;
    }

    bool erase(const T& key) {
        tree = erase(tree, key);
        if (tree)
            tree->parent = nullptr;
        return true;
    }

protected:
    Node* createNode(const T& item) {
        Node* p = new Node();
        p->init();
        p->value = item;
        count++;
        return p;
    }

    void destroyNode(Node* node) {
        count--;
        delete node;
    }

    void deleteRecursive(Node* node) {
        if (node != nullptr) {
            deleteRecursive(node->left);
            deleteRecursive(node->right);
            destroyNode(node);
        }
    }


    pair<Node*, Node*> split(Node* root, const T& key) {
        if (root == nullptr)
            return make_pair(nullptr, nullptr);

        if (root->value < key) {
            auto rs = split(root->right, key);
            root->right = rs.first;
            if (rs.first)
                rs.first->parent = root;
            update(root);
            return make_pair(root, rs.second);
        } else {
            auto ls = split(root->left, key);
            root->left = ls.second;
            if (ls.second)
                ls.second->parent = root;
            update(root);
            return make_pair(ls.first, root);
        }
    }

    Node* insert(Node* root, Node* node) {
        if (root == nullptr)
            return node;

        if (root->priority < node->priority) {
            auto splitted = split(root, node->value);
            node->left = splitted.first;
            node->right = splitted.second;
            if (splitted.first)
                splitted.first->parent = node;
            if (splitted.second)
                splitted.second->parent = node;
            update(node);
            return node;
        }

        if (node->value < root->value) {
            root->left = insert(root->left, node);
            if (root->left)
                root->left->parent = root;
        } else {
            root->right = insert(root->right, node);
            if (root->right)
                root->right->parent = root;
        }
        update(root);
        return root;
    }


    Node* merge(Node* a, Node* b) {
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
        } else {
            a->right = merge(a->right, b);
            if (a->right)
                a->right->parent = a;
            update(a);
            return a;
        }
    }

    Node* erase(Node* root, const T& key) {
        if (!root)
            return root;
        
        if (root->value == key) {
            Node* res = merge(root->left, root->right);
            destroyNode(root);
            return res;
        }

        if (key < root->value) {
            root->left = erase(root->left, key);
            if (root->left)
                root->left->parent = root;
        } else {
            root->right = erase(root->right, key);
            if (root->right)
                root->right->parent = root;
        }
        update(root);
        return root;
    }


    void updateCnt(Node* x) {
        x->cnt = 1;
        if (x->left)
            x->cnt += x->left->cnt;
        if (x->right)
            x->cnt += x->right->cnt;
    }

    void update(Node* x) {
        updateCnt(x);

        //TODO: add custom actions
    }
};


//NOTE: Using SplayTreeRangeQuery is recommended instead of TreapRangeQuery,
//      because SplayTreeRangeQuery is faster than TreapRangeQuery.
template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct TreapRangeQuery {
    struct Node {
        Node* parent;
        Node* left;
        Node* right;

        int   priority;
        int   cnt;

        T     value;
        T     rangeValue;

        T     lazy;
        bool  lazyExist;

        void init() {
            parent = left = right = nullptr;
            priority = rand() * rand();
            cnt = 1;
            lazyExist = false;
        }
    };

    T         defaultValue;
    MergeOp   mergeOp;
    BlockOp   blockOp;

    int       count;
    Node*     tree;

    TreapRangeQuery(T dflt = T())
        : defaultValue(dflt), mergeOp(), blockOp() {
        count = 0;
        tree = nullptr;
    }

    TreapRangeQuery(MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        count = 0;
        tree = nullptr;
    }

    ~TreapRangeQuery() {
        deleteRecursive(tree);
    }

    int size() const {
        return count;
    }

    void update(int index, int value) {
        update(index, index, value);
    }

    // inclusive
    void update(int left, int right, int value) {
        if (count <= 0 || left > right || right < 0 || left >= count)
            return;

        left = max(0, left);
        right = min(count - 1, right);

        auto rs = split(tree, right + 1);
        auto ls = split(rs.first, left);

        if (ls.second) {
            ls.second->lazyExist = true;
            ls.second->value = ls.second->lazy = value;
            ls.second->rangeValue = blockOp(value, ls.second->cnt);
            tree = merge(merge(ls.first, ls.second), rs.second);
            if (tree)
                tree->parent = nullptr;
        }
    }

    T query(int index) {
        return query(index, index);
    }

    // inclusive
    T query(int left, int right) {
        if (count <= 0 || left > right || right < 0 || left >= count)
            return defaultValue;

        left = max(0, left);
        right = min(count - 1, right);

        auto rs = split(tree, right + 1);
        auto ls = split(rs.first, left);

        if (!ls.second)
            return defaultValue;

        T res = ls.second->rangeValue;

        tree = merge(merge(ls.first, ls.second), rs.second);
        if (tree)
            tree->parent = nullptr;

        return res;
    }

    //-------------------------------------------------------------------------

    Node* insert(int index, const T& value) {
        Node* p = createNode(value);

        index = max(0, min(count, index));

        auto t = split(tree, index);
        tree = merge(merge(t.first, p), t.second);
        if (tree)
            tree->parent = nullptr;

        return p;
    }

    bool erase(int index) {
        if (index < 0 || index >= count)
            return false;

        auto t = split(tree, index);
        auto t2 = split(t.second, index + 1 - (t.first ? t.first->cnt : 0));
        tree = merge(t.first, t2.second);
        if (tree)
            tree->parent = nullptr;

        destroyNode(t2.first);

        return true;
    }

    const Node* operator [](int nth) const {
        assert((tree ? tree->cnt : 0) == count);

        if (nth < 0 || nth >= count)
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
        assert((tree ? tree->cnt : 0) == count);

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
    Node* createNode(const T& value) {
        Node* p = new Node();
        p->init();
        p->rangeValue = p->value = value;
        p->lazy = defaultValue;
        count++;
        return p;
    }

    void destroyNode(Node* node) {
        count--;
        delete node;
    }

    void deleteRecursive(Node* node) {
        if (node != nullptr) {
            deleteRecursive(node->left);
            deleteRecursive(node->right);
            destroyNode(node);
        }
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
        } else {
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
        } else {
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
        x->rangeValue = x->value;
        if (x->left)
            x->rangeValue = mergeOp(x->rangeValue, x->left->rangeValue);
        if (x->right)
            x->rangeValue = mergeOp(x->rangeValue, x->right->rangeValue);
    }

    void update(Node* x) {
        updateCnt(x);
        updateValue(x);
    }

    void pushDown(Node* x) {
        if (!x || !x->lazyExist)
            return;

        if (x->left) {
            x->left->lazyExist = true;
            x->left->value = x->left->lazy = x->lazy;
            x->left->rangeValue = blockOp(x->lazy, x->left->cnt);
        }
        if (x->right) {
            x->right->lazyExist = true;
            x->right->value = x->right->lazy = x->lazy;
            x->right->rangeValue = blockOp(x->lazy, x->right->cnt);
        }
        x->lazyExist = false;
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline TreapRangeQuery<T, MergeOp, BlockOp> makeTreapRangeQuery(MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return std::move(TreapRangeQuery<T, MergeOp, BlockOp>(mop, bop, dfltValue));
}
