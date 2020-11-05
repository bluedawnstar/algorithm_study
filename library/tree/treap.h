#pragma once

#include <algorithm>

// Treap (Cartesian tree) = tree + heap
template <typename T>
struct Treap {
    struct Node {
        Node* parent;
        Node* left;
        Node* right;

        int   cnt;

        int   priority;
        T     value;

        void init() {
            parent = left = right = nullptr;
            priority = Treap<T>::random();
            cnt = 1;
        }
    };

    int     count;
    Node*   tree;

    Treap() {
        count = 0;
        tree = nullptr;
    }

    ~Treap() {
        deleteRecursive(tree);
    }

    int size() const {
        return count;
    }

    // O(logN)
    Node* find(const T& key) {
        Node* p = tree;

        while (p && !(p->value == key)) {
            if (key < p->value)
                p = p->left;
            else
                p = p->right;
        }

        return p;
    }

    // O(logN)
    Node* operator [](int nth) {
        assert((tree ? tree->cnt : 0) == count);

        if (nth < 0 || nth >= count)
            return nullptr;

        int n = nth;
        Node* p = tree;
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

    // O(logN)
    int indexOf(Node* p) {
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

    // O(logN)
    Node* insert(const T& value) {
        Node* p = createNode(value);
        tree = insert(tree, p);
        tree->parent = nullptr;
        return p;
    }

    // O(logN)
    bool erase(const T& key) {
        tree = erase(tree, key);
        if (tree)
            tree->parent = nullptr;
        return true;
    }

protected:
    Node* createNode(const T& item) {
        Node* p = new Node();
        p->init();
        p->value = item;
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

    //---

    pair<Node*, Node*> split(Node* root, const T& key) {
        if (root == nullptr)
            return make_pair(nullptr, nullptr);

        if (key < root->value) {
            auto ls = split(root->left, key);
            root->left = ls.second;
            if (ls.second)
                ls.second->parent = root;
            update(root);
            return make_pair(ls.first, root);
        } else {
            auto rs = split(root->right, key);
            root->right = rs.first;
            if (rs.first)
                rs.first->parent = root;
            update(root);
            return make_pair(root, rs.second);
        }
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

    //---

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
        }

        if (node->value < root->value) {
            root->left = insert(root->left, node);
            if (root->left)
                root->left->parent = root;
        } else {
            root->right = insert(root->right, node);
            if (root->right)
                root->right->parent = root;
        }
        update(root);
        return root;
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

    //---

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

    //---

    static int random() {
        //static std::random_device rd;
        //static std::mt19937 eng(rd());
        static std::mt19937 eng(7);
        static std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());
        return dist(eng);
    }
};
