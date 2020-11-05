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

    Node* find(T key) {
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

    pair<Node*, bool> insert(T value) {
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

    bool erase(T key) {
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
        if (node == nullptr)
            return;

        vector<Node*> nodes;
        nodes.push_back(node);

        while (!nodes.empty()) {
            Node* p = nodes.back();
            nodes.pop_back();

            if (p->left)
                nodes.push_back(p->left);
            if (p->right)
                nodes.push_back(p->right);

            destroyNode(p);
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
