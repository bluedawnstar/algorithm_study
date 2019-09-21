#pragma once

#include <cassert>
#include <functional>
#include <algorithm>

// T has to have below functions
//    1) operator ==()
//    2) operator <()

template <typename T>
struct BSTNode {
    BSTNode<T>* parent;
    BSTNode<T>* left;
    BSTNode<T>* right;

    int     cnt;
    T       value;

    void init() {
        parent = this;
        left = this;
        right = this;
        cnt = 0;
    }

    void init(const T& item, BSTNode<T>* sentinel) {
        parent = sentinel;
        left = sentinel;
        right = sentinel;
        cnt = 1;
        value = item;
    }
};

template <typename T, typename Node = BSTNode<T>>
struct BST {
    Node*       root;     // root node
    bool        duplicatable;
    int         count;

    Node*       sentinel;

    //-----------------------------------------------------------

    BST() {
        sentinel = new Node();
        sentinel->init();

        root = sentinel;
        duplicatable = false;
        count = 0;
    }

    BST(const BST<T>& tree) {
        sentinel = new Node();
        sentinel->init();

        root = sentinel;
        duplicatable = false;
        count = 0;
        copyRecursive(tree.root);
    }

    virtual ~BST() {
        deleteRecursive(root);
        delete sentinel;
    }

    BST<T>& operator =(const BST<T>& tree) {
        if (this != &tree) {
            deleteRecursive(root);
            duplicatable = tree.duplicatable;
            root = sentinel;
            copyRecursive(tree.root);
        }
        return *this;
    }

    int size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    bool exist(const T& key) const {
        return find(key) != nullptr;
    }

    bool isDuplicatable() const {
        return duplicatable;
    }

    void setDuplicatable(bool enable) {
        duplicatable = enable;
    }


    Node* nullNode() const {
        return sentinel;
    }


