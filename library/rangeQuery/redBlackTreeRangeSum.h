#pragma once

// key-value pairs
template <typename KeyT = int, typename ValueT = int>
struct RBTreeRangeSum {
    enum RBColor {
        rbcBlack,
        rbcRed
    };

    struct Node {
        Node*   parent;
        Node*   left;
        Node*   right;

        int     cnt;

        RBColor color;

        KeyT    key;
        ValueT  value;
        ValueT  sum;

        void init(KeyT key, ValueT value) {
            parent = nullptr;
            left = nullptr;
            right = nullptr;
            cnt = 1;

            color = rbcBlack;
            this->key = key;
            this->value = sum = value;
        }

        bool operator ==(const Node& rhs) const {
            return key == rhs.key;
        }

        bool operator <(const Node& rhs) const {
            return key < rhs.key;
        }
    };

    Node*   root;   // root node
    int     count;

    //-----------------------------------------------------------

    RBTreeRangeSum() {
        root = nullptr;
        count = 0;
    }

    ~RBTreeRangeSum() {
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


    pair<Node*, bool> add(KeyT item, KeyT value) {
        pair<Node*, bool> ins = addBST(item, value);
        if (!ins.second)
            return ins;

        Node* x = ins.first;
        Node* y;

        x->color = rbcRed;
        while ((x != root) && (x->parent && x->parent->color == rbcRed)) {
            if (x->parent == x->parent->parent->left) {
                y = x->parent->parent->right;

                if (y && y->color == rbcRed) {
                    x->parent->color = rbcBlack;
                    y->color = rbcBlack;
                    x->parent->parent->color = rbcRed;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->right) {
                        x = x->parent;
                        rotateLeft(x);
                    }

                    x->parent->color = rbcBlack;
                    x->parent->parent->color = rbcRed;
                    rotateRight(x->parent->parent);
                }
            } else {
                y = x->parent->parent->left;

                if (y && y->color == rbcRed) {
                    x->parent->color = rbcBlack;
                    y->color = rbcBlack;
                    x->parent->parent->color = rbcRed;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->left) {
                        x = x->parent;
                        rotateRight(x);
                    }

                    x->parent->color = rbcBlack;
                    x->parent->parent->color = rbcRed;
                    rotateLeft(x->parent->parent);
                }
            }
        }
        root->color = rbcBlack;

        return ins;
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
    Node* createNode(KeyT item, KeyT value) {
        Node* p = new Node;
        p->init(item, value);
        count++;
        return p;
    }

    void destroyNode(Node* node) {
        delete node;
        count--;
    }

    void deleteRecursive(Node* node) {
        if (node != nullptr) {
            deleteRecursive(node->left);
            deleteRecursive(node->right);
            destroyNode(node);
        }
    }

    pair<Node*, bool> addBST(KeyT item, ValueT value) {
        Node* parent = nullptr;
        Node* x = root;

        while (x != nullptr) {
            parent = x;

            if (item == x->key) {
                x->value += value;
                x->sum += value;
                updateNodeToRoot(parent);
                return make_pair(x, false);
            } else {
                if (item < x->key)
                    x = x->left;
                else
                    x = x->right;
            }
        }

        Node* newNode = createNode(item, value);
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

    void rotateLeft(Node* node) {
        if (node == nullptr || node->right == nullptr)
            return;

        Node* y = node->right;

        // turn y's left subtree into node's right subtree
        node->right = y->left;
        if (y->left != nullptr)
            y->left->parent = node;

        // link node's parent to y
        y->parent = node->parent;
        if (node->parent == nullptr)
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

        updateNode(node);
        updateNode(y);
    }

    void rotateRight(Node* node) {
        if (node == nullptr || node->left == nullptr)
            return;

        Node* y = node->left;

        // turn y's right subtree into node's left subtree
        node->left = y->right;
        if (y->right != nullptr)
            y->right->parent = node;

        // link node's parent to y
        y->parent = node->parent;
        if (node->parent == nullptr)
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

        updateNode(node);
        updateNode(y);
    }

    void updateNodeToRoot(Node* node) {
        // update count
        while (node != nullptr) {
            updateNode(node);
            node = node->parent;
        }
    }

    void updateNode(Node* node) {
        if (node != nullptr) {
            node->cnt = (node->left ? node->left->cnt : 0) + (node->right ? node->right->cnt : 0) + 1;
            node->sum = (node->left ? node->left->sum : 0) + (node->right ? node->right->sum : 0) + node->value;
        }
    }
};
