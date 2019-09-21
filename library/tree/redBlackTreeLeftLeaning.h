#pragma once

// Left-leaning red-black trees (Guibas-Sedgewick, 1979 and Sedgewick, 2007)
// from https://algs4.cs.princeton.edu/code/edu/princeton/cs/algs4/RedBlackBST.java.html

// Benchmark result : in benchmarkBST.cpp

// T has to have below functions
//    1) operator ==()
//    2) operator <()

template <typename T>
struct LeftLeaningRBTree {
    enum RBColor {
        rbcBlack,
        rbcRed
    };

    struct Node {
        Node*   parent;
        Node*   left;
        Node*   right;

        int     cnt;

        RBColor color;  // color of incoming link
        T       value;

        explicit Node(const T& item, RBColor c = rgbRed) {
            init(item, c);
        }

        void init(const T& item, RBColor c = rbcRed) {
            parent = nullptr;
            left = nullptr;
            right = nullptr;
            cnt = 1;

            color = c;
            value = item;
        }

        Node* next() const {
            const Node* node = this;
            if (node->right != nullptr)
                return node->right->minimum();
            else {
                Node *cur, *parent;

                cur = const_cast<Node*>(node);
                parent = node->parent;
                while ((parent != nullptr) && (cur == parent->right)) {
                    cur = parent;
                    parent = parent->parent;
                }
                return parent;
            }
        }

        Node* prev() const {
            const Node* node = this;
            if (node->left != nullptr)
                return node->left->maximum();
            else {
                Node *cur, *parent;

                cur = const_cast<Node*>(node);
                parent = node->parent;
                while ((parent != nullptr) && (cur == parent->left)) {
                    cur = parent;
                    parent = parent->parent;
                }
                return parent;
            }
        }

        Node* minimum() const {
            Node* p = const_cast<Node*>(this);
            while (p->left != nullptr)
                p = p->left;
            return p;
        }

        Node* maximum() const {
            Node* p = const_cast<Node*>(this);
            while (p->right != nullptr)
                p = p->right;
            return p;
        }


        bool operator ==(const Node& rhs) const {
            return value == rhs.value;
        }

        bool operator <(const Node& rhs) const {
            return value < rhs.value;
        }
    };

    Node*       root;     // root node
    bool        duplicatable;
    int         count;

    //-----------------------------------------------------------

    LeftLeaningRBTree() {
        root = nullptr;
        duplicatable = false;
        count = 0;
    }

    LeftLeaningRBTree(const LeftLeaningRBTree<T>& tree) {
        root = nullptr;
        duplicatable = false;
        count = 0;
        copyRecursive(tree.root);
    }

    virtual ~LeftLeaningRBTree() {
        deleteRecursive(root);
    }

