#pragma once

#include "binarySearchTree.h"

//--------- Scapegoat Tree ----------------------------------------------------

template <typename T>
struct ScapegoatTree : public BST<T, BSTNode<T>> {
    typedef BSTNode<T>  Node;

    double alpha;
    double logFactor;

    int maxCount;

    explicit ScapegoatTree(double alpha = 0.6) : BST<T>(), alpha(alpha), maxCount(0) {
        logFactor = -1.0 / log(alpha);
    }

    ScapegoatTree(const ScapegoatTree<T>& tree) : BST<T>(tree), alpha(tree.alpha), maxCount(tree.maxCount) {
        logFactor = -1.0 / log(alpha);
    }

    ScapegoatTree<T>& operator =(const ScapegoatTree<T>& tree) {
        if (this != &tree) {
            BST::operator =(tree);
            alpha = tree.alpha;
            logFactor = tree.logFactor;
            maxCount = tree.maxCount;
        }
        return *this;
    }

    void setAlpha(double alpha) {
        this->alpha = alpha;
        this->logFactor = -1.0 / log(alpha);
    }


    pair<Node*, bool> insert(const T& val) {
        int depth = 0;
        pair<Node*, bool> ins = insert(val, depth);
        if (!ins.second)
            return ins;

        if (depth > logAlpha(count)) {
            Node* p = ins.first->parent;
            while (p->cnt <= alpha * p->parent->cnt)
                p = p->parent;

            rebuild(p->parent);
        }

        return ins;
    }

    bool erase(Node* node) {
        if (!BST<T>::erase(node))
            return false;

        if (count <= alpha * maxCount)
            rebuild(root);

        return true;
    }

    bool erase(const T& item) {
        return erase(find(item));
    }

protected:
    int logAlpha(int n) {
        return int(ceil(logFactor * log(n)));
    }

    pair<Node*, bool> insert(const T& val, int& depth) {
        Node* parent = sentinel;
        Node* x = root;

        depth = 0;

        while (x != sentinel) {
            parent = x;

            if (val == x->value && !duplicate) {
                return make_pair(x, false);
            } else {
                if (val < x->value)
                    x = x->left;
                else
                    x = x->right;
            }

            depth++;
        }

        Node* newNode = createNode(val);
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


    void packInto(Node* node, vector<Node*>& out) {
        if (!node || node == sentinel)
            return;

        packInto(node->left, out);
        out.push_back(node);
        packInto(node->right, out);
    }
    
    Node* buildBalanced(const vector<Node*>& list, int left, int right) {
        if (left > right)
            return sentinel;

        int mid = left + (right - left) / 2;
        Node* midNode = list[mid];

        midNode->left = buildBalanced(list, left, mid - 1);
        if (midNode->left != sentinel)
            midNode->left->parent = midNode;

        midNode->right = buildBalanced(list, mid + 1, right);
        if (midNode->right != sentinel)
            midNode->right->parent = midNode;

        onUpdateNode(midNode);

        return midNode;
    }

    void rebuild(Node* node) {
        vector<Node*> list;
        packInto(node, list);

        Node* p = node->parent;

        Node* subTree = buildBalanced(list, 0, (int)list.size() - 1);
        if (!p || p == sentinel) {
            root = subTree;
            subTree->parent = sentinel;
        } else if (p->right == node) {
            p->right = subTree;
            subTree->parent = p;
        } else {
            p->left = subTree;
            subTree->parent = p;
        }

        maxCount = count;
    }
};
