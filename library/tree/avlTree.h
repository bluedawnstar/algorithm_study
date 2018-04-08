#pragma once

#include "binarySearchTree.h"

//--------- AVL Tree ----------------------------------------------------------

template <typename T>
struct AVLData {
    AVLData() {
        height = 1;
    }

    AVLData(const T& data) {
        height = 1;
        value = data;
    }

    bool operator ==(const AVLData<T>& rhs) const {
        return value == rhs.value;
    }

    bool operator <(const AVLData<T>& rhs) const {
        return value < rhs.value;
    }
    
    int     height;
    T       value;
};

template <typename T>
struct AVLTree : public BST<AVLData<T>> {
    AVLTree() {
        sentinel->value.height = 0;
    }

    AVLTree(const AVLTree<T>& tree) : BST<AVLData<T>>(tree) {
        sentinel->value.height = 0;
    }

    AVLTree<T>& operator =(const AVLTree<T>& tree) {
        BST::operator =(tree);
        return *this;
    }

    pair<Node*, bool> insert(const T& val) {
        AVLData<T> item(val);
        pair<Node*, bool> ins = BST::insert(item);
        if (!ins.second)
            return ins;

        Node* x = ins.first;
        while (x != sentinel) {
            Node* parent = x->parent;

            int balance = getBalance(x);
            if (balance > 1) {
                if (!(val < x->left->value.value))
                    rotateLeft(x->left);
                rotateRight(x);
            } else if (balance < -1) {
                if (val < x->right->value.value)
                    rotateRight(x->right);
                rotateLeft(x);
            } else {
                updateHeight(x);
            }

            x = parent;
        }

        return ins;
    }

    bool erase(Node* node) {
        // "y" is a deleting node
        // "x" is child with which to replace y
        Node* z = node;
        if (z == sentinel)
            return false;

        Node *y, *x;

        if ((z->left == sentinel) || (z->right == sentinel))
            y = z;
        else
            y = next(z);

        // find child with which to replace y
        if (y->left != sentinel)
            x = y->left;
        else
            x = y->right;

        // splice child onto parent
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
            swap(z->value.value, y->value.value);

        updateNodeToRoot(y->parent);

        // adjust tree under AVL rules
        if (y != sentinel)
            deleteFixup(x);

        destroyNode(y);

        return true;
    }

    bool erase(const T& val) {
        return erase(find(val));
    }


protected:
    int getHeight(const Node* node) {
        return (node == nullptr || node == sentinel) ? 0 : node->value.height;
    }

    int getBalance(const Node* node) {
        return (node == nullptr || node == sentinel) ? 0 : getHeight(node->left) - getHeight(node->right);
    }

    void updateHeight(Node* node) {
        node->value.height = max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    virtual void onUpdateNode(Node* node) override {
        BST<AVLData<T>>::onUpdateNode(node);
        updateHeight(node);
    }


    void rotateLeft(Node* node) {
        if (node == sentinel || node->right == sentinel)
            return;

        Node* y = node->right;

        // turn y's left subtree into node's right subtree
        node->right = y->left;
        if (y->left != sentinel)
            y->left->parent = node;

        // link node's parent to y
        y->parent = node->parent;
        if (node->parent == sentinel)
            root = y;
        else {
            if (node == node->parent->left)
                node->parent->left = y;
            else
                node->parent->right = y;
        }

        // put node on y's left
        y->left = node;
        node->parent = y;

        onUpdateNode(node);
        onUpdateNode(y);
    }

    void rotateRight(Node* node) {
        if (node == sentinel || node->left == sentinel)
            return;

        Node* y = node->left;

        // turn y's right subtree into node's left subtree
        node->left = y->right;
        if (y->right != sentinel)
            y->right->parent = node;

        // link node's parent to y
        y->parent = node->parent;
        if (node->parent == sentinel)
            root = y;
        else {
            if (node == node->parent->right)
                node->parent->right = y;
            else
                node->parent->left = y;
        }

        // put node on y's right
        y->right = node;
        node->parent = y;

        onUpdateNode(node);
        onUpdateNode(y);
    }

    void deleteFixup(Node* node) {
        if (node == sentinel)
            return;

        Node* x = node;
        while (x != sentinel) {
            Node* parent = x->parent;

            int balance = getBalance(x);
            if (balance > 1) {
                if (getBalance(x->left) < 0)
                    rotateLeft(x->left);
                rotateRight(x);
            } else if (balance < -1) {
                if (getBalance(x->right) > 0)
                    rotateRight(x->right);
                rotateLeft(x);
            } else {
                updateHeight(x);
            }

            x = parent;
        }
    }
};