    pair<Node*, bool> insert(const T& item) {
        Node* parent = sentinel;
        Node* x = root;

        while (x != sentinel) {
            parent = x;

            if (item == x->value && !duplicatable) {
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

        if (parent == sentinel)
            root = newNode;
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
        if (z == sentinel)
            return false;

        // "y" is a deleting node
        // "x" is child with which to replace y
        Node *y, *x;

        // find node to splice out
        if ((z->left == sentinel) || (z->right == sentinel))
            y = z;
        else
            y = next(z);

        // find child with which to replace y
        if (y->left != sentinel)
            x = y->left;
        else
            x = y->right;

        // if child node of y exist, then splice child onto parent
        if (x != sentinel)
            x->parent = y->parent;

        if (y->parent == sentinel)
            // if deleting node is root, then replace root with x
            root = x;
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
        deleteRecursive(root);
        root = sentinel;
    }


    Node* operator [](int index) const {
        assert((root != sentinel ? root->cnt : 0) == count);

        if (index < 0 || index >= count)
            return sentinel;

        int n = index;
        Node* p = root;
        while (p != sentinel) {
            while (p->left != sentinel && p->left->cnt > n)
                p = p->left;
            if (p->left != sentinel)
                n -= p->left->cnt;
            if (!n--)
                break;
            p = p->right;
        }

        return p;
    }

    int indexOf(Node* p) const {
        assert((root != sentinel ? root->cnt : 0) == count);

        if (p == sentinel)
            return -1;

        int res = (p->left != sentinel) ? p->left->cnt : 0;
        while (p->parent != sentinel) {
            while (p->parent != sentinel && p->parent->left == p)
                p = p->parent;
            if (p->parent != sentinel) {
                p = p->parent;
                res++;
                if (p->left != sentinel)
                    res += p->left->cnt;
            }
        }

        return res;
    }


    Node* find(const T& key) const {
        Node *p = root;

        while (p != sentinel && !(p->value == key)) {
            if (key < p->value)
                p = p->left;
            else
                p = p->right;
        }

        return p;
    }

    Node* lowerBound(const T& key) const {
        if (root == sentinel)
            return sentinel;

        Node* y = sentinel;
        Node* x = root;
        while (x != sentinel) {
            if (!(x->value < key)) {
                y = x;
                x = x->left;
            } else
                x = x->right;
        }
        return y;
    }

    Node* upperBound(const T& key) const {
        if (root == sentinel)
            return sentinel;

        Node* y = sentinel;
        Node* x = root;
        while (x != sentinel) {
            if (key < x->value) {
                y = x;
                x = x->left;
            } else
                x = x->right;
        }
        return y;
    }


    Node* leftmost() const {
        return minimum(root);
    }

    // inclusive
    Node* rightmost() const {
        return maximum(root);
    }

    Node* minimum(const Node* node) const {
        Node* p = const_cast<Node*>(node);
        while (p->left != sentinel)
            p = p->left;
        return p;
    }

    Node* maximum(const Node* node) const {
        Node* p = const_cast<Node*>(node);
        while (p->right != sentinel)
            p = p->right;
        return p;
    }

    Node* next(const Node* node) const {
        if (node->right != sentinel)
            return minimum(node->right);
        else {
            Node *cur, *parent;

            cur = const_cast<Node*>(node);
            parent = node->parent;
            while ((parent != sentinel) && (cur == parent->right)) {
                cur = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }

    Node* prev(const Node* node) const {
        if (node->left != sentinel)
            return maximum(node->left);
        else {
            Node *cur, *parent;

            cur = const_cast<Node*>(node);
            parent = node->parent;
            while ((parent != sentinel) && (cur == parent->left)) {
                cur = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }


    void walkRecursive(const function<void(const T&)>& func) const {
        recurseWalk(root, func);
    }

    void walkIterative(const function<void(const T&)>& func) const {
        morrisInorderWalk(root, func);
    }

protected:
    Node* createNode(const T& item) {
        Node* p = new Node();
        p->init(item, sentinel);
        count++;
        return p;
    }

    void destroyNode(Node* node) {
        assert(node != sentinel);
        delete node;
        count--;
    }

    void copyRecursive(const Node* node) {
        if (node != sentinel) {
            insert(node->value);
            copyRecursive(node->left);
            copyRecursive(node->right);
        }
    }

    void deleteRecursive(Node* node) {
        if (node == nullptr || node == sentinel)
            return;

        vector<Node*> nodes;
        nodes.push_back(node);

        while (!nodes.empty()) {
            Node* p = nodes.back();
            nodes.pop_back();

            if (p->left != sentinel)
                nodes.push_back(p->left);
            if (p->right != sentinel)
                nodes.push_back(p->right);

            destroyNode(p);
        }
    }

    void recurseWalk(const Node* node, const function<void(const T&)>& func) const {
        if (node != sentinel) {
            recurseWalk(node->left, func);
            func(node->value);
            recurseWalk(node->right, func);
        }
    }

    void morrisInorderWalk(const Node* node, const function<void(const T&)>& func) const {
        Node *p = root, *tmp;
        while (p != sentinel) {
            if (p->left == sentinel) {
                func(p->value);
                p = p->right;
            } else {
                tmp = p->left;
                while (tmp->right != sentinel && tmp->right != p)
                    tmp = tmp->right;
                if (tmp->right == sentinel) {
                    tmp->right = p;
                    p = p->left;
                } else {
                    func(p->value);
                    tmp->right = sentinel;
                    p = p->right;
                }
            }
        }
    }

    void updateNodeToRoot(Node* node) {
        if (node == sentinel)
            return;

        // update count
        while (node != sentinel) {
            onUpdateNode(node);
            node = node->parent;
        }
    }

    virtual void onUpdateNode(Node* node) {
        if (node != sentinel)
            node->cnt = (node->left ? node->left->cnt : 0) + (node->right ? node->right->cnt : 0) + 1;
    }
};
