#pragma once

#include <algorithm>

// Implicit Treap
template <typename T = int, typename MergeOp = function<T(T, T)>, typename BlockOp = function<T(T, int)>>
struct TreapRangeQueryEx {
    enum LazyT {
        lzNone,
        lzSet = 0x01,
        lzAdd = 0x02,
        lzRevF = 0x04
    };

    struct Node {
        Node* parent;
        Node* left;
        Node* right;

        int   cnt;

        int   priority;
        T     value;
        T     rangeValue;

        int   lazyType;
        T     lazy;

        void init() {
            parent = left = right = nullptr;
            priority = TreapRangeQueryEx<T, MergeOp, BlockOp>::random();
            cnt = 1;
            lazyType = lzNone;
        }
    };

    T         defaultValue;
    MergeOp   mergeOp;
    BlockOp   blockOp;

    int       count;
    Node*     tree;

    explicit TreapRangeQueryEx(T dflt = T())
            : defaultValue(dflt), mergeOp(), blockOp() {
        count = 0;
        tree = nullptr;
    }

    TreapRangeQueryEx(MergeOp mop, BlockOp bop, T dflt = T())
            : defaultValue(dflt), mergeOp(mop), blockOp(bop) {
        count = 0;
        tree = nullptr;
    }

    ~TreapRangeQueryEx() {
        deleteRecursive(tree);
    }

    int size() const {
        return count;
    }

    //--- 

    // O(N)
    // precondition : A must be sorted
    void build(const T A[], int N) {
        tree = recBuild(A, 0, N - 1);
        count = N;
    }

    // O(N)
    // precondition : A must be sorted
    void build(const vector<T>& A) {
        build(A.data(), int(A.size()));
    }


    void update(int index, T value) {
        update(index, index, value);
    }

    // inclusive
    void update(int left, int right, T value) {
        if (count <= 0 || left > right || right < 0 || left >= count)
            return;

        left = max(0, left);
        right = min(count - 1, right);

        auto rs = split(tree, right + 1);
        auto ls = split(rs.first, left);

        if (ls.second) {
            pushDownTo(ls.second, lzSet, value);
            tree = merge(merge(ls.first, ls.second), rs.second);
            if (tree)
                tree->parent = nullptr;
        }
    }


    void add(int index, T value) {
        add(index, index, value);
    }

    // inclusive
    void add(int left, int right, T value) {
        if (count <= 0 || left > right || right < 0 || left >= count)
            return;

        left = max(0, left);
        right = min(count - 1, right);

        auto rs = split(tree, right + 1);
        auto ls = split(rs.first, left);

        if (ls.second) {
            pushDownTo(ls.second, lzAdd, value);
            tree = merge(merge(ls.first, ls.second), rs.second);
            if (tree)
                tree->parent = nullptr;
        }
    }


    void reverse(int left, int right) {
        if (count <= 0 || left > right || right < 0 || left >= count)
            return;

        left = max(0, left);
        right = min(count - 1, right);

        auto rs = split(tree, right + 1);
        auto ls = split(rs.first, left);

        if (ls.second) {
            ls.second->lazyType ^= lzRevF;
            tree = merge(merge(ls.first, ls.second), rs.second);
            if (tree)
                tree->parent = nullptr;
        }
    }

    //---

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

    vector<T> serialize() {
        vector<T> res;
        serialize(tree, res);
        return res;
    }

    //-------------------------------------------------------------------------

    Node* insert(int index, const T& value) {
        index = max(0, min(count, index));

        Node* p = createNode(value);

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


    void heapify(Node* p) {
        if (!p)
            return;

        Node* maxNode = p;
        if (p->left != nullptr && p->left->priority > maxNode->priority)
            maxNode = p->left;
        if (p->right != nullptr && p->right->priority > maxNode->priority)
            maxNode = p->right;
        if (maxNode != p) {
            swap(p->priority, maxNode->priority);
            heapify(maxNode);
        }
    }

    Node* recBuild(const T A[], int L, int R) {
        if (L > R)
            return nullptr;

        int mid = L + (R - L) / 2;
        Node* p = createNode(A[mid]);

        p->left = recBuild(A, L, mid - 1);
        if (p->left)
            p->left->parent = p;
        p->right = recBuild(A, mid + 1, R);
        if (p->right)
            p->right->parent = p;

        heapify(p);
        update(p);
        return p;
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

    //---

    static int random() {
        //static std::random_device rd;
        //static std::mt19937 eng(rd());
        static std::mt19937 eng(7);
        static std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());
        return dist(eng);
    }
};

template <typename T, typename MergeOp, typename BlockOp>
inline TreapRangeQueryEx<T, MergeOp, BlockOp> makeTreapRangeQueryEx(MergeOp mop, BlockOp bop, T dfltValue = T()) {
    return TreapRangeQueryEx<T, MergeOp, BlockOp>(mop, bop, dfltValue);
}
