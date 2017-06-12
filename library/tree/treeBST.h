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
        Node() {
            mParent = this;
            mLeft = this;
            mRight = this;
            mCnt = 0;
        }

        Node(const T& item, Node* sentinel) : mData(item) {
            mParent = sentinel;
            mLeft = sentinel;
            mRight = sentinel;
            mCnt = 1;
        }

        Node*   mLeft;
        Node*   mRight;
        Node*   mParent;
        int     mCnt;
        T       mData;
    };

    Node*       mRoot;     // root node
    bool        mDuplicate;
    int         mCount;

    Node*       mSentinel;

//-----------------------------------------------------------

    BST() {
        mSentinel = new Node();

        mRoot = mSentinel;
        mDuplicate = false;
        mCount = 0;
    }

    BST(const BST<T>& tree) {
        mSentinel = new Node();

        mRoot = mSentinel;
        mDuplicate = false;
        mCount = 0;
        copyRecursive(tree.mRoot);
    }

    virtual ~BST() {
        deleteRecursive(mRoot);
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

            if (item == x->mData && !mDuplicate) {
                return make_pair(x, false);
            } else {
                if (item < x->mData)
                    x = x->mLeft;
                else
                    x = x->mRight;
            }
        }

        Node* newNode = createNode(item);
        newNode->mParent = parent;

        if (parent == mSentinel)
            mRoot = newNode;
        else {
            if (newNode->mData < parent->mData)
                parent->mLeft = newNode;
            else
                parent->mRight = newNode;
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
        if ((z->mLeft == mSentinel) || (z->mRight == mSentinel))
            y = z;
        else
            y = next(z);

        // find child with which to replace y
        if (y->mLeft != mSentinel)
            x = y->mLeft;
        else
            x = y->mRight;

        // if child node of y exist, then splice child onto parent
        if (x != mSentinel)
            x->mParent = y->mParent;

        if (y->mParent == mSentinel)
            // if deleting node is root, then replace root with x
            mRoot = x;
        else {
            // splice in child node
            if (y == y->mParent->mLeft)
                y->mParent->mLeft = x;
            else
                y->mParent->mRight = x;
        }

        // if needed, save y data
        if (y != z)
            swap(z->mData, y->mData);

        updateNodeToRoot(y->mParent);

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
        assert((mRoot != mSentinel ? mRoot->mCnt : 0) == mCount);

        if (index < 0 || index >= mCount)
            return mSentinel;
        
        int n = index;
        Node* p = mRoot;
        while (p != mSentinel) {
            while (p->mLeft != mSentinel && p->mLeft->mCnt > n)
                p = p->mLeft;
            if (p->mLeft != mSentinel)
                n -= p->mLeft->mCnt;
            if (!n--)
                break;
            p = p->mRight;
        }

        return p;
    }

    int indexOf(Node* p) const {
        assert((mRoot != mSentinel ? mRoot->mCnt : 0) == mCount);

        if (p == mSentinel)
            return -1;

        int res = (p->mLeft != mSentinel) ? p->mLeft->mCnt : 0;
        while (p->mParent != mSentinel) {
            while (p->mParent != mSentinel && p->mParent->mLeft == p)
                p = p->mParent;
            if (p->mParent != mSentinel) {
                p = p->mParent;
                res++;
                if (p->mLeft != mSentinel)
                    res += p->mLeft->mCnt;
            }
        }

        return res;
    }


    Node* find(const T& key) const {
        Node *p = mRoot;

        while (p != mSentinel && !(p->mData == key)) {
            if (key < p->mData)
                p = p->mLeft;
            else
                p = p->mRight;
        }

        return p;
    }

    Node* lowerBound(const T& key) const {
        if (mRoot == mSentinel)
            return mSentinel;

        Node* y = mSentinel;
        Node* x = mRoot;
        while (x != mSentinel) {
            if (!(x->mData < key)) {
                y = x;
                x = x->mLeft;
            } else
                x = x->mRight;
        }
        return y;
    }

    Node* upperBound(const T& key) const {
        if (mRoot == mSentinel)
            return mSentinel;

        Node* y = mSentinel;
        Node* x = mRoot;
        while (x != mSentinel) {
            if (key < x->mData) {
                y = x;
                x = x->mLeft;
            } else
                x = x->mRight;
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
        while (p->mLeft != mSentinel)
            p = p->mLeft;
        return p;
    }

    Node* maximum(const Node* node) const {
        Node* p = const_cast<Node*>(node);
        while (p->mRight != mSentinel)
            p = p->mRight;
        return p;
    }

    Node* next(const Node* node) const {
        if (node->mRight != mSentinel)
            return minimum(node->mRight);
        else {
            Node *cur, *parent;

            cur = const_cast<Node*>(node);
            parent = node->mParent;
            while ((parent != mSentinel) && (cur == parent->mRight)) {
                cur = parent;
                parent = parent->mParent;
            }
            return parent;
        }
    }

    Node* prev(const Node* node) const {
        if (node->mLeft != mSentinel)
            return maximum(node->mLeft);
        else {
            Node *cur, *parent;

            cur = const_cast<Node*>(node);
            parent = node->mParent;
            while ((parent != mSentinel) && (cur == parent->mLeft)) {
                cur = parent;
                parent = parent->mParent;
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
        Node* p = new Node(item, mSentinel);
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
            insert(node->mData);
            copyRecursive(node->mLeft);
            copyRecursive(node->mRight);
        }
    }

    void deleteRecursive(Node* node) {
        if (node != mSentinel) {
            deleteRecursive(node->mLeft);
            deleteRecursive(node->mRight);
            destroyNode(node);
        }
    }

    void recurseWalk(const Node* node, const function<void(const T&)>& func) const {
        if (node != mSentinel) {
            recurseWalk(node->mLeft, func);
            func(node->mData);
            recurseWalk(node->mRight, func);
        }
    }

    void morrisInorderWalk(const Node* node, const function<void(const T&)>& func) const {
        Node *p = mRoot, *tmp;
        while (p != mSentinel) {
            if (p->mLeft == mSentinel) {
                func(p->mData);
                p = p->mRight;
            } else {
                tmp = p->mLeft;
                while (tmp->mRight != mSentinel && tmp->mRight != p)
                    tmp = tmp->mRight;
                if (tmp->mRight == mSentinel) {
                    tmp->mRight = p;
                    p = p->mLeft;
                } else {
                    func(p->mData);
                    tmp->mRight = mSentinel;
                    p = p->mRight;
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
            node = node->mParent;
        }
    }

    void updateNode(Node* node) {
        if (node != mSentinel)
            node->mCnt = (node->mLeft ? node->mLeft->mCnt : 0) + (node->mRight ? node->mRight->mCnt : 0) + 1;
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
        mColor = rbcBlack;
    }

    RBData(const T& data) {
        mColor = rbcBlack;
        mData = data;
    }

    bool operator ==(const RBData<T>& rhs) const {
        return mData == rhs.mData;
    }

    bool operator <(const RBData<T>& rhs) const {
        return mData < rhs.mData;
    }

    RBColor mColor;
    T       mData;
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

        x->mData.mColor = rbcRed;
        while ((x != mRoot) && (x->mParent->mData.mColor == rbcRed)) {
            if (x->mParent == x->mParent->mParent->mLeft) {
                y = x->mParent->mParent->mRight;

                if (y->mData.mColor == rbcRed) {
                    x->mParent->mData.mColor = rbcBlack;
                    y->mData.mColor = rbcBlack;
                    x->mParent->mParent->mData.mColor = rbcRed;
                    x = x->mParent->mParent;
                } else {
                    if (x == x->mParent->mRight) {
                        x = x->mParent;
                        rotateLeft(x);
                    }

                    x->mParent->mData.mColor = rbcBlack;
                    x->mParent->mParent->mData.mColor = rbcRed;
                    rotateRight(x->mParent->mParent);
                }
            } else {
                y = x->mParent->mParent->mLeft;

                if (y->mData.mColor == rbcRed) {
                    x->mParent->mData.mColor = rbcBlack;
                    y->mData.mColor = rbcBlack;
                    x->mParent->mParent->mData.mColor = rbcRed;
                    x = x->mParent->mParent;
                } else {
                    if (x == x->mParent->mLeft) {
                        x = x->mParent;
                        rotateRight(x);
                    }

                    x->mParent->mData.mColor = rbcBlack;
                    x->mParent->mParent->mData.mColor = rbcRed;
                    rotateLeft(x->mParent->mParent);
                }
            }
        }
        mRoot->mData.mColor = rbcBlack;

        return ins;
    }

    bool erase(Node* node) {
        // "y" is a deleting node
        // "x" is child with which to replace y
        Node* z = node;
        if (z == mSentinel)
            return false;

        Node *y, *x;

        if ((z->mLeft == mSentinel) || (z->mRight == mSentinel))
            y = z;
        else
            y = next(z);

        // find child with which to replace y
        if (y->mLeft != mSentinel)
            x = y->mLeft;
        else
            x = y->mRight;

        // splice child onto mParent
        if (x != mSentinel)
            x->mParent = y->mParent;

        if (y->mParent == mSentinel)
            // if deleting node is root, then replace root with x
            mRoot = x;
        else {
            // splice in child node
            if (y == y->mParent->mLeft)
                y->mParent->mLeft = x;
            else
                y->mParent->mRight = x;
        }

        // if needed, save y data
        if (y != z)
            swap(z->mData, y->mData);

        updateNodeToRoot(y->mParent);

        // adjust tree under red-black rules
        if (y != mSentinel && y->mData.mColor == rbcBlack)
            deleteFixup(x);

        destroyNode(y);

        return true;
    }

    bool erase(const T& item) {
        return erase(find(item));
    }

protected:
    void rotateLeft(Node* node) {
        if (node == mSentinel || node->mRight == mSentinel)
            return;

        Node* y = node->mRight;

        // turn y's left subtree into node's right subtree
        node->mRight = y->mLeft;
        if (y->mLeft != mSentinel)
            y->mLeft->mParent = node;

        // link node's mParent to y
        y->mParent = node->mParent;
        if (node->mParent == mSentinel)
            mRoot = y;
        else {
            if (node == node->mParent->mLeft)
                node->mParent->mLeft = y;
            else
                node->mParent->mRight = y;
        }

        // put node on y's left
        y->mLeft = node;
        node->mParent = y;

        updateNode(node);
        updateNode(y);
    }

    void rotateRight(Node* node) {
        if (node == mSentinel || node->mLeft == mSentinel)
            return;

        Node* y = node->mLeft;

        // turn y's right subtree into node's left subtree
        node->mLeft = y->mRight;
        if (y->mRight != mSentinel)
            y->mRight->mParent = node;

        // link node's mParent to y
        y->mParent = node->mParent;
        if (node->mParent == mSentinel)
            mRoot = y;
        else {
            if (node == node->mParent->mRight)
                node->mParent->mRight = y;
            else
                node->mParent->mLeft = y;
        }

        // put node on y's right
        y->mRight = node;
        node->mParent = y;

        updateNode(node);
        updateNode(y);
    }

    void deleteFixup(Node* node) {
        //assert(node != mSentinel);
        if (node == mSentinel)
            return;

        Node *w, *x = node;
        while ((x != mRoot) && (x->mData.mColor == rbcBlack)) {
            if (x == x->mParent->mLeft) {
                w = x->mParent->mRight;

                if (w->mData.mColor == rbcRed) {
                    w->mData.mColor = rbcBlack;
                    x->mParent->mData.mColor = rbcRed;
                    rotateLeft(x->mParent);
                    w = x->mParent->mRight;
                }

                if ((w->mLeft->mData.mColor == rbcBlack) && (w->mRight->mData.mColor == rbcBlack)) {
                    w->mData.mColor = rbcRed;
                    x = x->mParent;
                } else {
                    if (w->mRight->mData.mColor == rbcBlack) {
                        w->mLeft->mData.mColor = rbcBlack;
                        w->mData.mColor = rbcRed;
                        rotateRight(w);
                        w = x->mParent->mRight;
                    }

                    w->mData.mColor = x->mParent->mData.mColor;
                    x->mParent->mData.mColor = rbcBlack;
                    w->mRight->mData.mColor = rbcBlack;
                    rotateLeft(x->mParent);
                    x = mRoot;
                }
            } else {
                w = x->mParent->mLeft;

                if (w->mData.mColor == rbcRed) {
                    w->mData.mColor = rbcBlack;
                    x->mParent->mData.mColor = rbcRed;
                    rotateRight(x->mParent);
                    w = x->mParent->mLeft;
                }

                if ((w->mRight->mData.mColor == rbcBlack) && (w->mLeft->mData.mColor == rbcBlack)) {
                    w->mData.mColor = rbcRed;
                    x = x->mParent;
                } else {
                    if (w->mLeft->mData.mColor == rbcBlack) {
                        w->mRight->mData.mColor = rbcBlack;
                        w->mData.mColor = rbcRed;
                        rotateLeft(w);
                        w = x->mParent->mLeft;
                    }

                    w->mData.mColor = x->mParent->mData.mColor;
                    x->mParent->mData.mColor = rbcBlack;
                    w->mLeft->mData.mColor = rbcBlack;
                    rotateRight(x->mParent);
                    x = mRoot;
                }
            }
        }

        x->mData.mColor = rbcBlack;
    }
};
