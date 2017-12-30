#pragma once

#include <cassert>
#include <functional>
#include <algorithm>

// T has to have below functions
//    1) operator ==()
//    2) operator <()

template <typename T>
struct RBTreeSimple {
    enum RBColor {
        rbcBlack,
        rbcRed
    };

    struct Node {
        Node*   parent;
        Node*   left;
        Node*   right;

        int     cnt;

        RBColor color;
        T       value;

        Node() {
            init();
        }

        Node(const T& item) {
            init(item);
        }

        void init() {
            parent = this;
            left = this;
            right = this;
            cnt = 0;

            color = rbcBlack;
        }

        void init(const T& item) {
            parent = nullptr;
            left = nullptr;
            right = nullptr;
            cnt = 1;

            color = rbcBlack;
            value = item;
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


        bool operator ==(const Node& rhs) const {
            return value == rhs.value;
        }

        bool operator <(const Node& rhs) const {
            return value < rhs.value;
        }
    };

    Node*       mRoot;     // root node
    bool        mDuplicate;
    int         mCount;

    //-----------------------------------------------------------

    RBTreeSimple() {
        mRoot = nullptr;
        mDuplicate = false;
        mCount = 0;
    }

    RBTreeSimple(const RBTreeSimple<T>& tree) {
        mRoot = nullptr;
        mDuplicate = false;
        mCount = 0;
        copyRecursive(tree.mRoot);
    }

    virtual ~RBTreeSimple() {
        deleteRecursive(mRoot);
    }

    RBTreeSimple<T>& operator =(const RBTreeSimple<T>& tree) {
        if (this != &tree) {
            deleteRecursive(mRoot);
            mDuplicate = tree.mDuplicate;
            mRoot = nullptr;
            copyRecursive(tree.mRoot);
        }
        return *this;
    }

    int size() const {
        return mCount;
    }

    bool empty() const {
        return mCount == 0;
    }

    bool canDuplicate() const {
        return mDuplicate;
    }

    void enableDuplicate(bool enable) {
        mDuplicate = enable;
    }

    const Node* begin() const {
        return minimum(mRoot);
    }

    Node* begin() {
        return mRoot ? mRoot->minimum() : nullptr;
    }

    // inclusive
    const Node* end() const {
        return mRoot ? mRoot->maximum() : nullptr;
    }

    // inclusive
    Node* end() {
        return maximum(mRoot);
    }

    bool exist(const T& key) const {
        return find(key) != nullptr;
    }

    Node* operator [](int index) const {
        assert((mRoot != nullptr ? mRoot->cnt : 0) == mCount);

        if (index < 0 || index >= mCount)
            return nullptr;

        int n = index;
        Node* p = mRoot;
        while (p != nullptr) {
            while (p->left != nullptr && p->left->cnt > n)
                p = p->left;
            if (p->left != nullptr)
                n -= p->left->cnt;
            if (!n--)
                break;
            p = p->right;
        }

        return p;
    }

    int indexOf(Node* p) const {
        assert((mRoot != nullptr ? mRoot->cnt : 0) == mCount);

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

    Node* find(const T& key) const {
        Node *p = mRoot;

        while (p != nullptr && !(p->value == key)) {
            if (key < p->value)
                p = p->left;
            else
                p = p->right;
        }

        return p;
    }

    Node* lowerBound(const T& key) const {
        if (mRoot == nullptr)
            return nullptr;

        Node* y = nullptr;
        Node* x = mRoot;
        while (x != nullptr) {
            if (!(x->value < key)) {
                y = x;
                x = x->left;
            } else
                x = x->right;
        }
        return y;
    }

    Node* upperBound(const T& key) const {
        if (mRoot == nullptr)
            return nullptr;

        Node* y = nullptr;
        Node* x = mRoot;
        while (x != nullptr) {
            if (key < x->value) {
                y = x;
                x = x->left;
            } else
                x = x->right;
        }
        return y;
    }


    pair<Node*, bool> insert(const T& item) {
        pair<Node*, bool> ins = insertBST(item);
        if (!ins.second)
            return ins;

        Node* x = ins.first;
        Node* y;

        x->color = rbcRed;
        while ((x != mRoot) && (x->parent && x->parent->color == rbcRed)) {
            if (x->parent == x->parent->parent->left) {
                y = x->parent->parent->right;

                if (y && y->color == rbcRed) {
                    x->parent->color = rbcBlack;
                    y->color = rbcBlack;
                    x->parent->parent->color = rbcRed;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->right) {
                        x = x->parent;
                        rotateLeft(x);
                    }

                    x->parent->color = rbcBlack;
                    x->parent->parent->color = rbcRed;
                    rotateRight(x->parent->parent);
                }
            } else {
                y = x->parent->parent->left;

                if (y && y->color == rbcRed) {
                    x->parent->color = rbcBlack;
                    y->color = rbcBlack;
                    x->parent->parent->color = rbcRed;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->left) {
                        x = x->parent;
                        rotateRight(x);
                    }

                    x->parent->color = rbcBlack;
                    x->parent->parent->color = rbcRed;
                    rotateLeft(x->parent->parent);
                }
            }
        }
        mRoot->color = rbcBlack;

        return ins;
    }

