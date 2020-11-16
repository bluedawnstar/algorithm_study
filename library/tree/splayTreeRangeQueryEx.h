#pragma once

#include <algorithm>

template <typename T, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct SplayTreeRangeQueryEx {
    enum LazyT {
        lzNone,
        lzSet  = 0x01,
        lzAdd  = 0x02,
        lzRevF = 0x04
    };

    struct Node {
        Node* parent;
        Node* left;
        Node* right;

        int   cnt;
        T     value;
        T     rangeValue;

        int   lazyType;
        T     lazy;

        void init() {
            parent = left = right = nullptr;
            cnt = 1;
            lazyType = lzNone;
        }
    };

    T         defaultValue;
    MergeOp   mergeOp;
    BlockOp   blockOp;

    int       count;
    Node*     tree;

    explicit SplayTreeRangeQueryEx(T dflt = T())
        : defaultValue(dflt), mergeOp(), blockOp() {
        count = 0;
        tree = nullptr;
    }

    SplayTreeRangeQueryEx(MergeOp mop, BlockOp bop, T dflt = T())
        : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        count = 0;
        tree = nullptr;
    }

    ~SplayTreeRangeQueryEx() {
        deleteRecursive(tree);
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

    void build(const T A[], int n) {
        Node* x;

        tree = x = createNode();
        x->cnt = n;
        x->rangeValue = x->value = A[0];
        x->lazy = defaultValue;
        for (int i = 1; i < n; i++) {
            x->right = createNode();
            x->right->parent = x;

            x = x->right;
            x->left = x->right = nullptr;
            x->cnt = n - i;
            x->rangeValue = x->value = A[i];
            x->lazy = defaultValue;
        }

        while (x) {
            if (x->right)
                x->rangeValue = mergeOp(x->rangeValue, x->right->rangeValue);
            x = x->parent;
        }
    }

    void build(const vector<T>& A) {
        build(A.data(), int(A.size()));
    }

    int size() const {
        return count;
    }


    void update(int index, T value) {
        Node* x = operator [](index);
        x->rangeValue = x->value = value;
        if (x->left)
            x->rangeValue = mergeOp(x->rangeValue, x->left->rangeValue);
        if (x->right)
            x->rangeValue = mergeOp(x->rangeValue, x->right->rangeValue);
    }

    // inclusive
    void update(int left, int right, T value) {
        if (count <= 0 || left > right || right < 0 || left >= count)
            return;

        left = max(0, left);
        right = min(count - 1, right);

        Node* x = interval(left, right);
        if (x) {
            pushDownTo(x, lzSet, value);
            while (x->parent) {
                x = x->parent;
                updateValue(x);
            }
        }
    }


    void add(int index, T value) {
        Node* x = operator [](index);
        x->rangeValue = x->value += value;
        if (x->left)
            x->rangeValue = mergeOp(x->rangeValue, x->left->rangeValue);
        if (x->right)
            x->rangeValue = mergeOp(x->rangeValue, x->right->rangeValue);
    }

    // inclusive
    void add(int left, int right, T value) {
        if (count <= 0 || left > right || right < 0 || left >= count)
            return;

        left = max(0, left);
        right = min(count - 1, right);

        Node* x = interval(left, right);
        if (x) {
            pushDownTo(x, lzAdd, value);
            while (x->parent) {
                x = x->parent;
                updateValue(x);
            }
        }
    }


    // inclusive
    void reverse(int left, int right) {
        if (count <= 0 || left > right || right < 0 || left >= count)
            return;

        left = max(0, left);
        right = min(count - 1, right);

        Node* x = interval(left, right);
        if (x) {
            x->lazyType ^= lzRevF;
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

        Node* x = interval(left, right);
        return x->rangeValue;
    }

    vector<T> serialize() {
        vector<T> res;
        serialize(tree, res);
        return res;
    }

    //-------------------------------------------------------------------------

    Node* find(T key) {
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

    pair<Node*, bool> insert(T value) {
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

    bool erase(T key) {
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

    Node* interval(int left, int right) {
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
        return x;
    }

    void serialize(Node* x, vector<T>& out) {
        if (!x)
            return;
        pushDown(x);
        serialize(x->left, out);
        out.push_back(x->value);
        serialize(x->right, out);
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


    void pushDownTo(Node* x, LazyT type, T lazyValue) {
        if (!x)
            return;

        if (type == lzSet) {
            x->lazyType = (x->lazyType & lzRevF) | lzSet;
            x->value = x->lazy = lazyValue;
            x->rangeValue = blockOp(lazyValue, x->cnt);
        } else if (type == lzAdd) {
            if ((x->lazyType & (lzSet | lzAdd)) == lzNone) {
                x->lazyType = (x->lazyType & lzRevF) | lzAdd;
                x->lazy = lazyValue;
            } else {
                x->lazy += lazyValue;
            }
            x->value += lazyValue;
            x->rangeValue += blockOp(lazyValue, x->cnt);
        }
    }

    void pushDown(Node* x) {
        if (!x || x->lazyType == lzNone)
            return;

        if (x->lazyType & lzRevF) {
            swap(x->left, x->right);
            if (x->left)
                x->left->lazyType ^= lzRevF;
            if (x->right)
                x->right->lazyType ^= lzRevF;
        }

        switch (x->lazyType & (lzSet | lzAdd)) {
        case lzSet:
            pushDownTo(x->left, lzSet, x->lazy);
            pushDownTo(x->right, lzSet, x->lazy);
            break;
        case lzAdd:
            pushDownTo(x->left, lzAdd, x->lazy);
            pushDownTo(x->right, lzAdd, x->lazy);
            break;
        }

        x->lazy = defaultValue;
        x->lazyType = lzNone;
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline SplayTreeRangeQueryEx<T, MergeOp, BlockOp> makeSplayTreeRangeQueryEx(MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return SplayTreeRangeQueryEx<T, MergeOp, BlockOp>(mop, bop, dfltValue);
}
