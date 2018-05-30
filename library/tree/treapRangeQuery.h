#pragma once

#include <algorithm>

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
            priority = (rand() & 0x7fff) * (rand() & 0x7fff);
            cnt = 1;
            lazyExist = false;
        }
    };

    T         defaultValue;
    MergeOp   mergeOp;
    BlockOp   blockOp;

    int       count;
    Node*     tree;

    explicit TreapRangeQuery(T dflt = T())
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
        x->lazy = defaultValue;
        x->lazyExist = false;
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline TreapRangeQuery<T, MergeOp, BlockOp> makeTreapRangeQuery(MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return TreapRangeQuery<T, MergeOp, BlockOp>(mop, bop, dfltValue);
}
