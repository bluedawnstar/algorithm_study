#pragma once

#include <algorithm>

template <typename T>
struct SplayTree {
    struct Node {
        Node* parent;
        Node* left;
        Node* right;

        int   cnt;
        T     value;

        void init() {
            parent = left = right = nullptr;
            cnt = 1;
        }
    };

    int     count;
    Node*   tree;

    SplayTree() {
        count = 0;
        tree = nullptr;
    }

    ~SplayTree() {
        deleteRecursive(tree);
    }

    int size() const {
        return count;
    }

    Node* find(const T& key) {
        Node* p = tree;
        if (!p)
            return nullptr;

        bool found = false;
        while (p) {
            if (key == p->value) {
                found = true;
                break;
            }
            if (key < p->value) {
                if (!p->left)
                    break;
                p = p->left;
            } else {
                if (!p->right)
                    break;
                p = p->right;
            }
        }
        splay(p);
        return found ? p : nullptr;
    }

    Node* operator [](int nth) {
        if (nth < 0 || nth >= count)
            return nullptr;

        Node* x = tree;
        while (1) {
            while (x->left && x->left->cnt > nth)
                x = x->left;
            if (x->left)
                nth -= x->left->cnt;
            if (!nth--)
                break;
            x = x->right;
        }
        splay(x);
        return x;
    }

    int indexOf(Node* p) {
        if (p == nullptr)
            return -1;

        int res = (p->left != nullptr) ? p->left->cnt : 0;
        while (p->parent != nullptr) {
            while (p->parent != nullptr && p->parent->left == p)
                p = p->parent;
            if (p->parent != nullptr) {
                p = p->parent;
                res++;
                if (p->left != nullptr)
                    res += p->left->cnt;
            }
        }

        return res;
    }

    pair<Node*, bool> insert(const T& value) {
        Node* p = tree;
        Node** pp = nullptr;

        if (!p) {
            Node* x = createNode();
            tree = x;
            x->left = x->right = x->parent = nullptr;
            x->value = value;
            return make_pair(x, true);
        }

        while (1) {
            if (value == p->value)
                return make_pair(p, false);

            if (value < p->value) {
                if (!p->left) {
                    pp = &p->left;
                    break;
                }
                p = p->left;
            } else {
                if (!p->right) {
                    pp = &p->right;
                    break;
                }
                p = p->right;
            }
        }
        Node* x = createNode();
        *pp = x;
        x->left = x->right = nullptr;
        x->parent = p;
        x->value = value;
        splay(x);
        return make_pair(x, true);
    }