    LeftLeaningRBTree<T>& operator =(const LeftLeaningRBTree<T>& tree) {
        if (this != &tree) {
            deleteRecursive(root);
            duplicatable = tree.duplicatable;
            root = nullptr;
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

    bool isduplicatable() const {
        return duplicatable;
    }

    void setDuplicatable(bool enable) {
        duplicatable = enable;
    }

    const Node* begin() const {
        return root ? root->minimum() : nullptr;
    }

    Node* begin() {
        return root ? root->minimum() : nullptr;
    }

    // inclusive
    const Node* end() const {
        return root ? root->maximum() : nullptr;
    }

    // inclusive
    Node* end() {
        return root ? root->maximum() : nullptr;
    }

    bool exist(const T& key) const {
        return find(key) != nullptr;
    }

    Node* operator [](int index) const {
        assert((root != nullptr ? root->cnt : 0) == count);

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
        assert((root != nullptr ? root->cnt : 0) == count);

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

    Node* find(const T& key) const {
        Node *p = root;
        while (p != nullptr && !(p->value == key))
            p = (key < p->value) ? p->left : p->right;
        return p;
    }

    Node* lowerBound(const T& key) const {
        if (root == nullptr)
            return nullptr;

        Node* y = nullptr;
        Node* x = root;
        while (x != nullptr) {
            if (!(x->value < key)) {
                y = x;
                x = x->left;
            } else
                x = x->right;
        }
        return y;
    }

    Node* upperBound(const T& key) const {
        if (root == nullptr)
            return nullptr;

        Node* y = nullptr;
        Node* x = root;
        while (x != nullptr) {
            if (key < x->value) {
                y = x;
                x = x->left;
            } else
                x = x->right;
        }
        return y;
    }


    void insert(const T& item) {
        root = insert(root, item);
        root->parent = nullptr;
        root->color = rbcBlack;
    }

    void erase(const T& item) {
        if (!isRed(root->left) && !isRed(root->right))
            root->color = rbcRed;

        root = erase(root, item);
        if (root != nullptr) {
            root->parent = nullptr;
            root->color = rbcBlack;
        }
    }

    void eraseMin() {
        if (root == nullptr)
            return;

        if (!isRed(root->left) && !isRed(root->right))
            root->color = rbcRed;

        root = eraseMin(root);
        if (root != nullptr) {
            root->parent = nullptr;
            root->color = rbcBlack;
        }
    }

    void eraseMax() {
        if (root == nullptr)
            return;

        if (!isRed(root->left) && !isRed(root->right))
            root->color = rbcRed;

        root = eraseMax(root);
        if (root != nullptr) {
            root->parent = nullptr;
            root->color = rbcBlack;
        }
    }

    void clear() {
        deleteRecursive(root);
        root = nullptr;
    }

protected:
    Node* createNode(const T& item, RBColor c = rbcRed) {
        Node* p = new Node(item, c);
        count++;
        return p;
    }

    void destroyNode(Node* node) {
        //assert(node != nullptr);
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

    bool isRed(Node* node) {
        return node != nullptr && node->color == rbcRed;
    }

    Node* rotateRight(Node* node) {
        Node* y = node->left;

        // turn y's right subtree into node's left subtree
        node->left = y->right;
        if (y->right != nullptr)
            y->right->parent = node;

        // put node on y's right
        y->right = node;
        node->parent = y;

        y->color = node->color;
        node->color = rbcRed;

        updateNode(node);

        return y;
    }

    Node* rotateLeft(Node* node) {
        Node* y = node->right;

        // turn y's left subtree into node's right subtree
        node->right = y->left;
        if (y->left != nullptr)
            y->left->parent = node;

        // put node on y's left
        y->left = node;
        node->parent = y;

        y->color = node->color;
        node->color = rbcRed;

        updateNode(node);

        return y;
    }

    void flipColors(Node* node) {
        node->color = (node->color == rbcRed) ? rbcBlack : rbcRed;
        node->left->color = (node->left->color == rbcRed) ? rbcBlack : rbcRed;
        node->right->color = (node->right->color == rbcRed) ? rbcBlack : rbcRed;
    }

    // Assuming that node is red and both node.left and node.left.left are black, make node.left or one of its children red.
    Node* moveRedLeft(Node* node) {
        flipColors(node);
        if (isRed(node->right->left)) {
            node->right = rotateRight(node->right);
            if (node->right)
                node->right->parent = node;
            node = rotateLeft(node);
            flipColors(node);
        }
        return node;
    }

    // Assuming that node is red and both node.right and node.right.left are black, make node.right or one of its children red.
    Node* moveRedRight(Node* node) {
        flipColors(node);
        if (isRed(node->left->left)) {
            node = rotateRight(node);
            flipColors(node);
        }
        return node;
    }

    Node* balance(Node* node) {
        if (isRed(node->right))
            node = rotateLeft(node);
        if (isRed(node->left) && isRed(node->left->left))
            node = rotateRight(node);
        if (isRed(node->left) && isRed(node->right))
            flipColors(node);

        updateNode(node);

        return node;
    }


    Node* insert(Node* node, const T& item) {
        if (node == nullptr)
            return createNode(item);

        if (item == node->value && !duplicatable)
            node->value = item;
        else if (item < node->value) {
            node->left = insert(node->left, item);
            if (node->left)
                node->left->parent = node;
        } else {
            node->right = insert(node->right, item);
            if (node->right)
                node->right->parent = node;
        }

        // fix-up any right-leaning links
        if (isRed(node->right) && !isRed(node->left))
            node = rotateLeft(node);
        if (isRed(node->left) && isRed(node->left->left))
            node = rotateRight(node);
        if (isRed(node->left) && isRed(node->right))
            flipColors(node);

        updateNode(node);

        return node;
    }

    Node* eraseMin(Node* node) {
        if (node->left == nullptr) {
            destroyNode(node);
            return nullptr;
        }

        if (!isRed(node->left) && !isRed(node->left->left))
            node = moveRedLeft(node);

        node->left = eraseMin(node->left);
        if (node->left)
            node->left->parent = node;

        return balance(node);
    }

    Node* eraseMax(Node* node) {
        if (isRed(node->left))
            node = rotateRight(node);

        if (node->right == nullptr) {
            destroyNode(node);
            return nullptr;
        }

        if (!isRed(node->right) && !isRed(node->right->left))
            node = moveRedRight(node);

        node->right = eraseMax(node->right);
        if (node->right)
            node->right->parent = node;

        return balance(node);
    }

    Node* erase(Node* node, const T& item) {
        if (item < node->value) {
            if (!isRed(node->left) && !isRed(node->left->left))
                node = moveRedLeft(node);
            node->left = erase(node->left, item);
            if (node->left)
                node->left->parent = node;
        } else {
            if (isRed(node->left))
                node = rotateRight(node);
            if (item == node->value && node->right == nullptr) {
                destroyNode(node);
                return nullptr;
            }
            if (!isRed(node->right) && !isRed(node->right->left))
                node = moveRedRight(node);
            if (item == node->value) {
                Node* x = node->right->minimum();
                node->value = x->value;
                node->right = eraseMin(node->right);
                if (node->right)
                    node->right->parent = node;
            } else {
                node->right = erase(node->right, item);
                if (node->right)
                    node->right->parent = node;
            }
        }
        return balance(node);
    }


    void updateNode(Node* node) {
        if (node != nullptr)
            node->cnt = (node->left ? node->left->cnt : 0) + (node->right ? node->right->cnt : 0) + 1;

        //TODO: add custom actions
    }
};
