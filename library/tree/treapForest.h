#pragma once

#include <algorithm>

// Treap (Cartesian tree) = tree + heap
template <typename T>
struct TreapForest {
    struct Node {
        Node* left;
        Node* right;

        int   cnt;

        T     value;        // X
        int   priority;     // Y

        void init(T value, int priority) {
            left = right = nullptr;
            this->value = value;
            this->priority = priority;
            cnt = 1;
        }
    };

    vector<Node*> nodes;
    vector<Node*> freeNodes;

    TreapForest() {
    }

    ~TreapForest() {
        clear();
    }

    void clear() {
        for (auto* p : nodes)
            delete p;
        nodes.clear();
        freeNodes.clear();
    }

    int size(Node* root) const {
        return root ? root->cnt : 0;
    }

    // O(logN)
    Node* find(Node* root, T key) const {
        Node* p = root;

        while (p && !(p->value == key)) {
            if (key < p->value)
                p = p->left;
            else
                p = p->right;
        }

        return p;
    }

    // O(logN)
    Node* kth(Node* root, int k) const {
        int count = size(root);
        if (k < 0 || k >= count)
            return nullptr;

        int n = k;
        Node* p = root;
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

    //---

    // O(N)
    // precondition : A must be sorted
    Node* build(const T A[], int N) {
        return recBuild(A, 0, N - 1);
    }

    // O(N)
    // precondition : A must be sorted
    Node* build(const vector<T>& A) {
        return recBuild(A.data(), 0, int(A.size()) - 1);
    }


    // return new root, O(logN)
    Node* insert(Node* root, T value, int priority, Node** newNode = nullptr) {
        Node* p = createNode(value, priority);
        root = recInsert(root, p);
        if (newNode)
            *newNode = p;
        return root;
    }

    Node* insert(Node* root, T value, Node** newNode = nullptr) {
        Node* p = createNode(value, random());
        root = recInsert(root, p);
        if (newNode)
            *newNode = p;
        return root;
    }

    // return new root, O(logN)
    Node* erase(Node* root, T key) {
        return recErase(root, key);
    }

    // union, O(M*log(N/M))
    Node* unite(Node* rootL, Node* rootR) {
        if (!rootL || !rootR)
            return rootL ? rootL : rootR;

        if (rootL->priority < rootR->priority)
            swap(rootL, rootR);

        auto r = split(rootR, rootL->value);
        rootL->left = unite(rootL->left, r.first);
        rootL->right = unite(rootL->right, r.second);
        return rootL;
    }

protected:
    Node* createNode(T value, int priority) {
        Node* p = nullptr;
        if (!freeNodes.empty()) {
            p = freeNodes.back();
            freeNodes.pop_back();
        } else {
            p = new Node();
            nodes.push_back(p);
        }
        p->init(value, priority);
        return p;
    }

    void deleteNode(Node* p) {
        freeNodes.push_back(p);
    }

    //---

    pair<Node*, Node*> split(Node* root, T key) {
        if (root == nullptr)
            return make_pair(nullptr, nullptr);

        if (key < root->value) {
            auto ls = split(root->left, key);
            root->left = ls.second;
            update(root);
            return make_pair(ls.first, root);
        } else {
            auto rs = split(root->right, key);
            root->right = rs.first;
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
            update(b);
            return b;
        } else {
            a->right = merge(a->right, b);
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
        Node* p = createNode(A[mid], random());

        p->left = recBuild(A, L, mid - 1);
        p->right = recBuild(A, mid + 1, R);

        heapify(p);
        update(p);
        return p;
    }

    Node* recInsert(Node* root, Node* node) {
        if (root == nullptr)
            return node;

        if (root->priority < node->priority) {
            tie(node->left, node->right) = split(root, node->value);
            update(node);
            return node;
        }

        if (node->value < root->value)
            root->left = recInsert(root->left, node);
        else
            root->right = recInsert(root->right, node);

        update(root);
        return root;
    }

    Node* recErase(Node* root, T key) {
        if (!root)
            return root;

        if (root->value == key) {
            Node* res = merge(root->left, root->right);
            deleteNode(root);
            return res;
        }

        if (key < root->value)
            root->left = recErase(root->left, key);
        else
            root->right = recErase(root->right, key);

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
