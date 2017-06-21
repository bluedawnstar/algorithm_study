#pragma once

#include <algorithm>

template <typename T>
struct Treap {
    struct Node {
        Node* parent;
        Node* left;
        Node* right;

        int   priority;
        int   cnt;

        T     value;

        void init() {
            parent = left = right = nullptr;
            priority = rand() * rand();
            cnt = 1;
        }
    };

    int     mCount;
    Node*   mRoot;

    Treap() {
        mCount = 0;
        mRoot = nullptr;
    }

    ~Treap() {
        deleteRecursive(mRoot);
    }

    int size() const {
        return mCount;
    }

    Node* find(const T& key) {
        Node *p = mRoot;

        while (p && !(p->value == key)) {
            if (key < p->value)
                p = p->left;
            else
                p = p->right;
        }

        return p;
    }

    Node* operator [](int nth) {
        assert((mRoot ? mRoot->cnt : 0) == mCount);

        if (index < 0 || index >= mCount)
            return nullptr;

        int n = index;
        Node* p = mRoot;
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

    int indexOf(Node* p) {
        assert((mRoot ? mRoot->cnt : 0) == mCount);

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

    Node* insert(const T& value) {
        Node* p = createNode(value);
        mRoot = insert(mRoot, p);
        return p;
    }

    bool erase(const T& key) {
        mRoot = erase(mRoot, key);
        return true;
    }

protected:
    Node* createNode(const T& item) {
        Node* p = new Node();
        p->init();
        p->value = item;
        mCount++;
        return p;
    }

    void destroyNode(Node* node) {
        mCount--;
        delete node;
    }

    void deleteRecursive(Node* node) {
        if (node != nullptr) {
            deleteRecursive(node->left);
            deleteRecursive(node->right);
            destroyNode(node);
        }
    }


    pair<Node*, Node*> split(Node* root, const T& key) {
        if (root == nullptr)
            return make_pair(nullptr, nullptr);

        if (root->value < key) {
            auto rs = split(root->right, key);
            root->right = rs.first;
            if (rs.first)
                rs.first->parent = root;
            update(root);
            return make_pair(root, rs.second);
        } else {
            auto ls = split(root->left, key);
            root->left = ls.second;
            if (ls.second)
                ls.second->parent = root;
            return make_pair(ls.first, root);
        }
    }

    Node* insert(Node* root, Node* node) {
        if (root == nullptr)
            return node;

        if (root->priority < node->priority) {
            auto splitted = split(root, node->value);
            node->left = splitted.first;
            node->right = splitted.second;
            if (splitted.first)
                splitted.first->parent = node;
            if (splitted.second)
                splitted.second->parent = node;
            update(node);
            return node;
        } else if (node->value < root->value) {
            root->left = insert(root->left, node);
            if (root->left)
                root->left->parent = root;
            update(root);
        } else {
            root->right = insert(root->right, node);
            if (root->right)
                root->right->parent = root;
            update(root);
        }

        return root;
    }


    Node* merge(Node* a, Node* b) {
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

    Node* erase(Node* root, const T& key) {
        if (!root)
            return root;
        if (root->value == key) {
            Node* res = merge(root->left, root->right);
            destroyNode(root);
            return res;
        }
        if (key < root->value) {
            root->left = erase(root->left, key);
            if (root->left)
                root->left->parent = root;
        } else {
            root->right = erase(root->right, key);
            if (root->right)
                root->right->parent = root;
        }
        update(root);
        return root;
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
