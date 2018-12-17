#pragma once

// key-value pairs
template <typename KeyT = int, typename ValueT = int>
struct BSTRangeSum {
    struct Node {
        Node*   parent;
        Node*   left;
        Node*   right;

        int     cnt;

        KeyT    key;
        ValueT  value;
        ValueT  sum;

        void init(KeyT key, ValueT value) {
            parent = nullptr;
            left = nullptr;
            right = nullptr;
            cnt = 1;

            this->key = key;
            this->value = sum = value;
        }
    };

    Node*   root;   // root node
    int     count;

    //-----------------------------------------------------------

    BSTRangeSum() {
        root = nullptr;
        count = 0;
    }

    ~BSTRangeSum() {
        deleteRecursive(root);
    }

    int size() const {
        return count;
    }

    bool empty() const {
        return count == 0;
    }

    bool exist(KeyT key) const {
        return find(key) != nullptr;
    }


    Node* operator [](int index) const {
        if (index < 0 || index >= count)
            return nullptr;

        int n = index;
        Node* p = root;
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

    Node* find(KeyT key) const {
        Node *p = root;

        while (p != nullptr && !(p->key == key)) {
            if (key < p->key)
                p = p->left;
            else
                p = p->right;
        }

        return p;
    }


    pair<Node*, bool> add(KeyT key, ValueT value) {
        Node* parent = nullptr;
        Node* x = root;

        while (x != nullptr) {
            parent = x;

            if (key == x->key) {
                x->value += value;
                x->sum += value;
                updateNodeToRoot(x->parent);
                return make_pair(x, false);
            } else {
                if (key < x->key)
                    x = x->left;
                else
                    x = x->right;
            }
        }

        Node* newNode = createNode(key, value);
        newNode->parent = parent;

        if (parent == nullptr)
            root = newNode;
        else {
            if (newNode->key < parent->key)
                parent->left = newNode;
            else
                parent->right = newNode;
            updateNodeToRoot(parent);
        }

        return make_pair(newNode, true);
    }

    pair<ValueT, bool> get(KeyT key) const {
        Node* p = find(key);
        if (!p)
            return make_pair(0, false);
        return make_pair(p->value, true);
    }

    ValueT sum(KeyT key) const {
        if (root == nullptr)
            return 0;

        ValueT res = 0;

        Node* y = nullptr;
        Node* x = root;
        while (x != nullptr) {
            if (key < x->key) {
                x = x->left;
            } else {
                res += x->value;
                if (x->left)
                    res += x->left->sum;
                x = x->right;
            }
        }
        return res;
    }

    ValueT sum(KeyT lo, KeyT hi) const {
        return sum(hi) - sum(lo - 1);
    }


    Node* lowerBound(KeyT key) const {
        if (root == nullptr)
            return nullptr;

        Node* y = nullptr;
        Node* x = root;
        while (x != nullptr) {
            if (!(x->key < key)) {
                y = x;
                x = x->left;
            } else
                x = x->right;
        }
        return y;
    }

    Node* upperBound(KeyT key) const {
        if (root == nullptr)
            return nullptr;

        Node* y = nullptr;
        Node* x = root;
        while (x != nullptr) {
            if (key < x->key) {
                y = x;
                x = x->left;
            } else
                x = x->right;
        }
        return y;
    }

    void clear() {
        deleteRecursive(root);
        root = nullptr;
    }

protected:
    Node* createNode(KeyT key, ValueT value) {
        Node* p = new Node();
        p->init(key, value);
        count++;
        return p;
    }

    void destroyNode(Node* node) {
        delete node;
        count--;
    }

    void deleteRecursive(Node* node) {
        if (node == nullptr || node == nullptr)
            return;

        vector<Node*> nodes;
        nodes.push_back(node);

        while (!nodes.empty()) {
            Node* p = nodes.back();
            nodes.pop_back();

            if (p->left != nullptr)
                nodes.push_back(p->left);
            if (p->right != nullptr)
                nodes.push_back(p->right);

            destroyNode(p);
        }
    }

    void updateNodeToRoot(Node* node) {
        if (node == nullptr)
            return;

        // update count
        while (node != nullptr) {
            onUpdateNode(node);
            node = node->parent;
        }
    }

    void onUpdateNode(Node* node) {
        if (node != nullptr) {
            node->cnt = (node->left ? node->left->cnt : 0) + (node->right ? node->right->cnt : 0) + 1;
            node->sum = (node->left ? node->left->sum : 0) + (node->right ? node->right->sum : 0) + node->value;
        }
    }
};