    bool erase(Node* node) {
        // "y" is a deleting node
        // "x" is child with which to replace y
        Node* z = node;
        if (z == nullptr)
            return false;

        Node *y, *x;

        if ((z->left == nullptr) || (z->right == nullptr))
            y = z;
        else
            y = z->next();

        // find child with which to replace y
        if (y->left != nullptr)
            x = y->left;
        else
            x = y->right;

        // splice child onto parent
        if (x != nullptr)
            x->parent = y->parent;

        if (y->parent == nullptr)
            // if deleting node is root, then replace root with x
            mRoot = x;
        else {
            // splice in child node
            if (y == y->parent->left)
                y->parent->left = x;
            else
                y->parent->right = x;
        }

        // if needed, save y data
        if (y != z)
            swap(z->value, y->value);

        updateNodeToRoot(y->parent);

        // adjust tree under red-black rules
        if (y != nullptr && y->color == rbcBlack)
            deleteFixup(x);

        destroyNode(y);

        return true;
    }

    bool erase(const T& item) {
        return erase(find(item));
    }

    void clear() {
        deleteRecursive(mRoot);
        mRoot = nullptr;
    }

protected:
    Node* createNode(const T& item) {
        Node* p = new Node(item);
        mCount++;
        return p;
    }

    void destroyNode(Node* node) {
        assert(node != nullptr);
        delete node;
        mCount--;
    }

    void copyRecursive(const Node* node) {
        if (node != nullptr) {
            insert(node->value);
            copyRecursive(node->left);
            copyRecursive(node->right);
        }
    }

    void deleteRecursive(Node* node) {
        if (node != nullptr) {
            deleteRecursive(node->left);
            deleteRecursive(node->right);
            destroyNode(node);
        }
    }

    pair<Node*, bool> insertBST(const T& item) {
        Node* parent = nullptr;
        Node* x = mRoot;

        while (x != nullptr) {
            parent = x;

            if (item == x->value && !mDuplicate) {
                return make_pair(x, false);
            } else {
                if (item < x->value)
                    x = x->left;
                else
                    x = x->right;
            }
        }

        Node* newNode = createNode(item);
        newNode->parent = parent;

        if (parent == nullptr)
            mRoot = newNode;
        else {
            if (newNode->value < parent->value)
                parent->left = newNode;
            else
                parent->right = newNode;
            updateNodeToRoot(parent);
        }

        return make_pair(newNode, true);
    }

    void rotateLeft(Node* node) {
        if (node == nullptr || node->right == nullptr)
            return;

        Node* y = node->right;

        // turn y's left subtree into node's right subtree
        node->right = y->left;
        if (y->left != nullptr)
            y->left->parent = node;

        // link node's parent to y
        y->parent = node->parent;
        if (node->parent == nullptr)
            mRoot = y;
        else {
            if (node == node->parent->left)
                node->parent->left = y;
            else
                node->parent->right = y;
        }

        // put node on y's left
        y->left = node;
        node->parent = y;

        updateNode(node);
        updateNode(y);
    }

    void rotateRight(Node* node) {
        if (node == nullptr || node->left == nullptr)
            return;

        Node* y = node->left;

        // turn y's right subtree into node's left subtree
        node->left = y->right;
        if (y->right != nullptr)
            y->right->parent = node;

        // link node's parent to y
        y->parent = node->parent;
        if (node->parent == nullptr)
            mRoot = y;
        else {
            if (node == node->parent->right)
                node->parent->right = y;
            else
                node->parent->left = y;
        }

        // put node on y's right
        y->right = node;
        node->parent = y;

        updateNode(node);
        updateNode(y);
    }

    void deleteFixup(Node* node) {
        if (node == nullptr)
            return;

        Node *w, *x = node;
        while ((x != mRoot) && (x->color == rbcBlack)) {
            if (x == x->parent->left) {
                w = x->parent->right;
                if (!w)
                    break;

                if (w->color == rbcRed) {
                    w->color = rbcBlack;
                    x->parent->color = rbcRed;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if (!w)
                    break;

                if ((!w->left || w->left->color == rbcBlack) && (!w->right || w->right->color == rbcBlack)) {
                    w->color = rbcRed;
                    x = x->parent;
                } else {
                    if (!w->right || w->right->color == rbcBlack) {
                        w->left->color = rbcBlack;
                        w->color = rbcRed;
                        rotateRight(w);
                        w = x->parent->right;
                    }

                    w->color = x->parent->color;
                    x->parent->color = rbcBlack;
                    w->right->color = rbcBlack;
                    rotateLeft(x->parent);
                    x = mRoot;
                }
            } else {
                w = x->parent->left;
                if (!w)
                    break;

                if (w->color == rbcRed) {
                    w->color = rbcBlack;
                    x->parent->color = rbcRed;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }
                if (!w)
                    break;

                if ((!w->right || w->right->color == rbcBlack) && (!w->left || w->left->color == rbcBlack)) {
                    w->color = rbcRed;
                    x = x->parent;
                } else {
                    if (!w->left || w->left->color == rbcBlack) {
                        w->right->color = rbcBlack;
                        w->color = rbcRed;
                        rotateLeft(w);
                        w = x->parent->left;
                    }

                    w->color = x->parent->color;
                    x->parent->color = rbcBlack;
                    w->left->color = rbcBlack;
                    rotateRight(x->parent);
                    x = mRoot;
                }
            }
        }

        x->color = rbcBlack;
    }


    void updateNodeToRoot(Node* node) {
        // update count
        while (node != nullptr) {
            updateNode(node);
            node = node->parent;
        }
    }

    void updateNodeCnt(Node* node) {
        if (node != nullptr)
            node->cnt = (node->left ? node->left->cnt : 0) + (node->right ? node->right->cnt : 0) + 1;
    }

    void updateNode(Node* node) {
        updateNodeCnt(node);

        //TODO: add custom actions
    }
};
