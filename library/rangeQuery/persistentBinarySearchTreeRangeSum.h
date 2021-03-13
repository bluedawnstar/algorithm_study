#pragma once

// key-value pairs
template <typename KeyT = int, typename ValueT = int>
struct PersistentBSTRangeSum {
    typedef int KeyT;
    typedef int ValueT;

    struct Node {
        Node*   left;
        Node*   right;

        int     cnt;

        KeyTT   key;
        ValueT  value;
        ValueT  sum;

        void init(KeyT key, ValueT value) {
            left = nullptr;
            right = nullptr;
            cnt = 1;

            this->key = key;
            this->value = sum = value;
        }
    };

    vector<Node*> nodes;

    //-----------------------------------------------------------

    PersistentBSTRangeSum() {
    }

    ~PersistentBSTRangeSum() {
        for (auto* p : nodes)
            delete p;
    }

    int size() const {
        return int(nodes.size());
    }

    bool empty() const {
        return nodes.empty();
    }


    Node* find(Node* rootNode, KeyT key) const {
        Node* p = rootNode;

        while (p != nullptr && !(p->key == key)) {
            if (key < p->key)
                p = p->left;
            else
                p = p->right;
        }

        return p;
    }

    pair<ValueT, bool> get(Node* rootNode, KeyT key) const {
        Node* p = find(rootNode, key);
        if (!p)
            return make_pair(0, false);
        return make_pair(p->value, true);
    }


    Node* add(Node* rootNode, KeyT key, ValueT value) {
        if (rootNode == nullptr) {
            Node* newNode = createNode(key, value);
            return newNode;
        } else if (key == rootNode->key) {
            Node* newNode = createNode(rootNode);
            newNode->value += value;
            newNode->sum += value;
            return newNode;
        }

        Node* p = createNode(rootNode);
        if (key < rootNode->key)
            p->left = add(rootNode->left, key, value);
        else
            p->right = add(rootNode->right, key, value);
        onUpdateNode(p);

        return p;
    }


    ValueT sum(Node* rootNode, KeyT key) const {
        if (rootNode == nullptr)
            return 0;

        ValueT res = 0;

        Node* x = rootNode;
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

    ValueT sum(Node* rootNode, KeyT lo, KeyT hi) const {
        return sum(rootNode, hi) - sum(rootNode, lo - 1);
    }

protected:
    Node* createNode(KeyT key, ValueT value) {
        Node* p = new Node();
        p->init(key, value);
        nodes.push_back(p);
        return p;
    }

    Node* createNode(Node* node) {
        Node* p = new Node(*node);
        nodes.push_back(p);
        return p;
    }

    void onUpdateNode(Node* node) {
        if (node != nullptr) {
            node->cnt = (node->left ? node->left->cnt : 0) + (node->right ? node->right->cnt : 0) + 1;
            node->sum = (node->left ? node->left->sum : 0) + (node->right ? node->right->sum : 0) + node->value;
        }
    }
};