    bool erase(const T& key) {
        if (!find(key))
            return false;

        Node* p = tree;
        if (p->left) {
            if (p->right) {
                tree = p->left;
                tree->parent = nullptr;
                Node *x = tree;
                while (x->right)
                    x = x->right;
                x->right = p->right;
                p->right->parent = x;
                update(x);
                splay(x);
            } else {
                tree = p->left;
                tree->parent = nullptr;
            }
        } else if (p->right) {
            tree = p->right;
            tree->parent = nullptr;
        } else {
            tree = nullptr;
        }
        destroyNode(p);
        return true;
    }

protected:
    Node* createNode() {
        count++;
        return new Node();
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
        } else {
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

    void update(Node* x) {
        updateCnt(x);

        //TODO: add custom actions
    }
};

template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct SplayTreeRangeQuery {
    struct Node {
        Node* parent;
        Node* left;
        Node* right;

        int   cnt;
        T     value;
        T     rangeValue;

        T     lazy;
        bool  lazyExist;

        void init() {
            parent = left = right = nullptr;
            cnt = 1;
            lazyExist = false;
        }
    };

    T         defaultValue;
    MergeOp   mergeOp;
    BlockOp   blockOp;

    int       count;
    Node*     tree;

    SplayTreeRangeQuery(T dflt = T())
        : defaultValue(dflt), mergeOp(), blockOp() {
        count = 0;
        tree = nullptr;
    }

    SplayTreeRangeQuery(MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        count = 0;
        tree = nullptr;
    }

    ~SplayTreeRangeQuery() {
        deleteRecursive(tree);
    }

    int size() const {
        return count;
    }

    void build(int n) {
        Node *x;
        
        tree = x = createNode();
        x->cnt = n;
        x->rangeValue = x->value = x->lazy = defaultValue;
        for (int i = 1; i < n; i++) {
            x->right = createNode();
            x->right->parent = x;

            x = x->right;
            x->left = x->right = nullptr;
            x->cnt = n - i;
            x->rangeValue = x->value = x->lazy = defaultValue;
        }
    }

    void update(int index, int value) {
        Node* x = operator [](index);
        x->rangeValue = x->value = value;
        if (x->left)
            x->rangeValue = mergeOp(x->rangeValue, x->left->rangeValue);
        if (x->right)
            x->rangeValue = mergeOp(x->rangeValue, x->right->rangeValue);
    }

    // inclusive
    void update(int left, int right, int value) {
        if (count <= 0 || left > right || right < 0 || left >= count)
            return;

        left = max(0, left);
        right = min(count - 1, right);

        Node* x = nullptr;
        if (left == 0) {
            if (right == count - 1)
                x = tree;
            else {
                x = operator [](right + 1);
                x = x->left;
            }
        } else {
            if (right == count - 1) {
                x = operator [](left - 1);
                x = x->right;
            } else {
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
            x->lazyExist = true;
            x->value = x->lazy = value;
            x->rangeValue = blockOp(value, x->cnt);

            while (x->parent) {
                x = x->parent;
                updateValue(x);
            }
        }
    }

    T query(int index) {
        if (count <= 0 || index < 0 || index >= count)
            return defaultValue;
        return operator [](index)->value;
    }

    // inclusive
    T query(int left, int right) {
        if (count <= 0 || left > right || right < 0 || left >= count)
            return defaultValue;

        left = max(0, left);
        right = min(count - 1, right);

        if (left == 0) {
            if (right == count - 1)
                return tree->rangeValue;
            else {
                Node* x = operator [](right + 1);
                return x->left->rangeValue;
            }
        } else {
            if (right == count - 1) {
                Node* x = operator [](left - 1);
                return x->right->rangeValue;
            } else {
                Node* x = operator [](left - 1);
                tree = x->right;
                tree->parent = nullptr;

                operator [](right - left + 1);
                x->right = tree;
                tree->parent = x;
                tree = x;

                return x->right->left->rangeValue;
            }
        }
    }

    //-------------------------------------------------------------------------

    Node* find(const T& key) {
        Node* p = tree;
        if (!p)
            return nullptr;

        bool found = false;
        while (p) {
            pushDown(p);
            if (key == p->value) {
                found = true;
                break;
            }
            if (key < p->value) {
                if (!p->left)
                    break;
                p = p->left;
            } else {
                if (!p->right)
                    break;
                p = p->right;
            }
        }
        splay(p);
        return found ? p : nullptr;
    }

    Node* operator [](int nth) {
        if (nth < 0 || nth >= count)
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

    int indexOf(Node* p) {
        if (p == nullptr)
            return -1;

        int res = (p->left != nullptr) ? p->left->cnt : 0;
        while (p->parent != nullptr) {
            while (p->parent != nullptr && p->parent->left == p)
                p = p->parent;
            if (p->parent != nullptr) {
                p = p->parent;
                res++;
                if (p->left != nullptr)
                    res += p->left->cnt;
            }
        }

        return res;
    }

    pair<Node*, bool> insert(const T& value) {
        Node* p = tree;
        Node** pp = nullptr;

        if (!p) {
            Node* x = createNode();
            tree = x;
            x->left = x->right = x->parent = nullptr;
            x->rangeValue = x->value = value;
            return make_pair(x, true);
        }

        while (1) {
            if (value == p->value)
                return make_pair(p, false);

            if (value < p->value) {
                if (!p->left) {
                    pp = &p->left;
                    break;
                }
                pushDown(p);
                p = p->left;
            } else {
                if (!p->right) {
                    pp = &p->right;
                    break;
                }
                pushDown(p);
                p = p->right;
            }
        }
        Node* x = createNode();
        *pp = x;
        x->left = x->right = nullptr;
        x->parent = p;
        x->rangeValue = x->value = value;

        pushDown(p);
        pushDown(x);
        splay(x);
        return make_pair(x, true);
    }

    bool erase(const T& key) {
        if (!find(key))
            return false;

        Node* p = tree;
        if (p->left) {
            if (p->right) {
                tree = p->left;
                tree->parent = nullptr;

                Node *x = tree;
                while (x->right) {
                    pushDown(x);
                    x = x->right;
                }
                x->right = p->right;
                pushDown(x);
                update(x);

                p->right->parent = x;
                splay(x);
            } else {
                tree = p->left;
                tree->parent = nullptr;
                pushDown(tree);
            }
        } else if (p->right) {
            tree = p->right;
            tree->parent = nullptr;
            pushDown(tree);
        } else {
            tree = nullptr;
        }
        destroyNode(p);
        return true;
    }

protected:
    Node* createNode() {
        count++;
        return new Node();
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
        } else {
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
inline SplayTreeRangeQuery<T, MergeOp, BlockOp> makeSplayTreeRangeQuery(MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return std::move(SplayTreeRangeQuery<T, MergeOp, BlockOp>(mop, bop, dfltValue));
}
