#pragma once

#include <cassert>
#include <functional>
#include <algorithm>

// T has to have below functions
//    1) operator ==()
//    2) operator <()

template <typename T>
struct BST {
    struct Node {
        Node*   parent;
        Node*   left;
        Node*   right;

        int     cnt;
        T       value;

        void init() {
            parent = this;
            left = this;
            right = this;
            cnt = 0;
        }

        void init(const T& item, Node* sentinel) {
            parent = sentinel;
            left = sentinel;
            right = sentinel;
            cnt = 1;
            value = item;
        }
    };

    Node*       mRoot;     // root node
    bool        mDuplicate;
    int         mCount;

    Node*       mSentinel;

//-----------------------------------------------------------

    BST() {
        mSentinel = new Node();
        mSentinel->init();

        mRoot = mSentinel;
        mDuplicate = false;
        mCount = 0;
    }

    BST(const BST<T>& tree) {
        mSentinel = new Node();
        mSentinel->init();

        mRoot = mSentinel;
        mDuplicate = false;
        mCount = 0;
        copyRecursive(tree.mRoot);
    }

    virtual ~BST() {
        deleteRecursive(mRoot);
        delete mSentinel;
    }

    BST<T>& operator =(const BST<T>& tree) {
        if (this != &tree) {
            deleteRecursive(mRoot);
            mDuplicate = tree.mDuplicate;
            mRoot = mSentinel;
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

    bool exist(const T& key) const {
        return find(key) != nullptr;
    }

    bool canDuplicate() const {
        return mDuplicate;
    }

    void enableDuplicate(bool enable) {
        mDuplicate = enable;
    }


    Node* nullNode() const {
        return mSentinel;
    }


    pair<Node*,bool> insert(const T& item) {
        Node* parent = mSentinel;
        Node* x = mRoot;

        while (x != mSentinel) {
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

        if (parent == mSentinel)
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

    bool erase(Node* node) {
        Node* z = node;
        if (z == mSentinel)
            return false;

        // "y" is a deleting node
        // "x" is child with which to replace y
        Node *y, *x;

        // find node to splice out
        if ((z->left == mSentinel) || (z->right == mSentinel))
            y = z;
        else
            y = next(z);

        // find child with which to replace y
        if (y->left != mSentinel)
            x = y->left;
        else
            x = y->right;

        // if child node of y exist, then splice child onto parent
        if (x != mSentinel)
            x->parent = y->parent;

        if (y->parent == mSentinel)
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

        // free memory
        destroyNode(y);

        return true;
    }

    bool erase(const T& item) {
        return erase(find(item));
    }

    void clear() {
        deleteRecursive(mRoot);
        mRoot = mSentinel;
    }


    Node* operator [](int index) const {
        assert((mRoot != mSentinel ? mRoot->cnt : 0) == mCount);

        if (index < 0 || index >= mCount)
            return mSentinel;
        
        int n = index;
        Node* p = mRoot;
        while (p != mSentinel) {
            while (p->left != mSentinel && p->left->cnt > n)
                p = p->left;
            if (p->left != mSentinel)
                n -= p->left->cnt;
            if (!n--)
                break;
            p = p->right;
        }

        return p;
    }

    int indexOf(Node* p) const {
        assert((mRoot != mSentinel ? mRoot->cnt : 0) == mCount);

        if (p == mSentinel)
            return -1;

        int res = (p->left != mSentinel) ? p->left->cnt : 0;
        while (p->parent != mSentinel) {
            while (p->parent != mSentinel && p->parent->left == p)
                p = p->parent;
            if (p->parent != mSentinel) {
                p = p->parent;
                res++;
                if (p->left != mSentinel)
                    res += p->left->cnt;
            }
        }

        return res;
    }


    Node* find(const T& key) const {
        Node *p = mRoot;

        while (p != mSentinel && !(p->value == key)) {
            if (key < p->value)
                p = p->left;
            else
                p = p->right;
        }

        return p;
    }

    Node* lowerBound(const T& key) const {
        if (mRoot == mSentinel)
            return mSentinel;

        Node* y = mSentinel;
        Node* x = mRoot;
        while (x != mSentinel) {
            if (!(x->value < key)) {
                y = x;
                x = x->left;
            } else
                x = x->right;
        }
        return y;
    }

    Node* upperBound(const T& key) const {
        if (mRoot == mSentinel)
            return mSentinel;

        Node* y = mSentinel;
        Node* x = mRoot;
        while (x != mSentinel) {
            if (key < x->value) {
                y = x;
                x = x->left;
            } else
                x = x->right;
        }
        return y;
    }


    Node* leftmost() const {
        return minimum(mRoot);
    }

    // inclusive
    Node* rightmost() const {
        return maximum(mRoot);
    }

    Node* minimum(const Node* node) const {
        Node* p = const_cast<Node*>(node);
        while (p->left != mSentinel)
            p = p->left;
        return p;
    }

    Node* maximum(const Node* node) const {
        Node* p = const_cast<Node*>(node);
        while (p->right != mSentinel)
            p = p->right;
        return p;
    }

    Node* next(const Node* node) const {
        if (node->right != mSentinel)
            return minimum(node->right);
        else {
            Node *cur, *parent;

            cur = const_cast<Node*>(node);
            parent = node->parent;
            while ((parent != mSentinel) && (cur == parent->right)) {
                cur = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }

    Node* prev(const Node* node) const {
        if (node->left != mSentinel)
            return maximum(node->left);
        else {
            Node *cur, *parent;

            cur = const_cast<Node*>(node);
            parent = node->parent;
            while ((parent != mSentinel) && (cur == parent->left)) {
                cur = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }


    void walkRecursive(const function<void(const T&)>& func) const {
        recurseWalk(mRoot, func);
    }

    void walkIterative(const function<void(const T&)>& func) const {
        morrisInorderWalk(mRoot, func);
    }

protected:
    Node* createNode(const T& item) {
        Node* p = new Node();
        p->init(item, mSentinel);
        mCount++;
        return p;
    }

    void destroyNode(Node* node) {
        assert(node != mSentinel);
        delete node;
        mCount--;
    }

    void copyRecursive(const Node* node) {
        if (node != mSentinel) {
            insert(node->value);
            copyRecursive(node->left);
            copyRecursive(node->right);
        }
    }

    void deleteRecursive(Node* node) {
        if (node != mSentinel) {
            deleteRecursive(node->left);
            deleteRecursive(node->right);
            destroyNode(node);
        }
    }

    void recurseWalk(const Node* node, const function<void(const T&)>& func) const {
        if (node != mSentinel) {
            recurseWalk(node->left, func);
            func(node->value);
            recurseWalk(node->right, func);
        }
    }

    void morrisInorderWalk(const Node* node, const function<void(const T&)>& func) const {
        Node *p = mRoot, *tmp;
        while (p != mSentinel) {
            if (p->left == mSentinel) {
                func(p->value);
                p = p->right;
            } else {
                tmp = p->left;
                while (tmp->right != mSentinel && tmp->right != p)
                    tmp = tmp->right;
                if (tmp->right == mSentinel) {
                    tmp->right = p;
                    p = p->left;
                } else {
                    func(p->value);
                    tmp->right = mSentinel;
                    p = p->right;
                }
            }
        }
    }

    void updateNodeToRoot(Node* node) {
        if (node == mSentinel)
            return;

        // update count
        while (node != mSentinel) {
            updateNode(node);
            node = node->parent;
        }
    }

    void updateNodeCnt(Node* node) {
        if (node != mSentinel)
            node->cnt = (node->left ? node->left->cnt : 0) + (node->right ? node->right->cnt : 0) + 1;
    }

    void updateNode(Node* node) {
        updateNodeCnt(node);

        //TODO: add custom actions
    }
};


//--------- Red-Black Tree ----------------------------------------------------

enum RBColor {
    rbcBlack,
    rbcRed
};

template <typename T>
struct RBData {
    RBData() {
        color = rbcBlack;
    }

    RBData(const T& data) {
        color = rbcBlack;
        value = data;
    }

    bool operator ==(const RBData<T>& rhs) const {
        return value == rhs.value;
    }

    bool operator <(const RBData<T>& rhs) const {
        return value < rhs.value;
    }

    RBColor color;
    T       value;
};

template <typename T>
struct RBTree : public BST<RBData<T>> {
    RBTree() {
    }

    RBTree(const RBTree<T>& tree) : BST<RBData<T>>(tree) {
    }

    RBTree<T>& operator =(const RBTree<T>& tree) {
        BST::operator =(tree);
        return *this;
    }

    pair<Node*,bool> insert(const T& item) {
        RBData<T> rbitem(item);
        pair<Node*, bool> ins = BST::insert(rbitem);
        if (!ins.second)
            return ins;

        Node* x = ins.first;
        Node* y;

        x->value.color = rbcRed;
        while ((x != mRoot) && (x->parent->value.color == rbcRed)) {
            if (x->parent == x->parent->parent->left) {
                y = x->parent->parent->right;

                if (y->value.color == rbcRed) {
                    x->parent->value.color = rbcBlack;
                    y->value.color = rbcBlack;
                    x->parent->parent->value.color = rbcRed;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->right) {
                        x = x->parent;
                        rotateLeft(x);
                    }

                    x->parent->value.color = rbcBlack;
                    x->parent->parent->value.color = rbcRed;
                    rotateRight(x->parent->parent);
                }
            } else {
                y = x->parent->parent->left;

                if (y->value.color == rbcRed) {
                    x->parent->value.color = rbcBlack;
                    y->value.color = rbcBlack;
                    x->parent->parent->value.color = rbcRed;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->left) {
                        x = x->parent;
                        rotateRight(x);
                    }

                    x->parent->value.color = rbcBlack;
                    x->parent->parent->value.color = rbcRed;
                    rotateLeft(x->parent->parent);
                }
            }
        }
        mRoot->value.color = rbcBlack;

        return ins;
    }

    bool erase(Node* node) {
        // "y" is a deleting node
        // "x" is child with which to replace y
        Node* z = node;
        if (z == mSentinel)
            return false;

        Node *y, *x;

        if ((z->left == mSentinel) || (z->right == mSentinel))
            y = z;
        else
            y = next(z);

        // find child with which to replace y
        if (y->left != mSentinel)
            x = y->left;
        else
            x = y->right;

        // splice child onto parent
        if (x != mSentinel)
            x->parent = y->parent;

        if (y->parent == mSentinel)
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
        if (y != mSentinel && y->value.color == rbcBlack)
            deleteFixup(x);

        destroyNode(y);

        return true;
    }

    bool erase(const T& item) {
        return erase(find(item));
    }

protected:
    void rotateLeft(Node* node) {
        if (node == mSentinel || node->right == mSentinel)
            return;

        Node* y = node->right;

        // turn y's left subtree into node's right subtree
        node->right = y->left;
        if (y->left != mSentinel)
            y->left->parent = node;

        // link node's parent to y
        y->parent = node->parent;
        if (node->parent == mSentinel)
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
        if (node == mSentinel || node->left == mSentinel)
            return;

        Node* y = node->left;

        // turn y's right subtree into node's left subtree
        node->left = y->right;
        if (y->right != mSentinel)
            y->right->parent = node;

        // link node's parent to y
        y->parent = node->parent;
        if (node->parent == mSentinel)
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
        if (node == mSentinel)
            return;

        Node *w, *x = node;
        while ((x != mRoot) && (x->value.color == rbcBlack)) {
            if (x == x->parent->left) {
                w = x->parent->right;

                if (w->value.color == rbcRed) {
                    w->value.color = rbcBlack;
                    x->parent->value.color = rbcRed;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }

                if ((w->left->value.color == rbcBlack) && (w->right->value.color == rbcBlack)) {
                    w->value.color = rbcRed;
                    x = x->parent;
                } else {
                    if (w->right->value.color == rbcBlack) {
                        w->left->value.color = rbcBlack;
                        w->value.color = rbcRed;
                        rotateRight(w);
                        w = x->parent->right;
                    }

                    w->value.color = x->parent->value.color;
                    x->parent->value.color = rbcBlack;
                    w->right->value.color = rbcBlack;
                    rotateLeft(x->parent);
                    x = mRoot;
                }
            } else {
                w = x->parent->left;

                if (w->value.color == rbcRed) {
                    w->value.color = rbcBlack;
                    x->parent->value.color = rbcRed;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }

                if ((w->right->value.color == rbcBlack) && (w->left->value.color == rbcBlack)) {
                    w->value.color = rbcRed;
                    x = x->parent;
                } else {
                    if (w->left->value.color == rbcBlack) {
                        w->right->value.color = rbcBlack;
                        w->value.color = rbcRed;
                        rotateLeft(w);
                        w = x->parent->left;
                    }

                    w->value.color = x->parent->value.color;
                    x->parent->value.color = rbcBlack;
                    w->left->value.color = rbcBlack;
                    rotateRight(x->parent);
                    x = mRoot;
                }
            }
        }

        x->value.color = rbcBlack;
    }
};
